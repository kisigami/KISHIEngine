#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	//初期化
	void RenderingEngine::Init()
	{
		//ZPrepassの初期化
		InitZPrepassTargetRender();

		//MainRenderTargetの初期化
		InitMainRenderTarget();

		//G-Bufferの初期化
		InitGBuffer();

		//メインレンダリングターゲットの
		//スナップショットをとるための
		//レンダリングターゲットの初期化
		InitMainRTSnapshotRenderTarget();

		//メインレンダリングターゲットの
		//カラーバッファの内容を
		//フレームバッファにコピーするための
		//スプライトの初期化
		InitCopyMainRenderTargetToFrameBufferSprite();

		//ディファードライティングの初期化ｆひおあえはｇへいおｇｊｄｋｍｋｍ
		InitDeferredLighting();

		m_postEffect.Init(m_mainRenderTarget);

		
	}

	void RenderingEngine::InitZPrepassTargetRender()
	{
		m_zprepassRenderTarget.Create
		(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
			);
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1, 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		//アルベドカラーを出力用のレンダリングターゲットの初期化
		m_gBuffer[enGBufferAlbedo].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//法線出力用のレンダリングターゲットの初期化
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1, 
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//金属度と滑らかさマップ出力用のレンダリングターゲットの初期化
		m_gBuffer[enGBufferMetalSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//ワールド座標系出力用のレンダリングターゲットを初期化
		m_gBuffer[enGBufferWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitMainRTSnapshotRenderTarget()
	{
		for (auto& snapshotRt : m_mainRTSnapshots)
		{
			snapshotRt.Create(
				g_graphicsEngine->GetFrameBufferWidth(),
				g_graphicsEngine->GetFrameBufferHeight(),
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_FORMAT_UNKNOWN
			);
		}
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitDeferredLighting()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		/*m_deferredLightingCB.m_light.directionlight.ligDirection.x = 0.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.y = -1.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.z = -1.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.Normalize();

		m_deferredLightingCB.m_light.directionlight.ligColor.x = 5.0f;
		m_deferredLightingCB.m_light.directionlight.ligColor.y = 5.0f;
		m_deferredLightingCB.m_light.directionlight.ligColor.z = 5.0f;
		
		m_deferredLightingCB.m_light.eyePos = Vector3(0.0f, 150.0f, 200.0f);
		m_deferredLightingCB.m_light.ambientLight.x = 0.5f;
		m_deferredLightingCB.m_light.ambientLight.y = 0.5f;
		m_deferredLightingCB.m_light.ambientLight.z = 0.5f;*/

		m_sceneLight.Init();

		SpriteInitData spriteInitData;

		spriteInitData.m_width = frameBuffer_w;
		spriteInitData.m_height = frameBuffer_h;

		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			spriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}

		spriteInitData.m_fxFilePath = "Assets/shader/DeferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);

		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		// 初期化データを使ってスプライトを作成
		m_diferredLightingSprite.Init(spriteInitData);
	}

	//実行
	void RenderingEngine::Excute(RenderContext& rc)
	{
		//ZPprepass
		ZPrepass(rc);

		//GBufferレンダリング
		RenderToGBuffer(rc);

		DeferredLighting(rc);

		SnapshotMainRenderTarget(rc, EnMainRTSnapshot::enDrawnOpacity);
	
		ForwardRendering(rc);
	
		m_postEffect.Render(rc, m_mainRenderTarget);

		CopyMainRenderTargetToFrameBuffer(rc);

		m_renderToGBufferModels.clear();
		m_forwardRenderModels.clear();
		m_zprepassModels.clear();

	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& model : m_zprepassModels)
		{
			model->Draw(rc);
		}
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		//レンダリングターゲットをGBufferに変更
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedo],
			&m_gBuffer[enGBufferNormal],
			&m_gBuffer[enGBufferWorldPos],
			&m_gBuffer[enGBufferMetalSmooth],
		};

		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& model : m_renderToGBufferModels)
		{
			model->Draw(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void  RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		m_deferredLightingCB.m_light.eyePos = Vector3(0.0f,150.0f,200.0f);
		//ディファードライティングに必要なライトを更新
		{
			
		}
		
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_diferredLightingSprite.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot)
	{
		// メインレンダリングターゲットの内容をスナップショット
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);
		rc.SetRenderTargetAndViewport(m_mainRTSnapshots[(int)enSnapshot]);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedo].GetDSVCpuDescriptorHandle()
		);
		for (auto& model : m_forwardRenderModels)
		{
			model->Draw(rc);
		}

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		// メインレンダリングターゲットの絵をフレームバッファーにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// ビューポートを指定する
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 1600;
		viewport.Height = 900;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}
};