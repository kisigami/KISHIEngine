#pragma once

namespace nsK2EngineLow {

	class DirectionLight
	{
	public:
		struct DirLight
		{
			Vector3 ligDirection;
			float pad;
			Vector3 ligColor;
			float pad1;
			Vector3 ambentLight;
		};

	public:
		DirectionLight();
		~DirectionLight();

		void Update();

		void SetLigDirection(const Vector3& lig)
		{
			m_light.ligDirection = lig;
		}

		void SetLigColor(const Vector3& color)
		{
			m_light.ligColor = color;
		}

	private:
		DirLight m_light;
	};

	extern DirectionLight g_directionLight;
}