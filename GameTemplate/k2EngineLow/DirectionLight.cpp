#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {

	DirectionLight g_directionLight;

	DirectionLight::DirectionLight()
	{
		//ディレクションライト方向
		m_light.ligDirection.x = 0.0f;
		m_light.ligDirection.y = -1.0f;
		m_light.ligDirection.z = 1.0f;
		//正規化
		m_light.ligDirection.Normalize();

		//ディレクションライトカラー
		m_light.ligColor.x = 0.5f;
		m_light.ligColor.y = 0.5f;
		m_light.ligColor.z = 0.5f;

		m_light.eyePos = Vector3({ 0.0f, 400.0f, -500.0f });

		//アンビエントライト環境光
		m_light.ambentLight.x = 0.3f;
		m_light.ambentLight.y = 0.3f;
		m_light.ambentLight.z = 0.3f;
	}

	DirectionLight::~DirectionLight()
	{

	}

	void DirectionLight::Update()
	{

	}
}