#pragma once

class GameCamera;

class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Vector3& GetForward() const
	{
		return m_forward;
	}
private:
	enum EnPlayerState
	{
		enPlayerState_Idle,
		enPlayerState_Walk,
		enPlayerState_Cast,
		enPlayerState_ReadyCast,
	};

	void LoadAnimationClip();
	void Move();
	void Rotation();
	void PlayAnimation();
	void ManageState();
	void ProcessCommonStateTransition();
	void ProcessIdleStateTransition();
	void ProcessWalkStateTransition();
	void ProcessCastStateTransition();
	void ProcessReadyCastStateTransition();
	void CalcSkeleton();

	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Cast,
		enAnimClip_ReadyCast,
		enAnimClip_Num
	};

private:
	EnPlayerState m_playerState = enPlayerState_Idle;
	CharacterController m_charaCon;
	ModelRender         m_modelRender;
	ModelRender         m_modelRender2;
	Animation           m_animation;
	AnimationClip       m_animationClipArray[enAnimClip_Num];
	Vector3				m_position = Vector3::Zero;
	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_scale = Vector3::One;
	Vector3             m_moveSpeed;
	Vector3             m_forward = Vector3::AxisZ;
	GameCamera* m_camera = nullptr;
	
	//計算用
	Skeleton calcSkeleton[3];
	Animation calcAnim[3];

	//本物
	Skeleton skeleton;
	Model model;

	Animation anim;
	AnimationClip animclip;

	//ターゲットモーション
	enum entargetClip
	{
		enTargetClip_Normal,
		enTargetClip_Up,
		enTargetClip_UpRight,
		enTargetClip_Num
	};
	AnimationClip targetClipArray[enTargetClip_Num];
};

