#include "stdafx.h"
#include "UserInterface.h"
#include "Player.h"

UserInterface::UserInterface()
{}

UserInterface::~UserInterface()
{}

bool UserInterface::Start()
{
	m_castGaugeSprite.Init("Assets/sprite/castgauge/gauge.dds",FRAME_BUFFER_W,FRAME_BUFFER_H);
	m_castGaugeSprite.SetPosition(Vector3{ -334.0f,-450.0f,0.0f });
	m_castGaugeSprite.SetPivot(Vector2{ 0.0f,0.0f });
	m_castGaugeButtomSprite.Init("Assets/sprite/castgauge/gaugebutton.dds",FRAME_BUFFER_W,FRAME_BUFFER_H);
	m_castGaugeFrameSprite.Init("Assets/sprite/castgauge/gaugeframe.dds", FRAME_BUFFER_W,FRAME_BUFFER_H);
	m_player = FindGO<Player>("player");
	return true;
}

void UserInterface::Update()
{
	Vector3 scale = {m_player->GetCastPower(),1.0f,1.0f};
	m_castGaugeSprite.SetScale(scale);
	float a = 1.0f - m_player->GetCastPower();
	m_castGaugeSprite.SetMulColor(Vector4{a,1.0f,0.0f,1.0f });
	m_castGaugeSprite.Update();

	m_castGaugeButtomSprite.SetMulColor(Vector4{1.0f,1.0f,1.0f,0.5f});
	m_castGaugeButtomSprite.Update();
	m_castGaugeFrameSprite.Update();
}

void UserInterface::Render(RenderContext& rc)
{
	m_castGaugeButtomSprite.Draw(rc);
	m_castGaugeSprite.Draw(rc);
	m_castGaugeFrameSprite.Draw(rc);
}


