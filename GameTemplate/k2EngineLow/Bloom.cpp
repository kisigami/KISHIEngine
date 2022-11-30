#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		//輝度抽出用のレンダリングターゲット初期化
		InitLuminanceRenderTarget();
		//輝度抽出用のスプライト初期化
		InitLuminanceSprite(mainRenderTarget);
		//ガウシアンブラー初期化
		InitGaussianBlur();
		//ボケ画像加算合成スプライト初期化
		InitPlusBokeSprite();
		//メインレンダリングターゲットに描画された絵をフレームバッファにコピーするためのスプライト初期化
		InitCopyToFrameBufferSprite(mainRenderTarget);
	}

	void Bloom::InitLuminanceRenderTarget()
	{
		//輝度抽出用のレンダリングターゲット作成
		m_luminanceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //フレームバッファの幅
			g_graphicsEngine->GetFrameBufferHeight(), //フレームバッファの高さ
			1,                                        //ミップマップレベル
			1,                                        //テクスチャ配列のサイズ
			DXGI_FORMAT_R32G32B32A32_FLOAT,           //カラーバッファのフォーマット
			DXGI_FORMAT_D32_FLOAT                     //深度ステンシルバッファのフォーマット
		);
	}

	void Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
	{
		//スプライトの初期化データを作成
		SpriteInitData luminanceSpriteInitData;
		//輝度抽出用のシェーダーを指定する
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		//頂点シェーダーのエントリーポイントを指定
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//ピクセルシェーダーのエントリーポイントを指定
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//スプライトの幅を設定
		luminanceSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//スプライトの高さを設定
		luminanceSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//テクスチャにメインレンダリングターゲットのカラーバッファを指定
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//描き込むレンダリングターゲットのフォーマットを指定
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//初期化データでスプライトを初期化
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitGaussianBlur()
	{
		//ガウシアンブラーを初期化
		//m_gaussianBlur[0]は輝度テクスチャにガウシアンブラーをかける
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		//m_gaussianBlur[1]はm_gaussianBlur[0]にガウシアンブラーをかける
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		//m_gaussianBlur[2]はm_gaussianBlur[1]にガウシアンブラーをかける
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		//m_gaussianBlur[3]はm_gaussianBlur[2]にガウシアンブラーをかける
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}

	void Bloom::InitPlusBokeSprite()
	{
		//スプライトの初期化データを作成
		SpriteInitData finalSpriteInitData;
		//ボケ画像を4枚指定
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
		//シェーダーを指定
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		//ピクセルシェーダーのエントリーポイントを指定する
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
		//加算合成で描画するのでアルファブレンディングモードを加算に設定
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//スプライトの幅を設定
		finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//スプライトの高さを設定
		finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//描き込むレンダリングターゲットのフォーマットを指定
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//初期化データでスプライトを初期化
		m_finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::InitCopyToFrameBufferSprite(RenderTarget& mainRenderTarget)
	{
		//スプライトの初期化データを作成
		SpriteInitData spriteInitData;
		//テクスチャにメインレンダリングターゲットのカラーバッファを指定
		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//スプライトの幅を設定
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//スプライトの高さを設定
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//2D用のシェーダーを指定
		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		//初期化データでスプライトを初期化
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//輝度抽出
		//TakeLuminanceSprite(rc);
		//ガウシアンブラー実行
		//ExcuteGaussianBlur(rc);
		//ボケ画像をメインレンダリングターゲットに加算
		//PulsBokeSprite(rc, mainRenderTarget);
		//メインレンダリングターゲットの絵をフレームバッファにコピー
		CopyToFrameBufferSprite(rc);
	}

	void Bloom::TakeLuminanceSprite(RenderContext& rc)
	{
	    //輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		//輝度抽出
		m_luminanceSprite.Draw(rc);
		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);
	}

	void Bloom::ExcuteGaussianBlur(RenderContext& rc)
	{
		//ガウシアンブラーを４回実行
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);
	}

	void Bloom::PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		//最終合成
		m_finalSprite.Draw(rc);
		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}

	void Bloom::CopyToFrameBufferSprite(RenderContext& rc)
	{
		//メインレンダリングターゲットの絵をフレームバッファにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);
	}
};
