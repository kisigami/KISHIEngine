#pragma once
#include "SceneLight.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	class RenderingEngine
	{
	public:
		//スナップショット？？？？
		enum class EnMainRTSnapshot
		{
			enDrawnOpacity,     // 不透明オブジェクトの描画完了時点
			enNum,              // スナップショットの数
		};

		void Init();
		void Excute(RenderContext& rc);

	private:
		void InitZPrepassTargetRender();
		void InitMainRenderTarget();
		void InitGBuffer();
		void InitMainRTSnapshotRenderTarget();
		void InitCopyMainRenderTargetToFrameBufferSprite();
		void InitDeferredLighting();
		
		void ZPrepass(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot);
		void ForwardRendering(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		
		//GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedo,        // アルベド
			enGBufferNormal,        // 法線
			enGBufferWorldPos,      // ワールド座標
			enGBufferMetalSmooth,   // 金属度と滑らかさ。xに金属度、wに滑らかさが記録されている。
			enGBufferNum,           // G-Bufferの数
		};

		// ディファードライティング用の定数バッファ
		struct SDeferredLightingCB
		{
			Light m_light;      // ライト
			float pad;          // パディング
			Matrix mlvp;
		};

		SDeferredLightingCB   m_deferredLightingCB;
		Sprite                m_copyMainRtToFrameBufferSprite;
		Sprite                m_diferredLightingSprite;            // ディファードライティングを行うためのスプライト
		RenderTarget          m_zprepassRenderTarget;
		RenderTarget          m_mainRenderTarget;
		RenderTarget          m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];
		RenderTarget          m_gBuffer[enGBufferNum];

		std::vector< Model* > m_zprepassModels;                         // ZPrepassの描画パスで描画されるモデルのリスト
		std::vector< Model* > m_renderToGBufferModels;                  // Gバッファへの描画パスで描画するモデルのリスト
		std::vector< Model* > m_forwardRenderModels; 
		PostEffect            m_postEffect;      // フォワードレンダリングの描画パスで描画されるモデルのリスト
		SceneLight            m_sceneLight;
	};

	extern RenderingEngine g_renderingEngine;
};