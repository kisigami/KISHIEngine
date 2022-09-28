#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	Bloom g_bloom;

	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		//輝度抽出用のレンダリングターゲット作成
		InitLuminanceRenderTarget();

		//輝度抽出用のスプライト初期化
		InitLuminanceSprite(mainRenderTarget);

		//ガウシアンブラー初期化
		InitGaussianBlur();

		//ボケ画像を加算するスプライト初期化
		InitFinalSprite();

		//テクスチャを張り付けるスプライト初期化
		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;

		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//輝度抽出
		TakeLuminanceSprite(rc);

		//ガウシアンブラー実行
		ExcuteGaussianBlur(rc);

		//ボケ画像をメインレンダリングターゲットに加算
		PulsBokeSprite(rc, mainRenderTarget);

		//コピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToFrameBufferSprite.Draw(rc);
	}

	void Bloom::InitLight()
	{
		Light light;

		light.directionlight.ligDirection.x = 0.0f;
		light.directionlight.ligDirection.y = 0.0f;
		light.directionlight.ligDirection.z = -1.0f;

		light.directionlight.ligDirection.Normalize();

		light.directionlight.ligColor.x = 55.8f;
		light.directionlight.ligColor.y = 55.8f;
		light.directionlight.ligColor.z = 55.8f;

		light.eyePos = g_camera3D->GetPosition();

		light.ambientLight.x = 0.5f;
		light.ambientLight.y = 0.5f;
		light.ambientLight.z = 0.5f;


		g_sceneLight.SetLigDirection(light.directionlight.ligDirection);
		g_sceneLight.SetLigColor(light.directionlight.ligColor);
		g_sceneLight.SetAmbientLihgt(light.ambientLight);
		g_sceneLight.SetEyePos(light.eyePos);
	}

	void Bloom::InitLuminanceRenderTarget()
	{
		m_luminanceRenderTarget.Create(
			1600,
			900, 
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
	{
		SpriteInitData luminanceSpriteInitData;
		
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";

		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

		luminanceSpriteInitData.m_width = 1600;
		luminanceSpriteInitData.m_height = 900;

		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitGaussianBlur()
	{
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}

	void Bloom::InitFinalSprite()
	{
		SpriteInitData finalSpriteInitData;

		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
	
		finalSpriteInitData.m_width = 1600;
		finalSpriteInitData.m_height = 900;

		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::TakeLuminanceSprite(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		m_luminanceSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);
	}

	void Bloom::ExcuteGaussianBlur(RenderContext& rc)
	{
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);
	}

	void Bloom::PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		rc.SetRenderTargetAndViewport(mainRenderTarget);
	
		m_finalSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
};
