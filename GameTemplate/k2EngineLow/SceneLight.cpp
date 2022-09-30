#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	SceneLight g_sceneLight;

	void SceneLight::Init()
	{
		//�f�B���N�V�������C�g�̕���
		m_light.directionlight.ligDirection.x =  0.0f;
		m_light.directionlight.ligDirection.y = -1.0f;
		m_light.directionlight.ligDirection.z = -1.0f;
		m_light.directionlight.ligDirection.Normalize();

		//�f�B���N�V�������C�g�̃J���[
		m_light.directionlight.ligColor.x = 1.5f;
		m_light.directionlight.ligColor.y = 1.5f;
		m_light.directionlight.ligColor.z = 1.5f;

		//���_
		m_light.eyePos = g_camera3D->GetPosition();

		//����
		m_light.ambientLight.x = 0.1f;
		m_light.ambientLight.y = 0.1f;
		m_light.ambientLight.z = 0.1f;
	}

	void SceneLight::Update()
	{
	}
}