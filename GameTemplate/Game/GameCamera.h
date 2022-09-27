#pragma once

class Player;

class GameCamera: public IGameObject
{
public:
	bool Start();
	void Update();

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	Player* m_player = nullptr;
	Vector3 m_position;
	Vector3 m_target;
};

