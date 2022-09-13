#include "stdafx.h"
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm", nullptr, 0, enModelUpAxisZ);
	return true;
}

void Game::Update()
{
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
