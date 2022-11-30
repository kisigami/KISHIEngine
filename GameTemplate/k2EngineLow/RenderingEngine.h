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
		void Render2D(RenderContext& rc);

		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		struct SDeferredLightingCB
		{
			Light m_light;              // ライト
		};

		SDeferredLightingCB& GetDeferredLightingCB()
		{
			return m_deferredLightingCB;
		}

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Excute(RenderContext& rc);
		void DeferredLighting(RenderContext&  rc);
		/// <summary>
		/// メインレンダリングターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();
		void InitGBuffer();
		void InitDeferrdLighting();
		/// <summary>
		/// メインレンダリングターゲットを設定
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void SetMainRenderTarget(RenderContext& rc);
		void Init2DRenderTarget();

		void InitCopyMainRenderTargetToFrameBufferSprite();
	
	private:
		Sprite m_copyMainRtToFrameBufferSprite;
		Sprite m_mainSprite;
		Sprite m_2DSprite;
		SDeferredLightingCB m_deferredLightingCB;
		RenderTarget m_mainRenderTarget;  //メインレンダリングターゲット
		RenderTarget m_albedRenderTarget;
		RenderTarget m_normalRenderTarget;
		RenderTarget m_2DRenderTarget;
		Sprite m_diferredLightingSprite;
		PostEffect m_postEffect;          //ポストエフェクト
		std::vector< IRenderer* > m_renderObjects;                      // 描画オブジェクトのリスト。
	};

	//グローバル
	extern RenderingEngine g_renderingEngine;
};