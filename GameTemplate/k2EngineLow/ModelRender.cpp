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
		//モデルの初期化データを作成
		ModelInitData initData;
		//tkmファイルを指定
		initData.m_tkmFilePath = filePath;
		//fxファイルを指定
		initData.m_fxFilePath = "Assets/shader/model.fx";
		//定数バッファを指定
		initData.m_expandConstantBuffer = &g_renderingEngine.GetDeferredLightingCB();
		//定数バッファのサイズを指定
		initData.m_expandConstantBufferSize = sizeof(g_renderingEngine.GetDeferredLightingCB());
		//カラーバッファのフォーマットを指定
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//アニメーションクリップがあったら
		if (animationClips == nullptr)
		{
			//頂点シェーダーのエントリーポイントを指定
			initData.m_vsEntryPointFunc = "VSMain";
		}
		else
		{
			//スケルトンの指定
			initData.m_skeleton = &m_skeleton;
			//スキンありマテリアル用の頂点シェーダーを指定
			initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//スケルトンの初期化
			InitSkeleton(filePath);
			//アニメーションの初期化
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		}
		//モデルの上方向を指定
		initData.m_modelUpAxis = enModelUpAxis;
		//モデル初期化データでモデルを初期化
		m_model.Init(initData);
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
	
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}
