#pragma once
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void LoadAnimationClip();
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Num
	};

private:
	ModelRender m_modelRender;
	Animation m_animation;
	AnimationClip m_animationClipArray[enAnimClip_Num];
	Vector3				m_position = Vector3::Zero;
	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_scale = Vector3::One;
};

