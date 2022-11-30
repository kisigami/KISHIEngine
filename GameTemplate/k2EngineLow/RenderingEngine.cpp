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
		Init2DRenderTarget();
		//�|�X�g�G�t�F�N�g�̏�����
	    m_postEffect.Init(m_mainRenderTarget);
	    
	}

	void RenderingEngine::Excute(RenderContext& rc)
	{
		g_sceneLight.Update();
		m_deferredLightingCB.m_light = g_sceneLight.GetSceneLight();

		//�|�X�g�G�t�F�N�g�̕`��
		m_postEffect.Render(rc,m_mainRenderTarget);

		//2D�̕`��
		Render2D(rc);

		m_renderObjects.clear();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		//2D�`��I�u�W�F�N�g�`�悷��
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRender2D(rc);
		}
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

	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//�㏑���B
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//�e�N�X�`���̓��C�������_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		//�𑜓x��2D�����_�\�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		// �e�N�X�`����yBlurRenderTarget�̃J���[�o�b�t�@�[
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// �����_�����O�悪�t���[���o�b�t�@�[�Ȃ̂ŁA�𑜓x�̓t���[���o�b�t�@�[�Ɠ���
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		// �K���}�␳�����2D�`��̃V�F�[�_�[���w�肷��
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �������I�u�W�F�N�g���g���āA�X�v���C�g������������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);

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