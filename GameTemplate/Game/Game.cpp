#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_backGround = NewGO<BackGround>(0, "background");
	m_player = NewGO<Player>(0, "player");

	

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}
