#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g������
		InitLuminanceRenderTarget();
		//�P�x���o�p�̃X�v���C�g������
		InitLuminanceSprite(mainRenderTarget);
		//�K�E�V�A���u���[������
		InitGaussianBlur();
		//�{�P�摜���Z�����X�v���C�g������
		InitPlusBokeSprite();
		//���C�������_�����O�^�[�Q�b�g�ɕ`�悳�ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������
		InitCopyToFrameBufferSprite(mainRenderTarget);
	}

	void Bloom::InitLuminanceRenderTarget()
	{
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�쐬
		m_luminanceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),  //�t���[���o�b�t�@�̕�
			g_graphicsEngine->GetFrameBufferHeight(), //�t���[���o�b�t�@�̍���
			1,                                        //�~�b�v�}�b�v���x��
			1,                                        //�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R32G32B32A32_FLOAT,           //�J���[�o�b�t�@�̃t�H�[�}�b�g
			DXGI_FORMAT_D32_FLOAT                     //�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		);
	}

	void Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
	{
		//�X�v���C�g�̏������f�[�^���쐬
		SpriteInitData luminanceSpriteInitData;
		//�P�x���o�p�̃V�F�[�_�[���w�肷��
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		//���_�V�F�[�_�[�̃G���g���[�|�C���g���w��
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w��
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//�X�v���C�g�̕���ݒ�
		luminanceSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//�X�v���C�g�̍�����ݒ�
		luminanceSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//�e�N�X�`���Ƀ��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@���w��
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w��
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//�������f�[�^�ŃX�v���C�g��������
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitGaussianBlur()
	{
		//�K�E�V�A���u���[��������
		//m_gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		//m_gaussianBlur[1]��m_gaussianBlur[0]�ɃK�E�V�A���u���[��������
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		//m_gaussianBlur[2]��m_gaussianBlur[1]�ɃK�E�V�A���u���[��������
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		//m_gaussianBlur[3]��m_gaussianBlur[2]�ɃK�E�V�A���u���[��������
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}

	void Bloom::InitPlusBokeSprite()
	{
		//�X�v���C�g�̏������f�[�^���쐬
		SpriteInitData finalSpriteInitData;
		//�{�P�摜��4���w��
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
		//�V�F�[�_�[���w��
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
		//���Z�����ŕ`�悷��̂ŃA���t�@�u�����f�B���O���[�h�����Z�ɐݒ�
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//�X�v���C�g�̕���ݒ�
		finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//�X�v���C�g�̍�����ݒ�
		finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w��
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//�������f�[�^�ŃX�v���C�g��������
		m_finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::InitCopyToFrameBufferSprite(RenderTarget& mainRenderTarget)
	{
		//�X�v���C�g�̏������f�[�^���쐬
		SpriteInitData spriteInitData;
		//�e�N�X�`���Ƀ��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@���w��
		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//�X�v���C�g�̕���ݒ�
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		//�X�v���C�g�̍�����ݒ�
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//2D�p�̃V�F�[�_�[���w��
		spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
		//�������f�[�^�ŃX�v���C�g��������
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�P�x���o
		//TakeLuminanceSprite(rc);
		//�K�E�V�A���u���[���s
		//ExcuteGaussianBlur(rc);
		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z
		//PulsBokeSprite(rc, mainRenderTarget);
		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyToFrameBufferSprite(rc);
	}

	void Bloom::TakeLuminanceSprite(RenderContext& rc)
	{
	    //�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		//�P�x���o
		m_luminanceSprite.Draw(rc);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);
	}

	void Bloom::ExcuteGaussianBlur(RenderContext& rc)
	{
		//�K�E�V�A���u���[���S����s
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);
	}

	void Bloom::PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		//�ŏI����
		m_finalSprite.Draw(rc);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}

	void Bloom::CopyToFrameBufferSprite(RenderContext& rc)
	{
		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);
	}
};
