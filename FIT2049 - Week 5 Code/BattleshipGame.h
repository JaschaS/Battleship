#ifndef BATTLE_SHIP_GAME_H
#define BATTLE_SHIP_GAME_H

/*	FIT2049 - Assignment 1
*	BattleshipGame.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Game class of the battleship game.
*/

#include "Game.h"
#include "StateMachine.h"
#include "Shader.h"
#include "TexturedShader.h"
#include "DirectXTK\SpriteFont.h"
#include "OrthogonalCamera.h"
#include "GameBoard.h"
#include "GameMenu.h"
#include "Ship.h"
#include "Cursor.h"
#include "AIPlayer.h"
#include "Colors.h"

#include <memory>
#include <vector>


class BattleshipGame : public Game
{
private:
	enum GameStates
	{
		NO_STATE = 0,
		LOADING_STATE,
		MENU_STATE,
		INIT_STATE,
		GAMEPLAY_STATE,
		GAMEOVER_STATE
	};

	StateMachine<GameStates, BattleshipGame> m_stateMachine;

	OrthogonalCamera m_currentCamera;
	std::unique_ptr<DirectX::SpriteFont> m_font60;
	std::unique_ptr<DirectX::SpriteFont> m_font72;
	std::unique_ptr<DirectX::SpriteFont> m_font24;
	std::unique_ptr<DirectX::SpriteFont> m_font18;

	Shader m_meshShader;
	TexturedShader m_texturedShader;

	GameMenu m_menu;
	GameBoard m_playerBoard;
	GameBoard m_enemyBoard;
	Cursor m_cursor;
	AIPlayer m_ai;

	Ship m_battleShip;
	Ship m_aircraftCarrier;
	Ship m_destroyer;
	Ship m_patrolBoat;
	Ship m_submarine;
	Ship* m_currentSelection;
	
	void DoNothing_OnEnter() {}
	void DoNothing_OnUpdate(float timeStep) {}
	void DoNothing_OnExit() {}

	void Menu_OnEnter ();
	void Menu_OnUpdate(float timeStep);
	void Menu_Draw();

	//Indecates which players can shoot.
	enum Turn { PLAYER = 0, AI};
	Turn m_playerTurn{ PLAYER };

	void Gameplay_OnEnter ();
	void Gameplay_OnUpdate(float timeStep);
	void Gameplay_Draw();
	void PlayerTurn ();
	void AITurn ();

	void GameOver_Draw ();
	void GameOver_OnUpdate ( float timeStep );

	int m_shipsToPlace{ 5 };
	int m_validShips{ 0 };
	int m_placedShips{ 0 };
	void Init_OnUpdate(float timeStep);
	void Init_Draw();

	//##### LOADING #####
	/**
	 * Error flag if something could't be initialised.
	*/
	bool m_error{ false };

	/**
	 * Progression of the loadingscreen.
	*/
	int m_count{ 0 };
	void Loading_OnUpdate(float timeStep);
	void Loading_Draw();
	
	void ChangeState(GameStates state);
	void SelectShip (Ship &ship);

	/**
	 * Changes the position of the ship.
	 * @values defines the move direction of the ship.
	 * 0 -> (x+1) ship goes left
	 * 1 -> (x-1) ship goes right
	 * 2 -> (z+1) ship goes up
	 * 3 -> (z-1) ship goes down
	*/
	void ChangeShipPosition (int values);
public:
	BattleshipGame();
	virtual ~BattleshipGame();

	bool Initialise(Direct3D* renderer, InputController* input);

	void Update(float timestep);
	void Render();
	void Shutdown();
};

#endif