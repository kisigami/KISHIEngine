#pragma once
#include "C:\Users\kbc21\OneDrive\�f�X�N�g�b�v\KISHIEngine\GameTemplate\k2EngineLow\gameObject\IGameobject.h"
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

};

