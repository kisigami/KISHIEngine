#pragma once
#include "SceneLight.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	class RenderingEngine
	{
	public:
		//�X�i�b�v�V���b�g�H�H�H�H
		enum class EnMainRTSnapshot
		{
			enDrawnOpacity,     // �s�����I�u�W�F�N�g�̕`�抮�����_
			enNum,              // �X�i�b�v�V���b�g�̐�
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
		
		//GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedo,        // �A���x�h
			enGBufferNormal,        // �@��
			enGBufferWorldPos,      // ���[���h���W
			enGBufferMetalSmooth,   // �����x�Ɗ��炩���Bx�ɋ����x�Aw�Ɋ��炩�����L�^����Ă���B
			enGBufferNum,           // G-Buffer�̐�
		};

		// �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
		struct SDeferredLightingCB
		{
			Light m_light;      // ���C�g
			float pad;          // �p�f�B���O
			Matrix mlvp;
		};

		SDeferredLightingCB   m_deferredLightingCB;
		Sprite                m_copyMainRtToFrameBufferSprite;
		Sprite                m_diferredLightingSprite;            // �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g
		RenderTarget          m_zprepassRenderTarget;
		RenderTarget          m_mainRenderTarget;
		RenderTarget          m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];
		RenderTarget          m_gBuffer[enGBufferNum];

		std::vector< Model* > m_zprepassModels;                         // ZPrepass�̕`��p�X�ŕ`�悳��郂�f���̃��X�g
		std::vector< Model* > m_renderToGBufferModels;                  // G�o�b�t�@�ւ̕`��p�X�ŕ`�悷�郂�f���̃��X�g
		std::vector< Model* > m_forwardRenderModels; 
		PostEffect            m_postEffect;      // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f���̃��X�g
		SceneLight            m_sceneLight;
	};

	extern RenderingEngine g_renderingEngine;
};