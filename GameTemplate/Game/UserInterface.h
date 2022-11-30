#pragma once

class Player;

class UserInterface:public IGameObject
{
public:
	UserInterface();
	~UserInterface();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_castGaugeSprite;
	SpriteRender m_castGaugeButtomSprite;
	SpriteRender m_castGaugeFrameSprite;
	Player* m_player = nullptr;
};

