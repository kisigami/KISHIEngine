#pragma once

namespace nsK2EngineLow
{
	struct Directionlight
	{
		Vector3 ligDirection;
		float pad;
		Vector3 ligColor;
		float pad3;
	};

	struct Light
	{
		Directionlight directionlight;

		Vector3 eyePos;
		float pad1;
		Vector3 ambientLight;
	};

	class SceneLight
	{
	public:
		SceneLight() {};
		~SceneLight() {};
		void Init();
		void Update();

		void SetLigDirection(const Vector3& lig)
		{
			m_light.directionlight.ligDirection = lig;
		}

		void SetLigColor(const Vector3& color)
		{
			m_light.directionlight.ligColor = color;
		}

		void SetAmbientLihgt(const Vector3& lig)
		{
			m_light.ambientLight = lig;
		}

		void SetEyePos(const Vector3& lig)
		{
			m_light.eyePos = lig;
		}

		Light& GetLight()
		{
			return m_light;
		};

	private:
		Light          m_light;
	};

	extern SceneLight g_sceneLight;
};