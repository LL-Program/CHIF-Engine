#pragma once
#include "chifRenderPath.h"
#include "chifGUI.h"
#include "chifVector.h"

#include <string>

namespace chif
{
	class Sprite;
	class SpriteFont;

	class RenderPath2D :
		public RenderPath
	{
	protected:
		chif::graphics::Texture rtStenciled;
		chif::graphics::Texture rtStenciled_resolved;
		chif::graphics::Texture rtFinal;
		chif::graphics::Texture rtFinal_MSAA;

		chif::gui::GUI GUI;

		XMUINT2 current_buffersize{};
		float current_layoutscale{};

		float hdr_scaling = 9.0f;

		uint32_t msaaSampleCount = 1;

	public:
		// Delete GPU resources and initialize them to default
		virtual void DeleteGPUResources();
		// create resolution dependent resources, such as render targets
		virtual void ResizeBuffers();
		// update DPI dependent elements, such as GUI elements, sprites
		virtual void ResizeLayout();

		void Update(float dt) override;
		void FixedUpdate() override;
		void Render() const override;
		void Compose(chif::graphics::CommandList cmd) const override;

		virtual void setMSAASampleCount(uint32_t value) { msaaSampleCount = value; }
		constexpr uint32_t getMSAASampleCount() const { return msaaSampleCount; }

		const chif::graphics::Texture& GetRenderResult() const { return rtFinal; }
		virtual const chif::graphics::Texture* GetDepthStencil() const { return nullptr; }
		virtual const chif::graphics::Texture* GetGUIBlurredBackground() const { return nullptr; }

		void AddSprite(chif::Sprite* sprite, const std::string& layer = "");
		void RemoveSprite(chif::Sprite* sprite);
		void ClearSprites();
		int GetSpriteOrder(chif::Sprite* sprite);

		void AddFont(chif::SpriteFont* font, const std::string& layer = "");
		void RemoveFont(chif::SpriteFont* font);
		void ClearFonts();
		int GetFontOrder(chif::SpriteFont* font);

		struct RenderItem2D
		{
			enum class TYPE
			{
				SPRITE,
				FONT,
			} type = TYPE::SPRITE;
			union
			{
				chif::Sprite* sprite = nullptr;
				chif::SpriteFont* font;
			};
			int order = 0;
		};
		struct RenderLayer2D
		{
			chif::vector<RenderItem2D> items;
			std::string name;
			int order = 0;
		};
		chif::vector<RenderLayer2D> layers{ 1 };
		void AddLayer(const std::string& name);
		void SetLayerOrder(const std::string& name, int order);
		void SetSpriteOrder(chif::Sprite* sprite, int order);
		void SetFontOrder(chif::SpriteFont* font, int order);
		void SortLayers();
		void CleanLayers();

		const chif::gui::GUI& GetGUI() const { return GUI; }
		chif::gui::GUI& GetGUI() { return GUI; }

		float resolutionScale = 1.0f;
		XMUINT2 GetInternalResolution() const
		{
			return XMUINT2(
				uint32_t((float)GetPhysicalWidth() * resolutionScale),
				uint32_t((float)GetPhysicalHeight() * resolutionScale)
			);
		}

		float GetHDRScaling() const { return hdr_scaling; }
		void SetHDRScaling(float value) { hdr_scaling = value; }
	};

}
