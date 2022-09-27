#include "k2EngineLowPreCompile.h"
#include "Monochrome.h"

namespace nsK2EngineLow
{
	Monochrome g_monochrome;

	void Monochrome::Init()
	{
		InitOffscreenRenderTarget();
		InitPostEffectSprite();
	}

	void Monochrome::Draw(RenderContext& rc)
	{
		ChangeRenderTarget(rc);
		ChangeOnscreenRenderTarget(rc);
		DrawFullscreenSprite(rc);
	}

	void Monochrome::InitOffscreenRenderTarget()
	{
		//レンダリングターゲットを作成
		m_offscreenRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void Monochrome::InitPostEffectSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_offscreenRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;

		spriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";

		m_monochromeSprite.Init(spriteInitData);
	}

	void Monochrome::ChangeRenderTarget(RenderContext& rc)
	{
		RenderTarget* rtArray[] = { &m_offscreenRenderTarget };
		rc.WaitUntilToPossibleSetRenderTargets(1, rtArray);
		rc.SetRenderTargets(1, rtArray);
		rc.ClearRenderTargetViews(1, rtArray);

		g_engine->ExecuteRender();

		rc.WaitUntilFinishDrawingToRenderTargets(1, rtArray);
	}

	void Monochrome::ChangeOnscreenRenderTarget(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
	}

	void Monochrome::DrawFullscreenSprite(RenderContext& rc)
	{
		m_monochromeSprite.Draw(rc);
	}
};