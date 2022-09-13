#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {

	DirectionLight g_directionLight;

	DirectionLight::DirectionLight()
	{
		m_directionLight.ligDirection.x = 0.0f;
		m_directionLight.ligDirection.y = 1.0f;
		m_directionLight.ligDirection.z = 1.0f;

		m_directionLight.ligDirection.Normalize();

		m_directionLight.ligColor.x = 1.5f;
		m_directionLight.ligColor.y = 1.5f;
		m_directionLight.ligColor.z = 1.5f;
	}

	DirectionLight::~DirectionLight()
	{

	}

	void DirectionLight::Update()
	{

	}
}