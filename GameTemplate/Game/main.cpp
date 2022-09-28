#include "stdafx.h"
#include "system/system.h"
#include "bloom.h"
#include "Game.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;


/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);

	//メインレンダリングターゲット作成
	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1600,
		900,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//強いライト
	g_sceneLight.Init();

	auto game = NewGO<Game>(0, "game");

	g_bloom.Init(mainRenderTarget);

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		//レンダリングターゲットとして使用可能になるまで待つ
		renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//レンダリングターゲットを設定
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//レンダリングターゲットをクリア
		renderContext.ClearRenderTargetView(mainRenderTarget);

		//更新
		g_k2EngineLow->ExecuteUpdate();

		//描画
		g_engine->ExecuteRender();
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		//ブルームいろいろ
		g_bloom.Render(renderContext,mainRenderTarget);

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
