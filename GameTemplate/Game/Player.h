#pragma once

class GameCamera;

class Player:public IGameObject
{
public:
	enum EnPlayerState
	{
		enPlayerState_Idle,
		enPlayerState_Walk,
		enPlayerState_Cast,
		enPlayerState_Fit,
		enPlayerState_Fishing,
		enPlayerState_ReadyCast,
	};
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

	const int& GetState() const
	{
		return m_playerState;
	}
private:

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
	void ProcessFitStateTransition();
	void ProcessFishingStateTransition();

	void CalcSkeleton();
	void CalcAngle();
	void ChoiceAngleGroup();
	void CalcArea();

	//�p�x���O���[�v����������
	enum EnAngleGroup
	{
		enAngleGroup_0, //0�x����45�x
		enAngleGroup_1, //46�x����90�x
		enAngleGroup_2, //91�x����135�x
		enAngleGroup_3, //136�x����180�x
		enAngleGroup_4, //181�x����225�x
		enAngleGroup_5, //226�x����270�x
		enAngleGroup_6, //271�x����315�x
		enAngleGroup_7,  //316�x����360�x
		enAngleGroup_No
	};
	EnAngleGroup m_angleGroup = enAngleGroup_No;

	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Cast,
		enAnimClip_ReadyCast,
		enAnimClip_Fit,
		enAnimClip_Num
	};

private:
	FontRender m_fontRender;
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
	
	//�v�Z�p
	Skeleton calcSkeleton[3];
	Animation calcAnim[3];

	//�{��
	Skeleton skeleton;
	Model model;

	Animation anim;
	AnimationClip animclip;

	//�^�[�Q�b�g���[�V����
	enum entargetClip
	{
		enTargetClip_Normal,
		enTargetClip_Up,
		enTargetClip_UpRight,
		enTargetClip_Right,
		enTargetClip_DownRight,
		enTargetClip_Down,
		enTargetClip_DownLeft,
		enTargetClip_Left,
		enTargetClip_UpLeft,
		enTargetClip_Num
	};

	AnimationClip targetClipArray[enTargetClip_Num];
	Vector2 m_anglePos;
	float m_angle;
	float m_area1;
	float m_area2;
	float m_area3;
	Vector2 m_allArea;
};

