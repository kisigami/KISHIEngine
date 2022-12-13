#include "stdafx.h"
#include "FishingFloat.h"
#include "Player.h"

FishingFloat::FishingFloat()
{}

FishingFloat::~FishingFloat()
{}

bool FishingFloat::InitAnimation()
{
	//������A�j���[�V����
	m_animationClip[enAnimation_Atari].Load("Assets/animData/fishingfloat/atari.tka");
	m_animationClip[enAnimation_Atari].SetLoopFlag(true);
	//�ҋ@�A�j���[�V����
	m_animationClip[enAnimation_Idle].Load("Assets/animData/fishingfloat/idle.tka");
	m_animationClip[enAnimation_Idle].SetLoopFlag(true);
	return true;
}

bool FishingFloat::Start()
{
	//�A�j���[�V����������
	InitAnimation();
	//���f��������
	m_modelRender.Init("Assets/modelData/fishingfloat/float.tkm", m_animationClip, enAnimation_Num);
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");
	//�Ɛ�̍��W���擾
	m_position = m_player->GetRodTopPos();
	//������͂ňړ�����
	m_position += m_player->GetForward() * (m_player->GetCastPower() * 1600.0f + 200.0f);
	//�傫����ݒ�
	m_scale = { 0.5f,0.5f,0.5f };
	//�傫���A���W���X�V
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetPosition(m_position);
	//���f���̍X�V
	m_modelRender.Update();
	return true;
}

void FishingFloat::Update()
{
	//�ړ�����
	Move();

	//���W�̍X�V
	m_modelRender.SetPosition(m_position);
	//���f���̍X�V
	m_modelRender.Update();
}

void FishingFloat::Move()
{
	//���[����������Ă�����
	if (m_player->GetCoilingFlag() == true)
	{
		//�Ɛ�Ɍ������x�N�g�����v�Z
		Vector3 moveVector = m_player->GetRodTopPos() - m_position;
		//���K��
		moveVector.Normalize();
		//�ړ�����
		m_position += moveVector * 200.0f * g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		switch (m_state)
		{
		case FishingFloat::enFloatState_Idle:
			//�������Ȃ�
			break;
		case FishingFloat::enFloatState_Atari:
			//���΂炭������
			break;
		case FishingFloat::enFloatState_Chase:
			//������
			break;
		}
	}
}

void FishingFloat::Render(RenderContext& rc)
{
	//���f���`��
	m_modelRender.Draw(rc);
}