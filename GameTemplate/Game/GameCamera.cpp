#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");
	return true;
}

void GameCamera::Update()
{
	m_target = m_player->GetPosition();
	m_target.y += 50.0f;
	m_position = Vector3(0.0f, 150.0f, 200.0f);
	g_camera3D->SetPosition(m_position);
	g_camera3D->SetTarget(m_target);
}
