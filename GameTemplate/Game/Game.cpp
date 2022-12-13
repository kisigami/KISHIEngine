#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "UserInterface.h"
#include "Fish.h"

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
	m_ui = NewGO<UserInterface>(0,"userinterface");
	m_fish = NewGO<Fish>(0, "fish");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}
