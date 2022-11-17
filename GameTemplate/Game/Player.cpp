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
	//アニメーションクリップを初期化する
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Cast].Load("Assets/animData/player/cast.tka");
	m_animationClipArray[enAnimClip_Cast].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReadyCast].Load("Assets/animData/player/readycast.tka");
	m_animationClipArray[enAnimClip_ReadyCast].SetLoopFlag(false);

	//ターゲットモーションのクリップを初期化する
	//真ん中のターゲットモーション
	targetClipArray[enTargetClip_Normal].Load("Assets/animData/player/rod0.tka");
	targetClipArray[enTargetClip_Normal].SetLoopFlag(false);
	//上のターゲットモーション
	targetClipArray[enTargetClip_Up].Load("Assets/animData/player/rod1.tka");
	targetClipArray[enTargetClip_Up].SetLoopFlag(false);
	//右上のターゲットモーション
	targetClipArray[enTargetClip_UpRight].Load("Assets/animData/player/rod2.tka");
	targetClipArray[enTargetClip_UpRight].SetLoopFlag(false);

	//計算用のアニメーションを初期化する
	for (int i = 0; i < 3; i++)
	{
		//スケルトンをロードする
		calcSkeleton[i].Init("Assets/modelData/player/player.tks");
		//アニメーションにスケルトンとターゲットモーションを渡す
		calcAnim[i].Init(calcSkeleton[i], targetClipArray,enTargetClip_Num);
	}

	//本物のモデル
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
	//カメラのインスタンスを探す
	m_camera = FindGO<GameCamera>("gamecamera");
	//アニメーションのロード
	LoadAnimationClip();
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/player/player.tkm", m_animationClipArray, enAnimClip_Num);
	//大きさを設定
	Vector3 scale = { 2.5f, 2.5f, 2.5f };
	//座標を設定
	m_position = Vector3::Zero;
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(scale);
	//キャラコンを読み込み
	m_charaCon.Init(25.0f, 70.0f, m_position);
	//モデルの更新
	m_modelRender.Update();
	return true;
}

void Player::CalcAngle()
{
	Vector2 originPosition = Vector2::Zero;
	Vector2 angleposition = Vector2::Zero;
	angleposition.y = g_pad[0]->GetRStickYF();
	angleposition.x = g_pad[0]->GetRStickXF();

	
	//X軸のベクトル。
	Vector2 axisX = { 1.0f,0.0f };
	//正規化（長さを１に）
	axisX.Normalize();

	//射影。
	float dot = angleposition.Dot(axisX);

	m_anglePos = angleposition;

	//cos値を求める。
	float angleCos = dot / angleposition.Length();
	
	//入力角度を求める。
	float angle = acos(angleCos);
	angle = Math::RadToDeg(angle);
	
	//入力方向がしためだったら判定角度を反転。
	if (angleposition.y < 0) 
	{
		angle = 360.0f - angle;
	}

	m_angle = angle;
	ChoiceAngleGroup();
}

void Player::ChoiceAngleGroup()
{
	//後で治すので今は適当に
	if (g_pad[0]->GetRStickXF() == 0 &&
		g_pad[0]->GetRStickYF() == 0)
	{
		m_angleGroup = enAngleGroup_No;
	}

	if (m_angle >= 0.0f && m_angle <= 45.0f)
	{
		m_angleGroup = enAngleGroup_1;
	}

	else if (m_angle > 45.0f && m_angle <= 90.0f)
	{
		m_angleGroup = enAngleGroup_2;
	}

	else if (m_angle > 90.0f && m_angle <= 135.0f)
	{
		m_angleGroup = enAngleGroup_3;
	}

	else if (m_angle > 135.0f && m_angle <= 180.0f)
	{
		m_angleGroup = enAngleGroup_4;
	}

	else if (m_angle > 180.0f && m_angle <= 225.0f)
	{
		m_angleGroup = enAngleGroup_5;
	}

	else if (m_angle > 270.0f && m_angle <= 315.0f)
	{
		m_angleGroup = enAngleGroup_6;
	}

	else if (m_angle > 315.0f && m_angle <= 360.0f)
	{
		m_angleGroup = enAngleGroup_7;
		int a = 0;
	}
}

void Player::CalcArea()
{
	Vector2 AxisR = { 0.0f,0.0f };
	Vector2 AxisL = { 0.0f,0.0f };

	switch (m_angleGroup)
	{
	case Player::enAngleGroup_0:
		AxisR = { 1.0f,0.0f };
		AxisL = { 1.0f,1.0f };
		break;
	case Player::enAngleGroup_1:
		AxisR = { 1.0f,1.0f };
		AxisL = { 0.0f,1.0f };
		break;
	case Player::enAngleGroup_2:
		AxisR = { 0.0f,1.0f };
		AxisL = { -1.0f,1.0f };
		break;
	case Player::enAngleGroup_3:
		break;
	case Player::enAngleGroup_4:
		break;
	case Player::enAngleGroup_5:
		break;
	case Player::enAngleGroup_6:
		break;
	case Player::enAngleGroup_7:
		break;
	case Player::enAngleGroup_No:
		break;
	default:
		break;
	}
	//45度から90度の入力の時
	{
		float angle = m_angle;
		//S1の面積
		Vector2 axis0 = { 1.0f,1.0f };
		axis0.Normalize();
		Vector2 d = {0.0f,0.0f};
		d.Cross(axis0,m_anglePos);
		d.x /= 2;
		d.y /= 2;

		//S2の面積
		Vector2 axisX2 = { 0.0f,1.0f };
		axisX2.Normalize();
		Vector2 d2 = { 0.0f,0.0f };
		d2.Cross(m_anglePos, axisX2);
		if (d2.x < 0.0f)
		{
			d2.x *= -1;
		}
		d2.x /= 2;
		if (d2.y < 0.0f)
		{
			d2.y *= -1;
		}
		d2.y /= 2;
	
		//S3の面積
		Vector2 diff;
		diff.x = axis0.x - m_anglePos.x;
		diff.y = axis0.y - m_anglePos.y;

		Vector2 diff2;
		diff2.x = axisX2.x - m_anglePos.x;
		diff2.y = axisX2.y - m_anglePos.y;

		Vector2 d3;
		d3.Cross(diff, diff2);
		d3.x /= 2;
		if (d3.x < 0.0f)
		{
			d3.x *= -1;
		}
		d3.y /= 2;
		if (d3.y < 0.0f)
		{
			d3.y *= -1;
		}

		//Vector2 value;
		//value.x = d.x + d2.x + d3.x;
		//value.y = d.x + d2.y + d3.y;
		int a = 0;
	}
}

void Player::CalcSkeleton()
{
    
	//重さ仮の数値
	float brendWeight[3];
	brendWeight[0] = 0.33f;
	brendWeight[1] = 0.33f;
	brendWeight[2] = 0.33f;

	//最終ボーン計算
	std::unique_ptr<Matrix[]> boneMatrices;
	boneMatrices.reset(new Matrix[skeleton.GetNumBones()]);

	for (int boneNo = 0; boneNo < skeleton.GetNumBones(); boneNo++)
	{
		Matrix boneMat;
		//1個目のスケルトンのボーン行列を取得
		boneMat = calcSkeleton[0].GetBoneMatricesTopAddress()[boneNo];
		//ボーン行列×三角形の重さ
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
		//２個目のボーン行列を取得
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
		//3個目のボーン行列を取得
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

		//重みの足し算
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

		//ボーンを設定
		skeleton.SetBoneLocalMatrix(boneNo, boneMat);
	}
	
}

void Player::Update()
{

	//モーションブレンディング
	CalcSkeleton();
	CalcAngle();


	//本物
	//model.UpdateWorldMatrix(m_position,m_rotation,m_scale);
	skeleton.Update(model.GetWorldMatrix());
	anim.Progress(g_gameTime->GetFrameDeltaTime() * 1.0f);

	//移動処理
	Move();
	//回転処理
	Rotation();
	//アニメーション処理
	PlayAnimation();
	//各ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();
}

void Player::Move()
{
	//移動速度
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//ステックの入力量を取得
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//スティックの入力量×速度	
	m_moveSpeed += cameraForward * lStick_y * 1000.0f;
	m_moveSpeed += cameraRight * lStick_x * 1000.0f;
	m_moveSpeed.y = 0.0f;
	//キャラコンを使用して座標を動かす
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//揺れるので
	//モデルの座標にはカメラの座標を入れる
	Vector3 newposition = m_camera->GetPosition();
	newposition.y = 0.0f;

	//座標を設定する
	m_modelRender.SetPosition(newposition);
}

void Player::Rotation()
{
	//注視点へのベクトルを計算する
	Vector3 vector = m_camera->GetTarget() - m_camera->GetPosition();
	//正規化する
	vector.Normalize();
	//注視点を向く回転を作成
	m_rotation.SetRotationYFromDirectionXZ(vector);
	//回転を設定する
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
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//キャスト準備ステートへ
		m_playerState = enPlayerState_ReadyCast;
		return;
	}
	//移動速度があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f ||
		fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//走りステートに移行する
		m_playerState = enPlayerState_Walk;
		return;
	}

	//移動速度がなかったら
	else
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ProcessIdleStateTransition()
{
	//共通の遷移処理へ
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//共通の遷移処理へ
	ProcessCommonStateTransition();
}

void Player::ProcessCastStateTransition()
{
	//アニメーションが再生中ではなかったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ProcessCommonStateTransition();
		return;
	}
}

void Player::ProcessReadyCastStateTransition()
{
	//アニメーションが再生中ではなかったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//Aボタンが押されていなかったら
		if (g_pad[0]->IsPress(enButtonA) == false)
		{
			//キャストステートへ
			m_playerState = enPlayerState_Cast;
			return;
		}
	}
}

void Player::Render(RenderContext& rc)
{
	//本物の描画
	model.Draw(rc);
	//モデルの描画
	m_modelRender.Draw(rc);

}