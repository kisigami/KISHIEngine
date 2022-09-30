#include "stdafx.h"
#include "system/system.h"
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

	//ライト初期化
	g_sceneLight.Init();
	//レンダリングエンジンの初期化
	g_renderingEngine.Init();

	//ゲーム作成
	auto game = NewGO<Game>(0, "game");

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		//レンダリングターゲットの設定
		g_renderingEngine.SetMainRenderTarget(renderContext);

		//更新処理
		g_k2EngineLow->ExecuteUpdate();

		//描画処理
		g_k2EngineLow->ExecuteRender();

		//レンダリングエンジンを実行
		g_renderingEngine.Excute(renderContext);

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
