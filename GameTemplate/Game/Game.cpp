#include "stdafx.h"
#include "Game.h"

#include "Player.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}
