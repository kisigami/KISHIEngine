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
		ModelInitData initData;

		initData.m_tkmFilePath = filePath;
		initData.m_fxFilePath = "Assets/shader/model.fx";

		initData.m_expandConstantBuffer = &g_directionLight;
		initData.m_expandConstantBufferSize = sizeof(g_directionLight);

		if (animationClips == nullptr)
		{
			initData.m_vsEntryPointFunc = "VSMain";
		}
		else
		{
			initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			InitSkeleton(filePath);
			initData.m_skeleton = &m_skeleton;
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		}

		initData.m_modelUpAxis = enModelUpAxis;
		m_model.Init(initData);
		Update();
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
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
	
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}
