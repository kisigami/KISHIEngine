#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "FishingFloat.h"

namespace
{

}

Player::Player()
{

}

Player::~Player()
{

}

void Player::LoadAnimationClip()
{
	//�A�j���[�V�����N���b�v������������
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Cast].Load("Assets/animData/player/cast.tka");
	m_animationClipArray[enAnimClip_Cast].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReadyCast].Load("Assets/animData/player/readycast.tka");
	m_animationClipArray[enAnimClip_ReadyCast].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Fit].Load("Assets/animData/player/fit.tka");
	m_animationClipArray[enAnimClip_Fit].SetLoopFlag(false);

	//�^�[�Q�b�g���[�V�����̃N���b�v������������
	//�^�񒆂̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_Normal].Load("Assets/animData/player/rodnormal.tka");
	m_animationClipArray[enTargetClip_Normal].SetLoopFlag(false);
	//��̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_Up].Load("Assets/animData/player/rodup.tka");
	m_animationClipArray[enTargetClip_Up].SetLoopFlag(false);
	//�E��̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_UpRight].Load("Assets/animData/player/rodupright.tka");
	m_animationClipArray[enTargetClip_UpRight].SetLoopFlag(false);
	//�E�̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_Right].Load("Assets/animData/player/rodright.tka");
	m_animationClipArray[enTargetClip_Right].SetLoopFlag(false);
	//�E���̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_DownRight].Load("Assets/animData/player/roddownright.tka");
	m_animationClipArray[enTargetClip_DownRight].SetLoopFlag(false);
	//���̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_Down].Load("Assets/animData/player/roddown.tka");
	m_animationClipArray[enTargetClip_Down].SetLoopFlag(false);
	//�����̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_DownLeft].Load("Assets/animData/player/roddownleft.tka");
	m_animationClipArray[enTargetClip_DownLeft].SetLoopFlag(false);
	//���̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_Left].Load("Assets/animData/player/rodleft.tka");
	m_animationClipArray[enTargetClip_Left].SetLoopFlag(false);
	//����̃^�[�Q�b�g���[�V����
	m_animationClipArray[enTargetClip_UpLeft].Load("Assets/animData/player/rodupleft.tka");
	m_animationClipArray[enTargetClip_UpLeft].SetLoopFlag(false);

	//�v�Z�p�̃A�j���[�V����������������
	for (int i = 0; i < 3; i++)
	{
		//�X�P���g�������[�h����
		calcSkeleton[i].Init("Assets/modelData/player/player.tks");
		//�A�j���[�V�����ɃX�P���g���ƃ^�[�Q�b�g���[�V������n��
		calcAnim[i].Init(calcSkeleton[i], m_animationClipArray, enAnimClip_Num);
	}

	skeleton.Init("Assets/modelData/player/player.tks");
}

bool Player::Start()
{
	//�J�����̃C���X�^���X��T��
	m_camera = FindGO<GameCamera>("gamecamera");
	m_fishingFloat = FindGO<FishingFloat>("fish");
	//�A�j���[�V�����̃��[�h
	LoadAnimationClip();
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player.tkm", m_animationClipArray, enAnimClip_Num);
	//�傫����ݒ�
	Vector3 scale = { 2.5f, 2.5f, 2.5f };
	//���W��ݒ�
	m_position = Vector3::Zero;
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(scale);

	m_rodBoneID = m_modelRender.FindBoneID(L"RodBone004");
	//�L�����R����ǂݍ���
	m_charaCon.Init(25.0f, 70.0f, m_position);
	//���f���̍X�V
	m_modelRender.Update();
	return true;
}

void Player::Update()
{
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();
	//������͂̌v�Z
	CalcCastPower();
	//�ނ菈��
	Fishing();
	//�p�x�̌v�Z
	CalcAngle();
	//�}�[�N��t����
	skeleton.SetMarkPlayAnimation();
	//�X�P���g���X�V
	skeleton.Update(m_modelRender.GetWorldMatrix());
	//���f���̍X�V
	m_modelRender.Update();
	//�X�P���g���̃R�s�[
	m_modelRender.CopySkeleton(skeleton);

	//�Ɛ�̃{�[���s����擾
	Matrix matrix = skeleton.GetBone(m_rodBoneID)->GetWorldMatrix();
	//���W��ݒ�
	m_rodTopPos.x = matrix.m[3][0];
	m_rodTopPos.y = matrix.m[3][1];
	m_rodTopPos.z = matrix.m[3][2];

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"�^�C�}�[%f", float(m_fleeTimer));

	//�\������e�L�X�g��ݒ�B
	m_fontRender.SetText(wcsbuf);
	m_fontRender.SetPosition(-500.0f, 200.0f, 1.0f);
	m_fontRender.SetScale(2.3f);
	m_fontRender.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"�L��%f", float(m_fishFatigueValue));

	//�\������e�L�X�g��ݒ�B
	m_fontRender2.SetText(wcsbuf2);
	m_fontRender2.SetPosition(-500.0f, 100.0f, 1.0f);
	m_fontRender2.SetScale(2.3f);
	m_fontRender2.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

}

void Player::Move()
{
	//�ړ����x
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//�X�e�B�b�N�̓��͗ʁ~���x	
	m_moveSpeed += cameraForward * lStick_y * 1000.0f;
	m_moveSpeed += cameraRight * lStick_x * 1000.0f;
	m_moveSpeed.y = 0.0f;
	//�L�����R�����g�p���č��W�𓮂���
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//�����_�ւ̃x�N�g�����v�Z����
	Vector3 vector = m_camera->GetTarget() - m_camera->GetPosition();
	//���K������
	vector.Normalize();
	//�����_��������]���쐬
	m_rotation.SetRotationYFromDirectionXZ(vector);
	//��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::CalcCastPower()
{
	if (m_playerState != enPlayerState_ReadyCast)
	{
		return;
	}

	if (m_castPower > 1.0f)
	{
		m_castPower = 1.0f;
		flag = true;
	}
	if (m_castPower < 0.0f)
	{
		m_castPower = 0.0f;
		flag = false;
	}

	switch (flag)
	{
	case true:
		m_castPower -= g_gameTime->GetFrameDeltaTime() * 0.5f;
		break;
	case false:
		m_castPower += g_gameTime->GetFrameDeltaTime() * 0.5f;
		break;
	}
}

void Player::Fishing()
{
	//�t�B�b�V���O�ҋ@�X�e�[�g�ȊO�Ȃ�
	if (m_playerState != enPlayerState_Fishing_Idle)
	{
		//�������Ȃ�
		return;
	}

	//LB2�{�^����������Ă��邩?
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		//���[����������Ă���
		coilingFlag = true;
	}
	else
	{
		//���[����������Ă��Ȃ�
		coilingFlag = false;
	}

	//���̒��肪�ő�l�𒴂��Ă�����
	if (m_fishiPower >= 1.0f)
	{
		//�^�C�}�[�����Z����
		m_fleeTimer += g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		//�^�C�}�[�����Z�b�g����
		m_fleeTimer = 0.0f;
	}

	if (m_fleeTimer >= 1.0f)
	{
		//�ҋ@�X�e�[�g��
		m_playerState = enPlayerState_Idle;
		DeleteGO(m_fishingFloat);
		a = false;
	}

	////���[���������ĂȂ��Ƃ�
	//if (coilingFlag == false)
	//{
	//	m_fishiPower -= g_gameTime->GetFrameDeltaTime() * 0.7f;
	//	if (m_fishiPower < 0.0f)
	//	{
	//		m_fishiPower = 0.0f;
	//	}
	//	m_fishFatigueValue -= g_gameTime->GetFrameDeltaTime() * 0.5f;
	//}
	////�����Ă�Ƃ�
	//else
	//{
	//	m_fishiPower += g_gameTime->GetFrameDeltaTime() * 2.0f;
	//	if (m_fishiPower > 1.0f)
	//	{
	//		m_fishiPower = 1.0f;
	//	}
	//	m_fishFatigueValue += g_gameTime->GetFrameDeltaTime() * 3.0f;
	//}

	//������x�߂�������I���
	Vector3 diff = m_fishingFloat->GetPosition() - m_rodTopPos;
	if (diff.Length() < 140.0f)
	{
		m_playerState = enPlayerState_Idle;
		DeleteGO(m_fishingFloat);
		a = false;
	}
	////������x����������I���
	//if (diff.Length() > 2500.0f)
	//{
	//	m_playerState = enPlayerState_Idle;
	//	DeleteGO(m_fishingFloat);
	//	a = false;
	//}
}

void Player::CalcAngle()
{
	if (m_playerState != enPlayerState_Fishing &&
		m_playerState != enPlayerState_Fishing_Idle)
	{
		CalcArea();
		return;
	}

	Vector2 originPosition = Vector2::Zero;
	Vector2 angleposition = Vector2::Zero;

	angleposition.y = g_pad[0]->GetRStickYF();
	angleposition.x = g_pad[0]->GetRStickXF();

	//X���̃x�N�g���B
	Vector2 axisX = { 1.0f,0.0f };
	//���K���i�������P�Ɂj
	axisX.Normalize();

	//�ˉe�B
	float dot = angleposition.Dot(axisX);

	m_anglePos = angleposition;

	//cos�l�����߂�B
	float angleCos = dot / angleposition.Length();

	//���͊p�x�����߂�B
	float angle = acos(angleCos);
	angle = Math::RadToDeg(angle);

	//���͕����������߂������画��p�x�𔽓]�B
	if (angleposition.y < 0)
	{
		angle = 360.0f - angle;
	}

	m_angle = angle;

	//�p�x�ŃO���[�v��������
	ChoiceAngleGroup();
}

void Player::ChoiceAngleGroup()
{
	//�X�e�B�b�N�̓��͂�����������
	if (g_pad[0]->GetRStickXF() == 0 &&
		g_pad[0]->GetRStickYF() == 0)
	{
		m_angleGroup = enAngleGroup_No;
	}
	//0�x����45�x��������
	if (m_angle >= 0.0f && m_angle <= 45.0f)
	{
		m_angleGroup = enAngleGroup_0;
	}
	//45�x����90�x��������
	else if (m_angle > 45.0f && m_angle <= 90.0f)
	{
		m_angleGroup = enAngleGroup_1;
	}
	//91�x����135�x��������
	else if (m_angle > 90.0f && m_angle <= 135.0f)
	{
		m_angleGroup = enAngleGroup_2;
	}
	//136�x����180�x��������
	else if (m_angle > 135.0f && m_angle <= 180.0f)
	{
		m_angleGroup = enAngleGroup_3;
	}
	//181�x����225�x��������
	else if (m_angle > 180.0f && m_angle <= 225.0f)
	{
		m_angleGroup = enAngleGroup_4;
	}
	//226�x����270�x��������
	else if (m_angle > 225.0f && m_angle <= 270.0f)
	{
		m_angleGroup = enAngleGroup_5;
	}
	//271�x����315�x��������
	else if (m_angle > 270.0f && m_angle <= 315.0f)
	{
		m_angleGroup = enAngleGroup_6;
	}
	//316�x����360�x��������
	else if (m_angle > 315.0f && m_angle <= 360.0f)
	{
		m_angleGroup = enAngleGroup_7;
	}

	//�ʐς̌v�Z����
	CalcArea();
}

void Player::CalcArea()
{
	//�ʐς��v�Z���邽�߂̎����`
	Vector2 axis1 = { 0.0f,0.0f };
	Vector2 axis2 = { 0.0f,0.0f };
	float animSpeed = 60.0f / 1.0f;
	//�ʐς��v�Z���邽�߂̎��ƃ^�[�Q�b�g�N���b�v��I��
	switch (m_angleGroup)
	{
		//0�x����45�x�̎�
	case Player::enAngleGroup_0:
		//�^�E�̎�
		axis1 = { 1.0f,0.0f };
		//�E��̎�
		axis2 = { 1.0f,1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Right);
		calcAnim[2].Play(enTargetClip_UpRight);
		break;
		//46�x����90�x�̎�
	case Player::enAngleGroup_1:
		//�E��̎�
		axis1 = { 1.0f,1.0f };
		//�^��̎�
		axis2 = { 0.0f,1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_UpRight);
		calcAnim[2].Play(enTargetClip_Up);
		break;
		//91�x����135�x�̎�
	case Player::enAngleGroup_2:
		//�^��̎�
		axis1 = { 0.0f,1.0f };
		//����̎�
		axis2 = { -1.0f,1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Up);
		calcAnim[2].Play(enTargetClip_UpLeft);
		break;
		//135�x����180�x�̎�
	case Player::enAngleGroup_3:
		//����̎�
		axis1 = { -1.0f,1.0f };
		//�^���̎�
		axis2 = { -1.0f,0.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_UpLeft);
		calcAnim[2].Play(enTargetClip_Left);
		break;
		//181�x����225�x�̎�
	case Player::enAngleGroup_4:
		//�^���̎�
		axis1 = { -1.0f,0.0f };
		//�����̎�
		axis2 = { -1.0f,-1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Left);
		calcAnim[2].Play(enTargetClip_DownLeft);
		break;
		//226�x����270�x�̎�
	case Player::enAngleGroup_5:
		//�����̎�
		axis1 = { -1.0f,-1.0f };
		//�^���̎�
		axis2 = { 0.0f,-1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_DownLeft);
		calcAnim[2].Play(enTargetClip_Down);
		break;
		//271�x����315�x�̎�
	case Player::enAngleGroup_6:
		//�^���̎�
		axis1 = { 0.0f,-1.0f };
		//�E���̎�
		axis2 = { 1.0f,-1.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Down);
		calcAnim[2].Play(enTargetClip_DownRight);
		break;
		//316�x����360�x�̎�
	case Player::enAngleGroup_7:
		//�E���̎�
		axis1 = { 1.0f,-1.0f };
		//�^�E�̎�
		axis2 = { 1.0f,0.0f };
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_DownRight);
		calcAnim[2].Play(enTargetClip_Right);
		break;
		//�I�����ĂȂ��Ƃ�
	case Player::enAngleGroup_No:
		//�������Ȃ�
	default:
		switch (m_playerState)
		{
		case Player::enPlayerState_Idle:
			calcAnim[0].Play(enAnimClip_Idle, 0.3f);
			animSpeed = 1.0f;
			break;
		case Player::enPlayerState_Walk:
			calcAnim[0].Play(enAnimClip_Walk, 0.3f);
			animSpeed = 1.4f;
			break;
		case Player::enPlayerState_Cast:
			calcAnim[0].Play(enAnimClip_Cast, 0.3f);
			animSpeed = 2.0f;
			break;
		case Player::enPlayerState_Fit:
			calcAnim[0].Play(enAnimClip_Fit, 0.3f);
			animSpeed = 1.0f;
			break;
		case Player::enPlayerState_ReadyCast:
			calcAnim[0].Play(enAnimClip_ReadyCast, 0.3f);
			animSpeed = 1.0f;
			break;
		}
		break;
	}

	//�A�j���[�V�����Đ�
	calcAnim[0].Progress(animSpeed * g_gameTime->GetFrameDeltaTime());
	calcAnim[1].Progress(1 / 60.0f);
	calcAnim[2].Progress(1 / 60.0f);

	float angle = m_angle;

	//S1�̖ʐ�(�E���̎O�p�`)
	axis1.Normalize();
	Vector2 area1 = { 0.0f,0.0f };
	area1.Cross(m_anglePos, axis1);
	if (area1.x < 0.0f)
	{
		area1.x *= -1;
	}
	area1.x /= 2.0f;
	if (area1.y < 0.0f)
	{
		area1.y *= -1;
	}
	area1.y /= 2.0f;

	//S2�̖ʐ�
	axis2.Normalize();
	Vector2 area2 = { 0.0f,0.0f };
	area2.Cross(m_anglePos, axis2);
	if (area2.x < 0.0f)
	{
		area2.x *= -1.0f;
	}
	area2.x /= 2.0f;
	if (area2.y < 0.0f)
	{
		area2.y *= -1.0f;
	}
	area2.y /= 2.0f;

	//S3�̖ʐ�
	Vector2 area3 = { 0.3535544845f, 0.3535544845f };
	area3.x -= (area1.x + area2.x);
	area3.y -= (area1.y + area2.y);
	if (area3.x < 0.0f)
	{
		area3.x = 0.0f;
	}
	if (area3.y < 0.0f)
	{
		area3.y = 0.0f;
	}

	float allarea;
	allarea = area1.x + area2.x + area3.x;
	m_area1 = area1.x / allarea;
	m_area2 = area2.x / allarea;
	m_area3 = area3.x / allarea;

	//�d�����̐��l
	if (m_angleGroup != enAngleGroup_No && m_playerState == enPlayerState_Fishing || m_playerState == enPlayerState_Fishing_Idle)
	{
		brendWeight[0] = m_area3;
		brendWeight[1] = m_area2;
		brendWeight[2] = m_area1;
	}
	else
	{
		brendWeight[0] = 1.0f;
		brendWeight[1] = 0.0f;
		brendWeight[2] = 0.0f;
	}

	MotionBlending();
}

void Player::MotionBlending()
{
	//�ŏI�{�[���v�Z
	std::unique_ptr<Matrix[]> boneMatrices;
	boneMatrices.reset(new Matrix[skeleton.GetNumBones()]);

	for (int boneNo = 0; boneNo < skeleton.GetNumBones(); boneNo++)
	{
		Matrix boneMat;
		//1�ڂ̃X�P���g���̃{�[���s����擾
		boneMat = calcSkeleton[0].GetBone(boneNo)->GetLocalMatrix();
		//�{�[���s��~�O�p�`�̏d��
		boneMat.m[0][0] *= brendWeight[0];
		boneMat.m[0][1] *= brendWeight[0];
		boneMat.m[0][2] *= brendWeight[0];
		boneMat.m[0][3] *= brendWeight[0];

		boneMat.m[1][0] *= brendWeight[0];
		boneMat.m[1][1] *= brendWeight[0];
		boneMat.m[1][2] *= brendWeight[0];
		boneMat.m[1][3] *= brendWeight[0];

		boneMat.m[2][0] *= brendWeight[0];
		boneMat.m[2][1] *= brendWeight[0];
		boneMat.m[2][2] *= brendWeight[0];
		boneMat.m[2][3] *= brendWeight[0];

		boneMat.m[3][0] *= brendWeight[0];
		boneMat.m[3][1] *= brendWeight[0];
		boneMat.m[3][2] *= brendWeight[0];
		boneMat.m[3][3] *= brendWeight[0];

		Matrix boneMat1;
		//�Q�ڂ̃{�[���s����擾
		boneMat1 = calcSkeleton[1].GetBone(boneNo)->GetLocalMatrix();

		boneMat1.m[0][0] *= brendWeight[1];
		boneMat1.m[0][1] *= brendWeight[1];
		boneMat1.m[0][2] *= brendWeight[1];
		boneMat1.m[0][3] *= brendWeight[1];

		boneMat1.m[1][0] *= brendWeight[1];
		boneMat1.m[1][1] *= brendWeight[1];
		boneMat1.m[1][2] *= brendWeight[1];
		boneMat1.m[1][3] *= brendWeight[1];

		boneMat1.m[2][0] *= brendWeight[1];
		boneMat1.m[2][1] *= brendWeight[1];
		boneMat1.m[2][2] *= brendWeight[1];
		boneMat1.m[2][3] *= brendWeight[1];

		boneMat1.m[3][0] *= brendWeight[1];
		boneMat1.m[3][1] *= brendWeight[1];
		boneMat1.m[3][2] *= brendWeight[1];
		boneMat1.m[3][3] *= brendWeight[1];

		Matrix boneMat2;
		//3�ڂ̃{�[���s����擾
		boneMat2 = calcSkeleton[2].GetBone(boneNo)->GetLocalMatrix();

		boneMat2.m[0][0] *= brendWeight[2];
		boneMat2.m[0][1] *= brendWeight[2];
		boneMat2.m[0][2] *= brendWeight[2];
		boneMat2.m[0][3] *= brendWeight[2];

		boneMat2.m[1][0] *= brendWeight[2];
		boneMat2.m[1][1] *= brendWeight[2];
		boneMat2.m[1][2] *= brendWeight[2];
		boneMat2.m[1][3] *= brendWeight[2];

		boneMat2.m[2][0] *= brendWeight[2];
		boneMat2.m[2][1] *= brendWeight[2];
		boneMat2.m[2][2] *= brendWeight[2];
		boneMat2.m[2][3] *= brendWeight[2];

		boneMat2.m[3][0] *= brendWeight[2];
		boneMat2.m[3][1] *= brendWeight[2];
		boneMat2.m[3][2] *= brendWeight[2];
		boneMat2.m[3][3] *= brendWeight[2];

		//�d�݂̑����Z
		boneMat.m[0][0] += boneMat1.m[0][0] + boneMat2.m[0][0];
		boneMat.m[0][1] += boneMat1.m[0][1] + boneMat2.m[0][1];
		boneMat.m[0][2] += boneMat1.m[0][2] + boneMat2.m[0][2];
		boneMat.m[0][3] += boneMat1.m[0][3] + boneMat2.m[0][3];

		boneMat.m[1][0] += boneMat1.m[1][0] + boneMat2.m[1][0];
		boneMat.m[1][1] += boneMat1.m[1][1] + boneMat2.m[1][1];
		boneMat.m[1][2] += boneMat1.m[1][2] + boneMat2.m[1][2];
		boneMat.m[1][3] += boneMat1.m[1][3] + boneMat2.m[1][3];

		boneMat.m[2][0] += boneMat1.m[2][0] + boneMat2.m[2][0];
		boneMat.m[2][1] += boneMat1.m[2][1] + boneMat2.m[2][1];
		boneMat.m[2][2] += boneMat1.m[2][2] + boneMat2.m[2][2];
		boneMat.m[2][3] += boneMat1.m[2][3] + boneMat2.m[2][3];

		boneMat.m[3][0] += boneMat1.m[3][0] + boneMat2.m[3][0];
		boneMat.m[3][1] += boneMat1.m[3][1] + boneMat2.m[3][1];
		boneMat.m[3][2] += boneMat1.m[3][2] + boneMat2.m[3][2];
		boneMat.m[3][3] += boneMat1.m[3][3] + boneMat2.m[3][3];

		skeleton.SetBoneLocalMatrix(boneNo, boneMat);
	}

	//�A�j���[�V�����Đ����ł͂Ȃ�
	if (calcAnim[0].IsPlaying() == false)
	{
		switch (m_playerState)
		{
		case Player::enPlayerState_Cast:
			if (a == false)
			{
				m_fishingFloat = NewGO<FishingFloat>(0, "fish");
				a = true;
			}
			m_playerState = enPlayerState_Fishing_Idle;
			break;
		case Player::enPlayerState_Fishing_Idle:
			break;
		case Player::enPlayerState_Fit:
			m_playerState = enPlayerState_Fishing;
			break;
		case Player::enPlayerState_Fishing:
			break;
		case Player::enPlayerState_ReadyCast:
			if (g_pad[0]->IsPress(enButtonRB2) == false)
			{
				m_playerState = enPlayerState_Cast;
			}
			break;
		}
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
	case enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
	case enPlayerState_Cast:
		ProcessCastStateTransition();
		break;
	case enPlayerState_ReadyCast:
		ProcessReadyCastStateTransition();
		break;
	case enPlayerState_Fit:
		ProcessFitStateTransition();
		break;
	case enPlayerState_Fishing:
		ProcessFishingStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonRB2))
	{
		//�L���X�g�����X�e�[�g��
		m_playerState = enPlayerState_ReadyCast;
		return;
	}

	//�ړ����x����������
	if (fabsf(m_moveSpeed.x) >= 0.001f ||
		fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//����X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Walk;
		return;
	}

	//�ړ����x���Ȃ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ProcessIdleStateTransition()
{
	//���ʂ̑J�ڏ�����
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//���ʂ̑J�ڏ�����
	ProcessCommonStateTransition();
}

void Player::ProcessCastStateTransition()
{
	//�A�j���[�V�������Đ����ł͂Ȃ�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�L���X�g�����X�e�[�g��
			m_playerState = enPlayerState_Fit;
			return;
		}
		return;
	}
}

void Player::ProcessReadyCastStateTransition()
{
	//�A�j���[�V�������Đ����ł͂Ȃ�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

	}
}

void Player::ProcessFitStateTransition()
{
	//�A�j���[�V�������Đ����ł͂Ȃ�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�L���X�g�X�e�[�g��
		m_playerState = enPlayerState_Fishing;
		return;
	}
}

void Player::ProcessFishingStateTransition()
{
	//�A�j���[�V�������Đ����ł͂Ȃ�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (g_pad[0]->IsPress(enButtonRB2) == false)
		{
			//�L���X�g�X�e�[�g��
			m_playerState = enPlayerState_Cast;
			return;
		}
	}
}
void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
}