#pragma once
#include "PostEffect.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// レンダリングエンジンクラス
	/// </summary>
	class RenderingEngine
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Excute(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットを設定
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void SetMainRenderTarget(RenderContext& rc);

	private:
		RenderTarget m_mainRenderTarget;  //メインレンダリングターゲット
		PostEffect m_postEffect;          //ポストエフェクト
	};

	//グローバル
	extern RenderingEngine g_renderingEngine;
};