#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	void RenderingEngine::Init()
	{
		//メインレンダリングターゲットの初期化
		InitMainRenderTarget();
		//ポストエフェクトの初期化
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::Excute(RenderContext& rc)
	{
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