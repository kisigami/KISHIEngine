#pragma once
namespace nsK2EngineLow {
	class Monochrome
	{
	public:
		void Init();
		void Draw(RenderContext& rc);

		void InitOffscreenRenderTarget();
		void InitPostEffectSprite();
		void ChangeRenderTarget(RenderContext& rc);
		void ChangeOnscreenRenderTarget(RenderContext& rc);
		void DrawFullscreenSprite(RenderContext& rc);

	private:
		RenderTarget m_offscreenRenderTarget;
		Sprite m_monochromeSprite;
	};
	extern Monochrome g_monochrome;
}
