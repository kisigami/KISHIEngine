#pragma once
#include "PostEffect.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// �����_�����O�G���W���N���X
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
			Light m_light;              // ���C�g
		};

		SDeferredLightingCB& GetDeferredLightingCB()
		{
			return m_deferredLightingCB;
		}

		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// ���s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Excute(RenderContext& rc);
		void DeferredLighting(RenderContext&  rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitMainRenderTarget();
		void InitGBuffer();
		void InitDeferrdLighting();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��ݒ�
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void SetMainRenderTarget(RenderContext& rc);
		void Init2DRenderTarget();

		void InitCopyMainRenderTargetToFrameBufferSprite();
	
	private:
		Sprite m_copyMainRtToFrameBufferSprite;
		Sprite m_mainSprite;
		Sprite m_2DSprite;
		SDeferredLightingCB m_deferredLightingCB;
		RenderTarget m_mainRenderTarget;  //���C�������_�����O�^�[�Q�b�g
		RenderTarget m_albedRenderTarget;
		RenderTarget m_normalRenderTarget;
		RenderTarget m_2DRenderTarget;
		Sprite m_diferredLightingSprite;
		PostEffect m_postEffect;          //�|�X�g�G�t�F�N�g
		std::vector< IRenderer* > m_renderObjects;                      // �`��I�u�W�F�N�g�̃��X�g�B
	};

	//�O���[�o��
	extern RenderingEngine g_renderingEngine;
};