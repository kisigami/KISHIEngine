#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {

	DirectionLight g_directionLight;

	DirectionLight::DirectionLight()
	{
		//�f�B���N�V�������C�g����
		m_light.ligDirection.x = 0.0f;
		m_light.ligDirection.y = -1.0f;
		m_light.ligDirection.z = 1.0f;
		//���K��
		m_light.ligDirection.Normalize();

		//�f�B���N�V�������C�g�J���[
		m_light.ligColor.x = 0.5f;
		m_light.ligColor.y = 0.5f;
		m_light.ligColor.z = 0.5f;

		m_light.eyePos = Vector3({ 0.0f, 400.0f, -500.0f });

		//�A���r�G���g���C�g����
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