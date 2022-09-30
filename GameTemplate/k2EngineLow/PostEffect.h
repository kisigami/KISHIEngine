#pragma once
#include "Bloom.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// ポストエフェクトクラス
	/// </summary>
	class PostEffect
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget">レンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">レンダリングターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		Bloom    m_bloom;  //ブルーム
	};
};