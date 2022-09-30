#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	void RenderingEngine::Init()
	{
		//���C�������_�����O�^�[�Q�b�g�̏�����
		InitMainRenderTarget();
		//�|�X�g�G�t�F�N�g�̏�����
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::Excute(RenderContext& rc)
	{
		//�|�X�g�G�t�F�N�g�̕`��
		m_postEffect.Render(rc,m_mainRenderTarget);
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//���C�������_�����O�^�[�Q�b�g���쐬
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //�t���[���o�b�t�@�̕�
			g_graphicsEngine->GetFrameBufferHeight(), //�t���[���o�b�t�@�̍���
			1,                                        //�~�b�v�}�b�v���x��
			1,                                        //�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R32G32B32A32_FLOAT,           //�J���[�o�b�t�@�̃t�H�[�}�b�g
			DXGI_FORMAT_D32_FLOAT                     //�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		);
	}

	void RenderingEngine::SetMainRenderTarget(RenderContext& rc)
	{
		//�����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_mainRenderTarget);
	}
};