
#ifndef GAME_H
#define GAME_H

#include "InputController.h"
#include "Direct3D.h"
#include "DirectXTK\SpriteBatch.h"

#include <memory>

class Game
{
protected:
	/*enum GameStates
	{
		NO_STATE = 0,
		MENU_STATE,
		GAMEPLAY_STATE,
		GAMEOVER_STATE,
		STATE_COUNT
	};*/

	//StateMachine<GameStates, Game> *m_stateMachine;

	Direct3D* m_renderer;		//Our renderer is passed in when the Game is created
	InputController* m_input;	//

	//Camera m_currentCamera;		//Our game has a Camera to see everything with!
	//Shader* m_shader;			//We also need a shader to draw things with, later we will probably have multiple shaders
	//TexturedShader* m_texturedShader;	//
	
	//std::vector<GameObject*> m_gameObjects;

	//Mesh* m_gameBoard;			//This mesh is for the game board
	//Mesh* m_battleShip;			//This mesh is for the ship object
	//Matrix m_boardWorld;		//This matrix is the world matrix for the game board
	//Matrix m_battleShipWorld;	//This matrix is the world matrix for the ship
	//float m_rotation;			//This value controls the rotation of the ship

	//Texture* m_shipTexture;		//This will store the texture for our ship
	//Texture* m_buttonTexture;	//This will store the texture for our button

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;	//The sprite batch can be used to draw things in 2D screen space, we can draw textures and text
	//SpriteFont* m_arial;		//If we want to draw text then need a font to draw it with, a Sprite Font can do that!

	//Button* m_button;			//This is a pointer to our button, you could create as many of these as you need

	//void ButtonMethod();		//This method is just here so the button has something to call, you could call or do anything you want with the button

	//void Menu_OnEnter();
	//void Menu_OnUpdate(float timeStep);
	//void Menu_Draw();
	//void Menu_OnExit();

	/*void Gameplay_OnEnter();
	void Gameplay_OnUpdate(float timeStep);
	void Gameplay_Draw();
	void Gameplay_OnExit();

	void GameOver_OnEnter();
	void GameOver_OnUpdate(float timeStep);
	void GameOver_Draw();
	void GameOver_OnExit();*/


public:
	Game();
	virtual ~Game();	//Destructor

	bool Initialise(Direct3D* renderer, InputController* input);	//The initialise method loads all of the content for the game (meshes, textures, etc.)

	virtual void Update(float timestep) = 0;	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	virtual void Render() = 0;					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	virtual void Shutdown() = 0;		//Cleanup everything we initialised
}; 

template <class T> void Delete(T* object)
{
	if (object != nullptr)
	{
		delete object;
		object = nullptr;
	}
}

template <class T> void ReleaseAndDelete(T* object)
{
	if (object != nullptr)
	{
		object->Release();
		delete object;
		object = nullptr;
	}
}

#endif