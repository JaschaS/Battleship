/*	FIT2049 - Example Code
*	Camera.cpp
*	Created by Elliott Wilson - 2015 - Monash Univeristy
*	Implementation of Camera.h
*/

#include "Game.h"
#include "DirectXTK/CommonStates.h"
#include "SpinningObject.h"
#include "StaticObject.h"

Game::Game() 
{
	m_renderer = nullptr;
	m_input = nullptr;
}

Game::~Game()
{
}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{

	if (renderer == nullptr) return false;
	if (input == nullptr) return false;

	m_renderer = renderer;
	m_input = input;

	m_spriteBatch = std::unique_ptr<DirectX::SpriteBatch>(
		new DirectX::SpriteBatch(renderer->GetDeviceContext())	
	);
	

	return true;
}