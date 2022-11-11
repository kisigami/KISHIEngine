#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"


namespace  nsK2EngineLow {

	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis)
	{
		//���f���̏������f�[�^���쐬
		ModelInitData initData;
		//tkm�t�@�C�����w��
		initData.m_tkmFilePath = filePath;
		//fx�t�@�C�����w��
		initData.m_fxFilePath = "Assets/shader/model.fx";
		//�萔�o�b�t�@���w��
		initData.m_expandConstantBuffer = &g_renderingEngine.GetDeferredLightingCB();
		//�萔�o�b�t�@�̃T�C�Y���w��
		initData.m_expandConstantBufferSize = sizeof(g_renderingEngine.GetDeferredLightingCB());
		//�J���[�o�b�t�@�̃t�H�[�}�b�g���w��
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//�A�j���[�V�����N���b�v����������
		if (animationClips == nullptr)
		{
			//���_�V�F�[�_�[�̃G���g���[�|�C���g���w��
			initData.m_vsEntryPointFunc = "VSMain";
		}
		else
		{
			//�X�P���g���̎w��
			initData.m_skeleton = &m_skeleton;
			//�X�L������}�e���A���p�̒��_�V�F�[�_�[���w��
			initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�X�P���g���̏�����
			InitSkeleton(filePath);
			//�A�j���[�V�����̏�����
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		}
		//���f���̏�������w��
		initData.m_modelUpAxis = enModelUpAxis;
		//���f���������f�[�^�Ń��f����������
		m_model.Init(initData);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init
			(m_skeleton,
				m_animationClips,
				m_numAnimationClips);
		}
	}

	void ModelRender::Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		if(m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
	
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}
