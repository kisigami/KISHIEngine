#pragma once

class GameCamera;
class FishingFloat;

class Player :public IGameObject
{
public:
	enum EnPlayerState
	{
		enPlayerState_Idle, //�ҋ@
		enPlayerState_Walk, //����
		enPlayerState_Cast, //����
		enPlayerState_Fit, //������
		enPlayerState_Fishing, //�ނ蒆
		enPlayerState_ReadyCast,
		enPlayerState_Fishing_Idle,//������҂��X�e�[�g
	};
	//�p�x���O���[�v����������
	enum EnAngleGroup
	{
		enAngleGroup_0, //0�x����45�x�@�E
		enAngleGroup_1, //46�x����90�x�@�E
		enAngleGroup_2, //91�x����135�x�@��
		enAngleGroup_3, //136�x����180�x�@��
		enAngleGroup_4, //181�x����225�x�@��
		enAngleGroup_5, //226�x����270�x�@��
		enAngleGroup_6, //271�x����315�x�@�E
		enAngleGroup_7,  //316�x����360�x�@�E
		enAngleGroup_No  //���͂Ȃ�
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
	const float& GetCastPower() const
	{
		return m_castPower;
	}
	const float& GetFishiPower() const
	{
		return m_fishiPower;
	}
	const float& GetCoilingFlag() const
	{
		return coilingFlag;
	}
	const Vector3& GetRodTopPos() const
	{
		return m_rodTopPos;
	}
	const int& GetAnglePos() const
	{
		return m_angleGroup;
	}
	const Vector3& GetRight() const
	{
		return m_right;
	}
private:

	void LoadAnimationClip();
	void Move();
	void Rotation();
	void ManageState();
	void ProcessCommonStateTransition();
	void ProcessIdleStateTransition();
	void ProcessWalkStateTransition();
	void ProcessCastStateTransition();
	void ProcessReadyCastStateTransition();
	void ProcessFitStateTransition();
	void ProcessFishingStateTransition();

	void CalcAngle();
	void ChoiceAngleGroup();
	void CalcArea();
	void CalcBoneWeight();
	void ShedAnimBone();

	void MotionBlending();
	float brendWeight[3];
	Vector2 axis1 = { 0.0f,0.0f };
	Vector2 axis2 = { 0.0f,0.0f };
	void CalcCastPower();
	void Fishing();

	EnAngleGroup m_angleGroup = enAngleGroup_No;

	enum EnAnimationClip
	{
		enAnimClip_Idle,//�ҋ@
		enAnimClip_Walk,//����
		enAnimClip_Cast,//����
		enAnimClip_ReadyCast,//��������
		enAnimClip_Fit,//������
		enTargetClip_Normal,
		enTargetClip_Up,
		enTargetClip_UpRight,
		enTargetClip_Right,
		enTargetClip_DownRight,
		enTargetClip_Down,
		enTargetClip_DownLeft,
		enTargetClip_Left,
		enTargetClip_UpLeft,
		enAnimClip_Num//��
	};

private:
	FontRender m_fontRender;
	FontRender m_fontRender2;
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
	Vector3             m_right = Vector3::AxisX;

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

	};

	Vector2 m_anglePos = Vector2::Zero;
	float m_angle = 0.0f;
	float m_area1 = 0.0f;
	float m_area2 = 0.0f;
	float m_area3 = 0.0f;
	Vector2 m_allArea = Vector2::Zero;
	float m_castPower = 0.0f;
	bool flag = false;

	float m_fishiPower = 0.0f;
	bool coilingFlag = false;

	Vector3 m_ukipos;
	float hikyori = 10.0f;
	FishingFloat* m_fishingFloat = nullptr;
	bool a = false;
	int m_rodBoneID = -1;
	Vector3 m_rodTopPos;
	float m_fleeTimer = 0.0f;
	float m_fishFatigueValue = 0.0f;
	enum EnFishDirection
	{
		enFishState_Front,
		enFishState_Right,
		enFishStaet_Left,
	};
	EnFishDirection m_fishDirectionState = enFishState_Front;
};

