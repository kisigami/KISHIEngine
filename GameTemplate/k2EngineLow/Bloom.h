#pragma once

namespace nsK2EngineLow
{
	class Bloom
	{
	public:
		void Init();
	private:
		RenderTarget m_mainRenderTarget;
		RenderTarget m_luminanceRenderTarget;
		SpriteInitData m_luminanceSpriteInitData;
		Sprite m_luminanceSprite;
	};

};