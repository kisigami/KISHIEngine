#pragma once

class Player;
class BackGround;
class GameCamera;
class UserInterface;
class Fish;

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
	SpriteRender m_spriteRender;

	Player* m_player = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	UserInterface* m_ui = nullptr;
	Fish* m_fish;
};

