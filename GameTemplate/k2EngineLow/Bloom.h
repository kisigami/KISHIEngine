#pragma once

namespace nsK2EngineLow
{
	class Bloom
	{
	public:
		void Init(RenderTarget& mainRenderTargete);
		void Render(RenderContext& rc , RenderTarget& mainRenderTarget);
	private:
		RenderTarget m_mainRenderTarget;
		RenderTarget m_luminanceRenderTarget;
		SpriteInitData m_luminanceSpriteInitData;
		Sprite m_luminanceSprite;
		GaussianBlur m_gaussianBlur[4];
		SpriteInitData m_finalSpriteInitData;
		Sprite m_finalSprite;
		Sprite m_copyToFrameBufferSprite;
		SpriteInitData m_spriteInitData;
	};
};