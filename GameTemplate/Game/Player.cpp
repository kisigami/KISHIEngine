#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::LoadAnimationClip()
{
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
}

bool Player::Start()
{
	LoadAnimationClip();
	m_modelRender.Init("Assets/modelData/unityChan2.tkm",m_animationClipArray, enAnimClip_Num, enModelUpAxisY);
	return true;
}

void Player::Update()
{
	m_rotation.SetRotationDegY(180.0f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}