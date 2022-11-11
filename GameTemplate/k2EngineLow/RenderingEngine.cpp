#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	void RenderingEngine::Init()
	{
		m_deferredLightingCB.m_light = g_sceneLight.GetSceneLight();
		//メインレンダリングターゲットの初期化
		InitMainRenderTarget();
		//ポストエフェクトの初期化
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::Excute(RenderContext& rc)
	{
		g_sceneLight.Update();
		m_deferredLightingCB.m_light = g_sceneLight.GetSceneLight();

		//ポストエフェクトの描画
		m_postEffect.Render(rc,m_mainRenderTarget);
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲットを作成
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //フレームバッファの幅
			g_graphicsEngine->GetFrameBufferHeight(), //フレームバッファの高さ
			1,                                        //ミップマップレベル
			1,                                        //テクスチャ配列のサイズ
			DXGI_FORMAT_R32G32B32A32_FLOAT,           //カラーバッファのフォーマット
			DXGI_FORMAT_D32_FLOAT                     //深度ステンシルバッファのフォーマット
		);
	}

	void RenderingEngine::InitDeferrdLighting()
	{
		//スプライトの初期化データを作成
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_albedRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_textures[1] = &m_normalRenderTarget.GetRenderTargetTexture();
		//シェーダーを指定
		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		//スプライトの幅を設定
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//スプライトの高さを設定
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);
		//初期化データでスプライトを初期化
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		m_deferredLightingCB.m_light.eyePos = g_camera3D->GetPosition();
		//m_deferredLightingCB.m_light.mViewProjInv.Inverse(g_camera3D->GetViewProjectionMatrix());

		RenderTarget* rts[] = {
			&m_albedRenderTarget,
			&m_normalRenderTarget
		};
		rc.WaitUntilFinishDrawingToRenderTargets(2, rts);
		rc.SetRenderTargets(2, rts);
		rc.ClearRenderTargetViews(2, rts);
		rc.WaitUntilFinishDrawingToRenderTargets(2, rts);



		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
		m_diferredLightingSprite.Draw(rc);
	}

	void RenderingEngine::InitGBuffer()
	{
		m_albedRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //フレームバッファの幅
			g_graphicsEngine->GetFrameBufferHeight(), //フレームバッファの高さ
			1,                                        //ミップマップレベル
			1,                                        //テクスチャ配列のサイズ
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);

		m_normalRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //フレームバッファの幅
			g_graphicsEngine->GetFrameBufferHeight(), //フレームバッファの高さ
			1,                                        //ミップマップレベル
			1,                                        //テクスチャ配列のサイズ
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::SetMainRenderTarget(RenderContext& rc)
	{
		//レンダリングターゲットとして使用可能になるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_mainRenderTarget);
	}
};