#pragma once

namespace nsK2EngineLow {
	class ModelRender
	{
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
	private:
		Vector3 m_position = Vector3::Zero;
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
	};

}