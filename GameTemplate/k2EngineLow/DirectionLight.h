#pragma once

namespace nsK2EngineLow {

	struct DirLight
	{
		Vector3 ligDirection;
		float pad;
		Vector3 ligColor;
		float pad0;
		Vector3 eyePos;
		float pad1;
		Vector3 ambentLight;
	};

	class DirectionLight0 : public Noncopyable
	{
	public:

	public:
		DirectionLight0();
		~DirectionLight0();

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
}