#pragma once
namespace nsK2EngineLow 
{
	class ShadowMapRender
	{
	public:
		void Init();
		void Draw(RenderContext& rc);

		void InitShadowMapRender();
		void LightCamera();
		void InitShadowMapSprite();
		void Render(RenderContext& rc);
		void ShadowMapSpriteRender(RenderContext& rc);
	private:
		RenderTarget m_shadowMap;
		Sprite sprite;
	};
	extern ShadowMapRender g_shadowMapRender;
};