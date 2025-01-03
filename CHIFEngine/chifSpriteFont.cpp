#include "chifSpriteFont.h"
#include "chifHelper.h"

using namespace chif::graphics;

namespace chif
{

	void SpriteFont::FixedUpdate()
	{
		if (IsDisableUpdate())
			return;
	}
	void SpriteFont::Update(float dt)
	{
		if (IsDisableUpdate())
			return;

		if (fontStyleResource.IsValid())
		{
			params.style = fontStyleResource.GetFontStyle();
		}
		else
		{
			params.style = 0;
		}

		if (anim.typewriter.time > 0)
		{
			size_t text_length = text.length();
			size_t text_length_prev = std::min(text_length, size_t(chif::math::Lerp(float(std::min(text_length, anim.typewriter.character_start)), float(text_length + 1), anim.typewriter.elapsed / anim.typewriter.time)));
			anim.typewriter.elapsed += dt;
			size_t text_length_next = std::min(text_length, size_t(chif::math::Lerp(float(std::min(text_length, anim.typewriter.character_start)), float(text_length + 1), anim.typewriter.elapsed / anim.typewriter.time)));

			if (anim.typewriter.soundinstance.IsValid())
			{
				if (!anim.typewriter.IsFinished() && chif::audio::IsEnded(&anim.typewriter.soundinstance) && text_length_prev != text_length_next && text[text_length_next - 1] != ' ' && anim.typewriter.soundinstance.IsValid())
				{
					chif::audio::Stop(&anim.typewriter.soundinstance);
					if (!IsHidden())
					{
						chif::audio::Play(&anim.typewriter.soundinstance);
					}
				}
				chif::audio::ExitLoop(&anim.typewriter.soundinstance);
			}

			if (anim.typewriter.looped && anim.typewriter.elapsed > anim.typewriter.time)
			{
				anim.typewriter.reset();
			}
		}
	}

	void SpriteFont::Draw(CommandList cmd) const
	{
		if (IsHidden())
			return;

		chif::font::Draw(text.c_str(), GetCurrentTextLength(), params, cmd);
	}

	XMFLOAT2 SpriteFont::TextSize() const
	{
		return chif::font::TextSize(text, params);
	}
	float SpriteFont::TextWidth() const
	{
		return chif::font::TextWidth(text, params);
	}
	float SpriteFont::TextHeight() const
	{
		return chif::font::TextHeight(text, params);
	}

	void SpriteFont::SetText(const std::string& value)
	{
		chif::helper::StringConvert(value, text);
	}
	void SpriteFont::SetText(std::string&& value)
	{
		chif::helper::StringConvert(value, text);
	}
	void SpriteFont::SetText(const std::wstring& value)
	{
		text = value;
	}
	void SpriteFont::SetText(std::wstring&& value)
	{
		text = std::move(value);
	}

	std::string SpriteFont::GetTextA() const
	{
		std::string retVal;
		chif::helper::StringConvert(text, retVal);
		return retVal;
	}
	const std::wstring& SpriteFont::GetText() const
	{
		return text;
	}

	size_t SpriteFont::GetCurrentTextLength() const
	{
		size_t text_length = text.length();
		if (anim.typewriter.time > 0)
		{
			text_length = std::min(text_length, size_t(chif::math::Lerp(float(std::min(text_length, anim.typewriter.character_start)), float(text_length + 1), anim.typewriter.elapsed / anim.typewriter.time)));
		}
		return text_length;
	}

	void SpriteFont::Serialize(chif::Archive& archive, chif::ecs::EntitySerializer& seri)
	{
		const std::string& dir = archive.GetSourceDirectory();

		if (archive.IsReadMode())
		{
			archive >> _flags;
			std::string textA;
			archive >> textA;
			SetText(textA);
			archive >> fontStyleName;

			archive >> params._flags;
			archive >> params.position;
			archive >> params.size;
			archive >> params.scaling;
			archive >> params.rotation;
			archive >> params.spacingX;
			archive >> params.spacingY;
			archive >> *(uint32_t*)&params.h_align;
			archive >> *(uint32_t*)&params.v_align;
			archive >> params.color;
			archive >> params.shadowColor;
			archive >> params.h_wrap;
			archive >> params.softness;
			archive >> params.bolden;
			archive >> params.shadow_softness;
			archive >> params.shadow_bolden;
			archive >> params.shadow_offset_x;
			archive >> params.shadow_offset_y;
			archive >> params.hdr_scaling;
			archive >> params.intensity;
			archive >> params.shadow_intensity;

			archive >> anim.typewriter.time;
			archive >> anim.typewriter.looped;
			archive >> anim.typewriter.character_start;

			if (!fontStyleName.empty())
			{
				chif::jobsystem::Execute(seri.ctx, [&](chif::jobsystem::JobArgs args) {
					if (!fontStyleName.empty())
					{
						fontStyleName = dir + fontStyleName;
						fontStyleResource = chif::resourcemanager::Load(fontStyleName);
						params.style = fontStyleResource.GetFontStyle();
					}
				});
			}
		}
		else
		{
			seri.RegisterResource(fontStyleName);

			archive << _flags;
			std::string textA = GetTextA();
			archive << textA;
			archive << chif::helper::GetPathRelative(dir, fontStyleName);

			archive << params._flags;
			archive << params.position;
			archive << params.size;
			archive << params.scaling;
			archive << params.rotation;
			archive << params.spacingX;
			archive << params.spacingY;
			archive << *(uint32_t*)&params.h_align;
			archive << *(uint32_t*)&params.v_align;
			archive << params.color;
			archive << params.shadowColor;
			archive << params.h_wrap;
			archive << params.softness;
			archive << params.bolden;
			archive << params.shadow_softness;
			archive << params.shadow_bolden;
			archive << params.shadow_offset_x;
			archive << params.shadow_offset_y;
			archive << params.hdr_scaling;
			archive << params.intensity;
			archive << params.shadow_intensity;

			archive << anim.typewriter.time;
			archive << anim.typewriter.looped;
			archive << anim.typewriter.character_start;
		}
	}

}
