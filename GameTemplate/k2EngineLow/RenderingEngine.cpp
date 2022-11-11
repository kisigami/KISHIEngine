#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	void RenderingEngine::Init()
	{
		m_deferredLightingCB.m_light = g_sceneLight.GetSceneLight();
		//���C�������_�����O�^�[�Q�b�g�̏�����
		InitMainRenderTarget();
		//�|�X�g�G�t�F�N�g�̏�����
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::Excute(RenderContext& rc)
	{
		g_sceneLight.Update();
		m_deferredLightingCB.m_light = g_sceneLight.GetSceneLight();

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

	void RenderingEngine::InitDeferrdLighting()
	{
		//�X�v���C�g�̏������f�[�^���쐬
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_albedRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_textures[1] = &m_normalRenderTarget.GetRenderTargetTexture();
		//�V�F�[�_�[���w��
		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		//�X�v���C�g�̕���ݒ�
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//�X�v���C�g�̍�����ݒ�
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);
		//�������f�[�^�ŃX�v���C�g��������
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		m_deferredLightingCB.m_light.eyePos = g_camera3D->GetPosition();
		//m_deferredLightingCB.m_light.mViewProjInv.Inverse(g_camera3D->GetViewProjectionMatrix());

		RenderTarget* rts[] = {
			&m_albedRenderTarget,
			&m_normalRenderTarget
		};
		rc.WaitUntilFinishDrawingToRenderTargets(2, rts);
		rc.SetRenderTargets(2, rts);
		rc.ClearRenderTargetViews(2, rts);
		rc.WaitUntilFinishDrawingToRenderTargets(2, rts);



		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
		m_diferredLightingSprite.Draw(rc);
	}

	void RenderingEngine::InitGBuffer()
	{
		m_albedRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //�t���[���o�b�t�@�̕�
			g_graphicsEngine->GetFrameBufferHeight(), //�t���[���o�b�t�@�̍���
			1,                                        //�~�b�v�}�b�v���x��
			1,                                        //�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);

		m_normalRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //�t���[���o�b�t�@�̕�
			g_graphicsEngine->GetFrameBufferHeight(), //�t���[���o�b�t�@�̍���
			1,                                        //�~�b�v�}�b�v���x��
			1,                                        //�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
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