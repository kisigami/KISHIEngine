#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {

	DirectionLight g_directionLight;

	DirectionLight::DirectionLight()
	{
		//�f�B���N�V�������C�g����
		m_light.ligDirection.x = 0.0f;
		m_light.ligDirection.y = 1.0f;
		m_light.ligDirection.z = 1.0f;
		//���K��
		m_light.ligDirection.Normalize();
		//�f�B���N�V�������C�g�̃J���[
		m_light.ligColor.x = 0.7f;
		m_light.ligColor.y = 0.7f;
		m_light.ligColor.z = 0.7f;

		//�A���r�G���g���C�g����
		m_light.ambentLight.x = 0.5f;
		m_light.ambentLight.y = 0.5f;
		m_light.ambentLight.z = 0.5f;
	}

	DirectionLight::~DirectionLight()
	{

	}

	void DirectionLight::Update()
	{

	}
}