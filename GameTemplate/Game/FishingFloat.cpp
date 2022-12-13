#include "stdafx.h"
#include "FishingFloat.h"
#include "Player.h"

FishingFloat::FishingFloat()
{}

FishingFloat::~FishingFloat()
{}

bool FishingFloat::InitAnimation()
{
	//当たりアニメーション
	m_animationClip[enAnimation_Atari].Load("Assets/animData/fishingfloat/atari.tka");
	m_animationClip[enAnimation_Atari].SetLoopFlag(true);
	//待機アニメーション
	m_animationClip[enAnimation_Idle].Load("Assets/animData/fishingfloat/idle.tka");
	m_animationClip[enAnimation_Idle].SetLoopFlag(true);
	return true;
}

bool FishingFloat::Start()
{
	//アニメーション初期化
	InitAnimation();
	//モデル初期化
	m_modelRender.Init("Assets/modelData/fishingfloat/float.tkm", m_animationClip, enAnimation_Num);
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	//竿先の座標を取得
	m_position = m_player->GetRodTopPos();
	//投げる力で移動する
	m_position += m_player->GetForward() * (m_player->GetCastPower() * 1600.0f + 200.0f);
	//大きさを設定
	m_scale = { 0.5f,0.5f,0.5f };
	//大きさ、座標を更新
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetPosition(m_position);
	//モデルの更新
	m_modelRender.Update();
	return true;
}

void FishingFloat::Update()
{
	//移動処理
	Move();

	//座標の更新
	m_modelRender.SetPosition(m_position);
	//モデルの更新
	m_modelRender.Update();
}

void FishingFloat::Move()
{
	//リールが巻かれていたら
	if (m_player->GetCoilingFlag() == true)
	{
		//竿先に向かうベクトルを計算
		Vector3 moveVector = m_player->GetRodTopPos() - m_position;
		//正規化
		moveVector.Normalize();
		//移動する
		m_position += moveVector * 200.0f * g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		switch (m_state)
		{
		case FishingFloat::enFloatState_Idle:
			//何もしない
			break;
		case FishingFloat::enFloatState_Atari:
			//しばらく当たる
			break;
		case FishingFloat::enFloatState_Chase:
			//逃げる
			break;
		}
	}
}

void FishingFloat::Render(RenderContext& rc)
{
	//モデル描画
	m_modelRender.Draw(rc);
}