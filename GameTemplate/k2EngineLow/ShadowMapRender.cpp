#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	ShadowMapRender g_shadowMapRender;
	void ShadowMapRender::Init()
	{
		InitShadowMapRender();
		LightCamera();
		InitShadowMapSprite();
	}

	void ShadowMapRender::Draw(RenderContext& rc)
	{
		Render(rc);
		ShadowMapSpriteRender(rc);
	}

	void ShadowMapRender::InitShadowMapRender()
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void ShadowMapRender::LightCamera()
	{
		Camera lightCamera;
		lightCamera.SetPosition(0, 600, 0);
		lightCamera.SetTarget(0, 0, 0);
		lightCamera.SetUp(1, 0, 0);
		lightCamera.SetViewAngle(Math::DegToRad(20.0f));
		lightCamera.Update();
	}

	void ShadowMapRender::InitShadowMapSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_shadowMap.GetRenderTargetTexture();
		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		spriteInitData.m_width = 256;
		spriteInitData.m_height = 256;
		sprite.Init(spriteInitData);
	}

	void ShadowMapRender::Render(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap);
		g_engine->ExecuteRender();
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	}

	void ShadowMapRender::ShadowMapSpriteRender(RenderContext& rc)
	{
		sprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f, 0.0f }
		, g_quatIdentity, g_vec3One, { 0.0f,0.0f });
		sprite.Draw(rc);
	}
}