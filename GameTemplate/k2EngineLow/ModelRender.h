#pragma once

namespace  nsK2EngineLow {
	class ModelRender:public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ);

		void Update();

		void Draw(RenderContext& rc);

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		void SetScale(Vector3& scale)
		{
			m_scale = scale;
		}

		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

	private:
		void InitSkeleton(const char* filePath);

		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

	private:
		AnimationClip* m_animationClips = nullptr;
		Animation      m_animation;
		int            m_numAnimationClips = 0;
		float		   m_animationSpeed = 1.0f;
		Vector3        m_position = Vector3::Zero;
		Vector3        m_scale = Vector3::One;
		Quaternion     m_rotation = Quaternion::Identity;
		Model          m_model;
		Skeleton       m_skeleton;
	};

}