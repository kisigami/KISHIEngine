#pragma once

class Player;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;

	Player* m_player = nullptr;
};

