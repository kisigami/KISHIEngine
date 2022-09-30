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
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// ���s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Excute(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��ݒ�
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void SetMainRenderTarget(RenderContext& rc);

	private:
		RenderTarget m_mainRenderTarget;  //���C�������_�����O�^�[�Q�b�g
		PostEffect m_postEffect;          //�|�X�g�G�t�F�N�g
	};

	//�O���[�o��
	extern RenderingEngine g_renderingEngine;
};