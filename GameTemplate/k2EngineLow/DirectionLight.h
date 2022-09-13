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
		};
		
		
	public:
		DirectionLight();
		~DirectionLight();

		void Update();

		void SetLigDirection(const Vector3& lig)
		{
			m_directionLight.ligDirection = lig;
		}

		void SetLigColor(const Vector3& color)
		{
			m_directionLight.ligColor = color;
		}

	private:
		DirLight m_directionLight;
	};

	extern DirectionLight g_directionLight;
}