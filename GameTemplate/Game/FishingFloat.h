#pragma once

class Player;

class FishingFloat :public IGameObject
{
public:
	enum EnAnimation
	{
		enAnimation_Idle,
		enAnimation_Atari,
		enAnimation_Num,
	};

	enum EnFloatState
	{
		enFloatState_Idle,
		enFloatState_Atari,
		enFloatState_Chase,
	};

	FishingFloat();
	~FishingFloat();
	bool InitAnimation();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(Vector3& pos)
	{
		m_position = pos;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	const int& GetState() const
	{
		return m_state;
	}
private:
	void Move();

	Animation m_animation;
	AnimationClip m_animationClip[enAnimation_Num];
	Vector3 target;
	EnFloatState m_state = enFloatState_Idle;
	ModelRender m_modelRender;
	Vector3 m_moveSpeed;
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Player* m_player = nullptr;
	FontRender m_fontRender;
	Vector3 m_nextposition;
	float m_moveTimer = 3.0f;
	Vector3             m_forward = Vector3::AxisZ;
	Vector3             m_right = Vector3::AxisX;
};