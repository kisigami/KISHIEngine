#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;

	//������
	void RenderingEngine::Init()
	{
		//ZPrepass�̏�����
		InitZPrepassTargetRender();

		//MainRenderTarget�̏�����
		InitMainRenderTarget();

		//G-Buffer�̏�����
		InitGBuffer();

		//���C�������_�����O�^�[�Q�b�g��
		//�X�i�b�v�V���b�g���Ƃ邽�߂�
		//�����_�����O�^�[�Q�b�g�̏�����
		InitMainRTSnapshotRenderTarget();

		//���C�������_�����O�^�[�Q�b�g��
		//�J���[�o�b�t�@�̓��e��
		//�t���[���o�b�t�@�ɃR�s�[���邽�߂�
		//�X�v���C�g�̏�����
		InitCopyMainRenderTargetToFrameBufferSprite();

		//�f�B�t�@�[�h���C�e�B���O�̏��������Ђ������͂��ւ�����������������
		InitDeferredLighting();

		m_postEffect.Init(m_mainRenderTarget);

		
	}

	void RenderingEngine::InitZPrepassTargetRender()
	{
		m_zprepassRenderTarget.Create
		(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
			);
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1, 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		//�A���x�h�J���[���o�͗p�̃����_�����O�^�[�Q�b�g�̏�����
		m_gBuffer[enGBufferAlbedo].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�@���o�͗p�̃����_�����O�^�[�Q�b�g�̏�����
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1, 
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//�����x�Ɗ��炩���}�b�v�o�͗p�̃����_�����O�^�[�Q�b�g�̏�����
		m_gBuffer[enGBufferMetalSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//���[���h���W�n�o�͗p�̃����_�����O�^�[�Q�b�g��������
		m_gBuffer[enGBufferWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitMainRTSnapshotRenderTarget()
	{
		for (auto& snapshotRt : m_mainRTSnapshots)
		{
			snapshotRt.Create(
				g_graphicsEngine->GetFrameBufferWidth(),
				g_graphicsEngine->GetFrameBufferHeight(),
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_FORMAT_UNKNOWN
			);
		}
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitDeferredLighting()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		/*m_deferredLightingCB.m_light.directionlight.ligDirection.x = 0.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.y = -1.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.z = -1.0f;
		m_deferredLightingCB.m_light.directionlight.ligDirection.Normalize();

		m_deferredLightingCB.m_light.directionlight.ligColor.x = 5.0f;
		m_deferredLightingCB.m_light.directionlight.ligColor.y = 5.0f;
		m_deferredLightingCB.m_light.directionlight.ligColor.z = 5.0f;
		
		m_deferredLightingCB.m_light.eyePos = Vector3(0.0f, 150.0f, 200.0f);
		m_deferredLightingCB.m_light.ambientLight.x = 0.5f;
		m_deferredLightingCB.m_light.ambientLight.y = 0.5f;
		m_deferredLightingCB.m_light.ambientLight.z = 0.5f;*/

		m_sceneLight.Init();

		SpriteInitData spriteInitData;

		spriteInitData.m_width = frameBuffer_w;
		spriteInitData.m_height = frameBuffer_h;

		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			spriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}

		spriteInitData.m_fxFilePath = "Assets/shader/DeferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);

		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		// �������f�[�^���g���ăX�v���C�g���쐬
		m_diferredLightingSprite.Init(spriteInitData);
	}

	//���s
	void RenderingEngine::Excute(RenderContext& rc)
	{
		//ZPprepass
		ZPrepass(rc);

		//GBuffer�����_�����O
		RenderToGBuffer(rc);

		DeferredLighting(rc);

		SnapshotMainRenderTarget(rc, EnMainRTSnapshot::enDrawnOpacity);
	
		ForwardRendering(rc);
	
		m_postEffect.Render(rc, m_mainRenderTarget);

		CopyMainRenderTargetToFrameBuffer(rc);

		m_renderToGBufferModels.clear();
		m_forwardRenderModels.clear();
		m_zprepassModels.clear();

	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& model : m_zprepassModels)
		{
			model->Draw(rc);
		}
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		//�����_�����O�^�[�Q�b�g��GBuffer�ɕύX
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedo],
			&m_gBuffer[enGBufferNormal],
			&m_gBuffer[enGBufferWorldPos],
			&m_gBuffer[enGBufferMetalSmooth],
		};

		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& model : m_renderToGBufferModels)
		{
			model->Draw(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void  RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		m_deferredLightingCB.m_light.eyePos = Vector3(0.0f,150.0f,200.0f);
		//�f�B�t�@�[�h���C�e�B���O�ɕK�v�ȃ��C�g���X�V
		{
			
		}
		
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_diferredLightingSprite.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot)
	{
		// ���C�������_�����O�^�[�Q�b�g�̓��e���X�i�b�v�V���b�g
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);
		rc.SetRenderTargetAndViewport(m_mainRTSnapshots[(int)enSnapshot]);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRTSnapshots[(int)enSnapshot]);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedo].GetDSVCpuDescriptorHandle()
		);
		for (auto& model : m_forwardRenderModels)
		{
			model->Draw(rc);
		}

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 1600;
		viewport.Height = 900;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}
};