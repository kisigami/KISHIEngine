#pragma once

class Player;

class GameCamera: public IGameObject
{
public:
	bool Start();
	void Update();

	const Vector3& GetPosition() const
	{
		return m_cameraPosition;
	}
	const Vector3& GetTarget() const
	{
		return m_targetPosition;
	}

	Vector3 m_toCameraPos;
	Vector3 m_toCameraPosOld;
	Vector3 m_targetPosition;
	Vector3 m_cameraPosition;
	Player* m_player = nullptr;
	Vector3 m_position;
	Vector3 m_target;
};

