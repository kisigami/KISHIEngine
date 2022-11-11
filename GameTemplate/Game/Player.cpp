#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

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

	//�^�[�Q�b�g���[�V�����̃N���b�v������������
	//�^�񒆂̃^�[�Q�b�g���[�V����
	targetClipArray[enTargetClip_Normal].Load("Assets/animData/player/rod0.tka");
	targetClipArray[enTargetClip_Normal].SetLoopFlag(false);
	//��̃^�[�Q�b�g���[�V����
	targetClipArray[enTargetClip_Up].Load("Assets/animData/player/rod1.tka");
	targetClipArray[enTargetClip_Up].SetLoopFlag(false);
	//�E��̃^�[�Q�b�g���[�V����
	targetClipArray[enTargetClip_UpRight].Load("Assets/animData/player/rod2.tka");
	targetClipArray[enTargetClip_UpRight].SetLoopFlag(false);

	//�v�Z�p�̃A�j���[�V����������������
	for (int i = 0; i < 3; i++)
	{
		//�X�P���g�������[�h����
		calcSkeleton[i].Init("Assets/modelData/player/player.tks");
		//�A�j���[�V�����ɃX�P���g���ƃ^�[�Q�b�g���[�V������n��
		calcAnim[i].Init(calcSkeleton[i], targetClipArray,enTargetClip_Num);
	}

	//�{���̃��f��
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/player/player.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	skeleton.Init("Assets/modelData/player/player.tks");
	initData.m_expandConstantBuffer = &g_renderingEngine.GetDeferredLightingCB();
	initData.m_expandConstantBufferSize = sizeof(g_renderingEngine.GetDeferredLightingCB());
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_skeleton = &skeleton;
	initData.m_vsSkinEntryPointFunc = "VSSkinMain";
	model.Init(initData);
}

bool Player::Start()
{
	//�J�����̃C���X�^���X��T��
	m_camera = FindGO<GameCamera>("gamecamera");
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
	//�L�����R����ǂݍ���
	m_charaCon.Init(25.0f, 70.0f, m_position);
	//���f���̍X�V
	m_modelRender.Update();
	return true;
}

void Player::CalcSkeleton()
{
	//�v�Z�p�̃A�j���[�V�����Đ�
	calcAnim[0].Play(enTargetClip_Normal);
	calcAnim[1].Play(enTargetClip_Up);
	calcAnim[2].Play(enTargetClip_UpRight);

	calcAnim[0].Progress(1/60.0f);
	calcAnim[1].Progress(1/60.0f);
	calcAnim[2].Progress(1/60.0f);

	//�d�����̐��l
	float brendWeight[3];
	brendWeight[0] = 0.33f;
	brendWeight[1] = 0.33f;
	brendWeight[2] = 0.33f;

	//�ŏI�{�[���v�Z
	std::unique_ptr<Matrix[]> boneMatrices;
	boneMatrices.reset(new Matrix[skeleton.GetNumBones()]);

	for (int boneNo = 0; boneNo < skeleton.GetNumBones(); boneNo++)
	{
		Matrix boneMat;
		//1�ڂ̃X�P���g���̃{�[���s����擾
		boneMat = calcSkeleton[0].GetBoneMatricesTopAddress()[boneNo];
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
		boneMat1 = calcSkeleton[1].GetBoneMatricesTopAddress()[boneNo];

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
		boneMat2 = calcSkeleton[2].GetBoneMatricesTopAddress()[boneNo];

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

		//boneMat = calcSkeleton[0].GetBoneMatricesTopAddress()[boneNo] * brendWeight[0];
		//boneMat += calcSkeleton[1].GetBoneMatricesTopAddress()[boneNo] * brendWeight[1];
		//boneMat += calcSkeleton[2].GetBoneMatricesTopAddress()[boneNo] * brendWeight[2];

		//�{�[����ݒ�
		skeleton.SetBoneLocalMatrix(boneNo, boneMat);
	}
	
}

void Player::Update()
{
	//���[�V�����u�����f�B���O
	CalcSkeleton();
	
	//�{��
	//model.UpdateWorldMatrix(m_position,m_rotation,m_scale);
	skeleton.Update(model.GetWorldMatrix());
	anim.Progress(g_gameTime->GetFrameDeltaTime() * 1.0f);

	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�A�j���[�V��������
	PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
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

	//�h���̂�
	//���f���̍��W�ɂ̓J�����̍��W������
	Vector3 newposition = m_camera->GetPosition();
	newposition.y = 0.0f;

	//���W��ݒ肷��
	m_modelRender.SetPosition(newposition);
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
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		break;
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.4f);
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.3f);
		break;
	case enPlayerState_Cast:
		m_modelRender.SetAnimationSpeed(2.0f);
		m_modelRender.PlayAnimation(enAnimClip_Cast, 0.3f);
		break;
	case enPlayerState_ReadyCast:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimClip_ReadyCast, 0.3f);
		break;
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
	}
}

void Player::ProcessCommonStateTransition()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
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
		//ProcessCommonStateTransition();
		return;
	}
}

void Player::ProcessReadyCastStateTransition()
{
	//�A�j���[�V�������Đ����ł͂Ȃ�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//A�{�^����������Ă��Ȃ�������
		if (g_pad[0]->IsPress(enButtonA) == false)
		{
			//�L���X�g�X�e�[�g��
			m_playerState = enPlayerState_Cast;
			return;
		}
	}
}

void Player::Render(RenderContext& rc)
{
	//�{���̕`��
	model.Draw(rc);
	//���f���̕`��
	m_modelRender.Draw(rc);
}