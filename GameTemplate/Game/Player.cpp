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
	m_animationClipArray[enAnimClip_Fit].Load("Assets/animData/player/fit.tka");
	m_animationClipArray[enAnimClip_Fit].SetLoopFlag(false);

	//ターゲットモーションのクリップを初期化する
	//真ん中のターゲットモーション
	targetClipArray[enTargetClip_Normal].Load("Assets/animData/player/rodnormal.tka");
	targetClipArray[enTargetClip_Normal].SetLoopFlag(false);
	//上のターゲットモーション
	targetClipArray[enTargetClip_Up].Load("Assets/animData/player/rodup.tka");
	targetClipArray[enTargetClip_Up].SetLoopFlag(false);
	//右上のターゲットモーション
	targetClipArray[enTargetClip_UpRight].Load("Assets/animData/player/rodupright.tka");
	targetClipArray[enTargetClip_UpRight].SetLoopFlag(false);
	//右のターゲットモーション
	targetClipArray[enTargetClip_Right].Load("Assets/animData/player/rodright.tka");
	targetClipArray[enTargetClip_Right].SetLoopFlag(false);
	//右下のターゲットモーション
	targetClipArray[enTargetClip_DownRight].Load("Assets/animData/player/roddownright.tka");
	targetClipArray[enTargetClip_DownRight].SetLoopFlag(false);
	//下のターゲットモーション
	targetClipArray[enTargetClip_Down].Load("Assets/animData/player/roddown.tka");
	targetClipArray[enTargetClip_Down].SetLoopFlag(false);
	//左下のターゲットモーション
	targetClipArray[enTargetClip_DownLeft].Load("Assets/animData/player/roddownleft.tka");
	targetClipArray[enTargetClip_DownLeft].SetLoopFlag(false);
	//左のターゲットモーション
	targetClipArray[enTargetClip_Left].Load("Assets/animData/player/rodleft.tka");
	targetClipArray[enTargetClip_Left].SetLoopFlag(false);
	//左上のターゲットモーション
	targetClipArray[enTargetClip_UpLeft].Load("Assets/animData/player/rodupleft.tka");
	targetClipArray[enTargetClip_UpLeft].SetLoopFlag(false);

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
	
	//角度でグループ分け処理
	ChoiceAngleGroup();
}

void Player::ChoiceAngleGroup()
{
	//スティックの入力が無かったら
	if (g_pad[0]->GetRStickXF() == 0 &&
		g_pad[0]->GetRStickYF() == 0)
	{
		m_angleGroup = enAngleGroup_No;
	}
	//0度から45度だったら
	if (m_angle >= 0.0f && m_angle <= 45.0f)
	{
		m_angleGroup = enAngleGroup_0;
	}
	//45度から90度だったら
	else if (m_angle > 45.0f && m_angle <= 90.0f)
	{
		m_angleGroup = enAngleGroup_1;
	}
	//91度から135度だったら
	else if (m_angle > 90.0f && m_angle <= 135.0f)
	{
		m_angleGroup = enAngleGroup_2;
	}
	//136度から180度だったら
	else if (m_angle > 135.0f && m_angle <= 180.0f)
	{
		m_angleGroup = enAngleGroup_3;
	}
	//181度から225度だったら
	else if (m_angle > 180.0f && m_angle <= 225.0f)
	{
		m_angleGroup = enAngleGroup_4;
	}
	//226度から270度だったら
	else if (m_angle > 225.0f && m_angle <= 270.0f)
	{
		m_angleGroup = enAngleGroup_5;
	}
	//271度から315度だったら
	else if (m_angle > 270.0f && m_angle <= 315.0f)
	{
		m_angleGroup = enAngleGroup_6;
	}
	//316度から360度だったら
	else if (m_angle > 315.0f && m_angle <= 360.0f)
	{
		m_angleGroup = enAngleGroup_7;
	}
	
	//面積の計算処理
	CalcArea();
}

void Player::CalcArea()
{
	//面積を計算するための軸を定義
	Vector2 axis1 = { 0.0f,0.0f };
	Vector2 axis2 = { 0.0f,0.0f };

	//面積を計算するための軸を選択
	switch (m_angleGroup)
	{
		//0度から45度の時
	case Player::enAngleGroup_0:
		//真右の軸
		axis1 = { 1.0f,0.0f };
		//右上の軸
		axis2 = { 1.0f,1.0f };
		break;
		//46度から90度の時
	case Player::enAngleGroup_1:
		//右上の軸
		axis1 = { 1.0f,1.0f };
		//真上の軸
		axis2 = { 0.0f,1.0f };
		break;
		//91度から135度の時
	case Player::enAngleGroup_2:
		//真上の軸
		axis1 = { 0.0f,1.0f };
		//左上の軸
		axis2 = { -1.0f,1.0f };
		break;
		//135度から180度の時
	case Player::enAngleGroup_3:
		//左上の軸
		axis1 = { -1.0f,1.0f };
		//真左の軸
		axis2 = { -1.0f,0.0f };
		break;
		//181度から225度の時
	case Player::enAngleGroup_4:
		//真左の軸
		axis1 = { -1.0f,0.0f };
		//左下の軸
		axis2 = { -1.0f,-1.0f };
		break;
		//226度から270度の時
	case Player::enAngleGroup_5:
		//左下の軸
		axis1 = { -1.0f,-1.0f };
		//真下の軸
		axis2 = { 0.0f,-1.0f };
		break;
		//271度から315度の時
	case Player::enAngleGroup_6:
		//真下の軸
		axis1 = { 0.0f,-1.0f };
		//右下の軸
		axis2 = { 1.0f,-1.0f };
		break;
		//316度から360度の時
	case Player::enAngleGroup_7:
		//右下の軸
		axis1 = { 1.0f,-1.0f };
		//真右の軸
		axis2 = { 1.0f,0.0f };
		break;
		//選択してないとき
	case Player::enAngleGroup_No:
		//何もしない
		break;
	}

	float angle = m_angle;

	//S1の面積(右側の三角形)
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

	//S2の面積
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

	//S3の面積
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

	//モーションブレンディング
	CalcSkeleton();
}

void Player::CalcSkeleton()
{
    //グループで再生するアニメーション分ける
	switch (m_angleGroup)
	{
	case Player::enAngleGroup_0:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Right);
		calcAnim[2].Play(enTargetClip_UpRight);
		break;
	case Player::enAngleGroup_1:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_UpRight);
		calcAnim[2].Play(enTargetClip_Up);
		break;
	case Player::enAngleGroup_2:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Up);
		calcAnim[2].Play(enTargetClip_UpLeft);
		break;
	case Player::enAngleGroup_3:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_UpLeft);
		calcAnim[2].Play(enTargetClip_Left);
		break;
	case Player::enAngleGroup_4:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Left);
		calcAnim[2].Play(enTargetClip_DownLeft);
		break;
	case Player::enAngleGroup_5:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_DownLeft);
		calcAnim[2].Play(enTargetClip_Down);
		break;
	case Player::enAngleGroup_6:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_Down);
		calcAnim[2].Play(enTargetClip_DownRight);
		break;
	case Player::enAngleGroup_7:
		calcAnim[0].Play(enTargetClip_Normal);
		calcAnim[1].Play(enTargetClip_DownRight);
		calcAnim[2].Play(enTargetClip_Right);
		break;
	case Player::enAngleGroup_No:
		break;
	}

	calcAnim[0].Progress(1 / 60.0f);
	calcAnim[1].Progress(1 / 60.0f);
	calcAnim[2].Progress(1 / 60.0f);

	//重さ仮の数値
	float brendWeight[3];
	if (m_angleGroup != enAngleGroup_No)
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

	//最終ボーン計算
	std::unique_ptr<Matrix[]> boneMatrices;
	boneMatrices.reset(new Matrix[skeleton.GetNumBones()]);

	for (int boneNo = 0; boneNo < skeleton.GetNumBones(); boneNo++)
	{
		Matrix boneMat;
		//1個目のスケルトンのボーン行列を取得
		boneMat = calcSkeleton[0].GetBone(boneNo)->GetLocalMatrix();
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
		//3個目のボーン行列を取得
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

		skeleton.SetBoneLocalMatrix(boneNo, boneMat);
	}
}

void Player::Update()
{
	if (m_playerState == enPlayerState_Fishing)
	{
		CalcAngle();
		skeleton.SetMarkPlayAnimation();
		skeleton.Update(m_modelRender.GetWorldMatrix());
	}
	//移動処理
	Move();
	//回転処理
	Rotation();
	//アニメーション処理
	PlayAnimation();
	//各ステートの遷移処理
	ManageState();
	CalcCastPower();
	//モデルの更新
	m_modelRender.Update();

	if (m_playerState == enPlayerState_Fishing)
	{
		m_modelRender.CopySkeleton(skeleton);
	}
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

	//カメラ固定中ではなかったら
	if (m_playerState == enPlayerState_Fishing)
	{
		//モデルの座標にキャラコンの座標を入れる
		m_modelRender.SetPosition(m_position);
	}
	else
	{
		//モデルの座標にはカメラの座標を入れる
		Vector3 newposition = m_camera->GetPosition();
		newposition.y = 0.0f;
		m_modelRender.SetPosition(newposition);
	}
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
	case enPlayerState_Fit:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimClip_Fit, 0.3f);
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
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonRB2))
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
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//キャスト準備ステートへ
			m_playerState = enPlayerState_Fit;
			return;
		}
		return;
	}
}

void Player::ProcessReadyCastStateTransition()
{
	////アニメーションが再生中ではなかったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//Aボタンが押されていなかったら
		if (g_pad[0]->IsPress(enButtonRB2) == false)
		{
			//キャストステートへ
			m_playerState = enPlayerState_Cast;
			return;
		}
	}
}

void Player::ProcessFitStateTransition()
{
	//アニメーションが再生中ではなかったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//キャストステートへ
		m_playerState = enPlayerState_Fishing;
		return;
	}
}

void Player::ProcessFishingStateTransition()
{
	//アニメーションが再生中ではなかったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		return;
	}
}
void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
}