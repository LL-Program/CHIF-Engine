#pragma once
#include "chifFont.h"
#include "chifAudio.h"
#include "chifArchive.h"
#include "chifECS.h"
#include "chifResourceManager.h"

#include <string>

namespace chif
{
	class SpriteFont
	{
	public:
		enum FLAGS
		{
			EMPTY = 0,
			HIDDEN = 1 << 0,
			DISABLE_UPDATE = 1 << 1,
			CAMERA_FACING = 1 << 2,
			CAMERA_SCALING = 1 << 3,
		};
		uint32_t _flags = EMPTY;
		std::wstring text;
		chif::font::Params params;
		chif::Resource fontStyleResource;
		std::string fontStyleName;

		SpriteFont() = default;
		SpriteFont(const std::string& value, const chif::font::Params& params = chif::font::Params(), const std::string& fontStyleName = "") :params(params), fontStyleName(fontStyleName)
		{
			SetText(value);
			if (!fontStyleName.empty())
			{
				fontStyleResource = chif::resourcemanager::Load(fontStyleName);
			}
		}
		virtual ~SpriteFont() = default;

		virtual void FixedUpdate();
		virtual void Update(float dt);
		virtual void Draw(chif::graphics::CommandList cmd) const;

		constexpr void SetHidden(bool value = true) { if (value) { _flags |= HIDDEN; } else { _flags &= ~HIDDEN; } }
		constexpr bool IsHidden() const { return _flags & HIDDEN; }
		constexpr void SetDisableUpdate(bool value = true) { if (value) { _flags |= DISABLE_UPDATE; } else { _flags &= ~DISABLE_UPDATE; } }
		constexpr void SetCameraFacing(bool value = true) { if (value) { _flags |= CAMERA_FACING; } else { _flags &= ~CAMERA_FACING; } }
		constexpr void SetCameraScaling(bool value = true) { if (value) { _flags |= CAMERA_SCALING; } else { _flags &= ~CAMERA_SCALING; } }

		constexpr bool IsDisableUpdate() const { return _flags & DISABLE_UPDATE; }
		constexpr bool IsCameraFacing() const { return _flags & CAMERA_FACING; }
		constexpr bool IsCameraScaling() const { return _flags & CAMERA_SCALING; }

		XMFLOAT2 TextSize() const;
		float TextWidth() const;
		float TextHeight() const;

		void SetText(const std::string& value);
		void SetText(std::string&& value);
		void SetText(const std::wstring& value);
		void SetText(std::wstring&& value);

		std::string GetTextA() const;
		const std::wstring& GetText() const;

		size_t GetCurrentTextLength() const;

		struct Animation
		{
			struct Typewriter
			{
				float time = 0; // time to fully type the text in seconds (0: disable)
				bool looped = false; // if true, typing starts over when finished
				size_t character_start = 0; // starting character for the animation
				chif::audio::Sound sound;
				chif::audio::SoundInstance soundinstance;

				float elapsed = 0; // internal use; you don't need to initialize

				void reset()
				{
					elapsed = 0;
				}
				void Finish()
				{
					elapsed = time;
				}
				bool IsFinished() const
				{
					return time <= elapsed;
				}
			} typewriter;
		} anim;

		void Serialize(chif::Archive& archive, chif::ecs::EntitySerializer& seri);
	};
}
