#include "stdafx.h"
#include "UserInterface.h"
#include "Player.h"

UserInterface::UserInterface()
{}

UserInterface::~UserInterface()
{}

bool UserInterface::Start()
{
	m_castGaugeSprite.Init("Assets/sprite/castgauge/gauge.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_castGaugeSprite.SetPosition(Vector3{ -400.0f,-540.0f,0.0f });
	m_castGaugeSprite.SetPivot(Vector2{ 0.0f,0.0f });
	m_castGaugeButtomSprite.Init("Assets/sprite/castgauge/gaugebutton.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_castGaugeFrameSprite.Init("Assets/sprite/castgauge/gaugeframe.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	//m_castGaugeFrameSprite.SetMulColor(Vector4{ 1.0f, 1.0f, 1.0f, 0.3f });
	m_player = FindGO<Player>("player");
	return true;
}

void UserInterface::Update()
{
	Vector3 scale;
	if (m_player->GetState() == m_player->enPlayerState_Fishing)
	{
		scale = { m_player->GetFishiPower(),1.0f,1.0f };
		float red = 0.0f, green = 1.0f;
		red = m_player->GetFishiPower() * 2.0f;
		if (m_player->GetFishiPower() >= 0.5f)
		{
			float c = m_player->GetFishiPower() - 1.0f;
			green = fabsf(c) * 2.0f;
		}
		m_castGaugeSprite.SetMulColor(Vector4{ red,green,0.0f,1.0f });
	}

	else
	{
		scale = { m_player->GetCastPower(),1.0f,1.0f };
		float a = 1.0f - m_player->GetCastPower();
		m_castGaugeSprite.SetMulColor(Vector4{ a,1.0f,0.0f, 1.0f });
	}

	m_castGaugeSprite.SetScale(scale);
	m_castGaugeSprite.Update();

	m_castGaugeButtomSprite.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,0.5f });
	m_castGaugeButtomSprite.Update();
	m_castGaugeFrameSprite.Update();
}

void UserInterface::Render(RenderContext& rc)
{
	m_castGaugeButtomSprite.Draw(rc);
	m_castGaugeSprite.Draw(rc);
	m_castGaugeFrameSprite.Draw(rc);
}


