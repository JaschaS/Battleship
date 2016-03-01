#include "BattleshipGame.h"
#include "DirectXTK\CommonStates.h"
#include <iostream>
#include "DebugLog.h"

/*	FIT2049 - Assignment 1
*	BattleshipGame.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of Battleship.h.
*/

void printHelp ();

BattleshipGame::BattleshipGame() 
	: 
	Game(),
	m_stateMachine(this, NO_STATE),
	//Screen Width/Height divided by Scale value, keeps the aspect ration intact. 
	//Scale value defines how big the view of the camera is.
	m_currentCamera(1280 / 50, 720 / 50),
	m_aircraftCarrier(1),
	m_battleShip(2),
	m_destroyer(3),
	m_submarine(4),
	m_patrolBoat(5)
{
	m_currentSelection = nullptr;
}

BattleshipGame::~BattleshipGame() {}

bool BattleshipGame::Initialise(Direct3D* renderer, InputController* input)
{
	using namespace DirectX::SimpleMath;

	bool error;

	//Game checks if renderer and input is nullptr
	error = Game::Initialise(renderer, input);
	if (!error) return false;

	//Set the Camera position. z = -0.0001f, if its 0 nothing will rendered...
	m_currentCamera.SetPosition(Vector3(0, 10, -0.00001f));
	m_currentCamera.SetLookAt(Vector3(0, 0, 0));

	m_font72 = std::unique_ptr<DirectX::SpriteFont> (
					new DirectX::SpriteFont (
						renderer->GetDevice (),
						L"Assets/Fonts/pixelfont_72.spritefont"
						)
				);

	m_font60 = std::unique_ptr<DirectX::SpriteFont>(
					new DirectX::SpriteFont(
						renderer->GetDevice(),
						L"Assets/Fonts/pixelfont_60.spritefont"
					)
				);
	m_font24 = std::unique_ptr<DirectX::SpriteFont>(
					new DirectX::SpriteFont(
						renderer->GetDevice(),
						L"Assets/Fonts/pixelfont_24.spritefont"
					)
				);
	m_font18 = std::unique_ptr<DirectX::SpriteFont>(
					new DirectX::SpriteFont(
						renderer->GetDevice(),
						L"Assets/Fonts/pixelfont_13.spritefont"
					)
				);

	m_stateMachine.RegisterState(
		MENU_STATE,
		&BattleshipGame::Menu_OnEnter,
		&BattleshipGame::Menu_OnUpdate,
		&BattleshipGame::Menu_Draw,
		&BattleshipGame::DoNothing_OnExit
	);
	m_stateMachine.RegisterState(
		GAMEPLAY_STATE,
		&BattleshipGame::Gameplay_OnEnter,
		&BattleshipGame::Gameplay_OnUpdate,
		&BattleshipGame::Gameplay_Draw,
		&BattleshipGame::DoNothing_OnExit
	);
	m_stateMachine.RegisterState(
		INIT_STATE,
		&BattleshipGame::DoNothing_OnEnter,
		&BattleshipGame::Init_OnUpdate,
		&BattleshipGame::Init_Draw,
		&BattleshipGame::DoNothing_OnExit
	);
	m_stateMachine.RegisterState(
		LOADING_STATE,
		&BattleshipGame::DoNothing_OnEnter,
		&BattleshipGame::Loading_OnUpdate,
		&BattleshipGame::Loading_Draw,
		&BattleshipGame::DoNothing_OnExit
	);
	m_stateMachine.RegisterState (
		GAMEOVER_STATE,
		&BattleshipGame::DoNothing_OnEnter,
		&BattleshipGame::GameOver_OnUpdate,
		&BattleshipGame::GameOver_Draw,
		&BattleshipGame::DoNothing_OnExit
	);
	m_stateMachine.ChangeState(LOADING_STATE);

	return true;
}

void BattleshipGame::Update(float timestep) 
{
	m_input->BeginUpdate();

	m_currentCamera.Update(timestep);
	m_stateMachine.Update(timestep);

	m_input->EndUpdate();
}

void BattleshipGame::Render()
{
	m_renderer->BeginScene( 30.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f );
	m_renderer->SetCurrentShader(nullptr);

	m_stateMachine.Draw();

	m_renderer->EndScene();
}

void BattleshipGame::Shutdown()
{
	m_meshShader.Release();
	m_texturedShader.Release();
}

//############# PRIVATE METHODS ############
void BattleshipGame::Menu_OnEnter ()
{
	m_playerBoard.Reset ();
	m_enemyBoard.Reset ();
	m_ai.PlaceShips ();
	m_cursor.Reset (DirectX::SimpleMath::Vector3::Zero);
	m_aircraftCarrier.Reset ();
	m_battleShip.Reset ();
	m_destroyer.Reset ();
	m_submarine.Reset ();
	m_patrolBoat.Reset ();
	m_playerTurn = PLAYER;
	m_count = 0;
}

void BattleshipGame::Menu_OnUpdate(float timeStep)
{

	if ( m_input->GetKeyUp('P') )
	{
		m_stateMachine.ChangeState ( INIT_STATE );
	}
	if ( m_input->GetKeyUp ( 'Q' ) )
	{
		PostQuitMessage ( 0 );
	}
	if ( m_input->GetKeyUp ( 'H' ) )
	{
		printHelp ();
	}

	if ( m_input->GetKeyUp ( VK_UP ) )
	{
		m_menu.CursorUp ();
	}
	else
	{
		if ( m_input->GetKeyUp ( VK_DOWN ) )
		{
			m_menu.CursorDown ();
		}
	}

	if ( m_input->GetKeyUp ( VK_SPACE ) || m_input->GetKeyUp(VK_RETURN) )
	{
		int value = m_menu.GetCursor ();

		switch ( value )
		{
		case 0:
			m_stateMachine.ChangeState ( INIT_STATE );
			break;
		case 1:
			printHelp ();
			break;
		case 2:
			PostQuitMessage ( 0 );
			break;
		default:
			break;
		}
	}

	m_menu.Update();
}

void BattleshipGame::Menu_Draw()
{
	DirectX::CommonStates states(m_renderer->GetDevice());
	m_spriteBatch.get()->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	m_menu.Draw();

	m_spriteBatch.get()->End();
}

void BattleshipGame::Gameplay_OnEnter ()
{
	m_cursor.Reset ( DirectX::SimpleMath::Vector3(-7, 0, -2) );
	m_cursor.Visible ( true );
	m_cursor.Valid ( true );
}

void BattleshipGame::Gameplay_OnUpdate(float timeStep)
{
	
	if ( m_playerTurn == PLAYER )
	{
		PlayerTurn ();
	}
	else
	{
		AITurn ();
	}
	
	//Look if someone has won...
	if ( m_playerBoard.ShipsLeft () == 0 || m_enemyBoard.ShipsLeft() == 0 )
	{
		m_stateMachine.ChangeState ( GAMEOVER_STATE );
	}

	if ( m_input->GetKeyUp ( 'M' ) )
	{
		m_stateMachine.ChangeState ( MENU_STATE );
	}

	if ( m_input->GetKeyUp ( 'H' ) )
	{
		MessageBox ( 
			NULL,
			"Find all ships in the right board before the ai find your ships.\n\n Move the white cursor with the arrow keys.\n\n Shoot with spacebar.\n\n Yellow marker indicates a hit. Black a miss.",
			"Help",
			MB_OK 
		);
	}

	m_enemyBoard.Update ( timeStep );
	m_playerBoard.Update(timeStep);
	m_cursor.Update ( timeStep );
	m_aircraftCarrier.Update ( timeStep );
	m_destroyer.Update ( timeStep );
	m_battleShip.Update ( timeStep );
	m_submarine.Update ( timeStep );
	m_patrolBoat.Update ( timeStep );
}

void BattleshipGame::Gameplay_Draw()
{
	m_playerBoard.Render(m_renderer, m_currentCamera);
	m_enemyBoard.Render(m_renderer, m_currentCamera);
	m_cursor.Render ( m_renderer, m_currentCamera );
	m_aircraftCarrier.Render ( m_renderer, m_currentCamera );
	m_destroyer.Render ( m_renderer, m_currentCamera );
	m_battleShip.Render ( m_renderer, m_currentCamera );
	m_submarine.Render ( m_renderer, m_currentCamera );
	m_patrolBoat.Render ( m_renderer, m_currentCamera );

	DirectX::CommonStates states ( m_renderer->GetDevice () );
	m_spriteBatch.get ()->Begin ( DirectX::SpriteSortMode_Deferred, states.NonPremultiplied () );

	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"[M]enu", Vector2 ( 10, 10 ), WHITE );
	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"[H]elp", Vector2 ( 150, 10 ), WHITE );

	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"P1", Vector2 ( 50, 100 ), BLUE );
	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"AI", Vector2 ( 950, 100 ), GREEN );

	std::wstring shipsLeft = L"Ships left: " + std::to_wstring ( m_playerBoard.ShipsLeft() );
	m_font18.get ()->DrawString ( m_spriteBatch.get (), shipsLeft.c_str (), Vector2 ( 100, 100 ), WHITE );

	shipsLeft = L"Ships left: " + std::to_wstring ( m_enemyBoard.ShipsLeft () );
	m_font18.get ()->DrawString ( m_spriteBatch.get (), shipsLeft.c_str (), Vector2 ( 1000, 100 ), WHITE );

	DirectX::SimpleMath::Vector2 stringSize;
	DirectX::SimpleMath::Vector2 textOrigin;

	switch ( m_playerTurn )
	{
	case PLAYER:
		stringSize = m_font60.get ()->MeasureString ( L"P1" );
		textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );

		m_font60.get()->DrawString ( m_spriteBatch.get (), L"P1", Vector2 ( 640, 50 ), BLUE, 0, textOrigin );
		break; 
	case AI:
		stringSize = m_font60.get ()->MeasureString ( L"AI" );
		textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );

		m_font60.get ()->DrawString ( m_spriteBatch.get (), L"AI", Vector2 ( 640, 50 ), GREEN, 0, textOrigin );
		break;
	default:
		break;
	}

	m_spriteBatch.get ()->End ();
}

void BattleshipGame::Init_OnUpdate(float timeStep)
{
	/* 1) Player can press 1-5 to set a ship.
	 * 2) Player can move current selected ship with arrow keys.
	 * 4) If place is !valid -> Ship will rendered red.
	 *
	 * ship_position + ship_length + ship_direction -> position in array.
	 * A place is valid if,
	 * 1) The ship dont collide with an other ship.
	 * 2) The ship is inside the array.
	*/
	m_playerBoard.Update(timeStep);

	/* Use Keydown and else if
	 * Reason:
	 * The user should only select one ship at a time. But if the
	 * user press more the one key (ex. 1 and 2) then 2 ships will
	 * be selected. Keydown and the else will prevent the user to 
	 * do this.
	*/
	if (m_input->GetKeyDown('1'))
	{
		SelectShip ( m_aircraftCarrier );
	}
	else
	{
		if (m_input->GetKeyDown('2'))
		{
			SelectShip ( m_battleShip );
		}
		else 
		{
			if (m_input->GetKeyDown('3'))
			{
				SelectShip ( m_destroyer );
			}
			else
			{
				if (m_input->GetKeyDown('4'))
				{
					SelectShip ( m_submarine );
				}
				else 
				{
					if ( m_input->GetKeyDown ( '5' ) )
					{
						SelectShip ( m_patrolBoat );
					}
				}
			}
		}
	}

	//Move the current selected ship with the arrow keys.
	if (m_currentSelection != nullptr)
	{
		//The ship should move left or right / up or down. But not both at the same time..
		if (m_input->GetKeyUp(VK_LEFT))
		{
			ChangeShipPosition ( 0 );
		}
		else 
		{
			if ( m_input->GetKeyUp ( VK_RIGHT ) )
			{
				ChangeShipPosition ( 1 );
			}
		}

		if (m_input->GetKeyUp(VK_UP))
		{
			ChangeShipPosition ( 2 );
		}
		else 
		{
			if ( m_input->GetKeyUp ( VK_DOWN ) )
			{
				ChangeShipPosition ( 3 );
			}
		}

	}

	//Rotates the current selected ship.
	if ( m_input->GetKeyUp ( 'R' ) && m_currentSelection != nullptr)
	{
		DirectX::SimpleMath::Vector3 position = m_currentSelection->GetPosition ();
		int x = (int) position.x;
		int z = (int) position.z;
		int length = m_currentSelection->GetShipLength ();
		int id = m_currentSelection->GetID ();
		int addArray[2]{ id, FREE };	//Used for add value.
		Direction shipDirection = m_currentSelection->GetDirection ();

		//Clear old position is prev position was valid.
		if ( m_currentSelection->IsValid () )
		{
			m_playerBoard.AddValueToPosition ( x, z, shipDirection, length, &id, FREE, 1 );
		}

		m_currentSelection->Flip();

		shipDirection = m_currentSelection->GetDirection ();

		m_currentSelection->Valid (
			m_playerBoard.AddValueToPosition ( x, z, shipDirection, length, addArray, id, 2 )
		);

	}

	if ( m_input->GetKeyUp ( 'S' ) && m_validShips == 5 )
	{
		m_stateMachine.ChangeState ( GAMEPLAY_STATE );
	}

	if ( m_input->GetKeyUp ( 'M' ) )
	{
		m_stateMachine.ChangeState ( MENU_STATE );
	}

	m_aircraftCarrier.Update(timeStep);
	m_destroyer.Update(timeStep);
	m_battleShip.Update(timeStep);
	m_submarine.Update(timeStep);
	m_patrolBoat.Update(timeStep);

	//Check which ship is valid
	int shipsValid = 0;
	int shipsPlaced = 0;

	if ( m_aircraftCarrier.IsValid () ) ++shipsValid;
	if ( m_destroyer.IsValid () ) ++shipsValid;
	if ( m_battleShip.IsValid () ) ++shipsValid;
	if ( m_submarine.IsValid () ) ++shipsValid;
	if ( m_patrolBoat.IsValid () ) ++shipsValid;

	if ( m_aircraftCarrier.IsPlaced () ) ++shipsPlaced;
	if ( m_destroyer.IsPlaced () ) ++shipsPlaced;
	if ( m_battleShip.IsPlaced () ) ++shipsPlaced;
	if ( m_submarine.IsPlaced () ) ++shipsPlaced;
	if ( m_patrolBoat.IsPlaced () ) ++shipsPlaced;

	m_shipsToPlace = 5 - shipsPlaced;
	m_validShips = shipsValid;
	m_placedShips = shipsPlaced;
}

void BattleshipGame::Init_Draw()
{
	using namespace DirectX::SimpleMath;

	m_playerBoard.Render(m_renderer, m_currentCamera);

	m_aircraftCarrier.Render(m_renderer, m_currentCamera);
	m_destroyer.Render(m_renderer, m_currentCamera);
	m_battleShip.Render(m_renderer, m_currentCamera);
	m_submarine.Render(m_renderer, m_currentCamera);
	m_patrolBoat.Render(m_renderer, m_currentCamera);

	DirectX::CommonStates states(m_renderer->GetDevice());
	m_spriteBatch.get()->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	std::wstring shipsToPlace = L"Ships to place: " + std::to_wstring ( m_shipsToPlace );
	
	m_font18.get ()->DrawString ( m_spriteBatch.get (), shipsToPlace.c_str(), Vector2 ( 50, 50 ), WHITE );

	std::wstring shipsValid = L"Ships valid: " + std::to_wstring ( m_validShips );

	m_font18.get ()->DrawString ( m_spriteBatch.get (), shipsValid.c_str (), Vector2 ( 450, 50 ), WHITE );

	std::wstring shipsPlaced = L"Ships placed: " + std::to_wstring ( m_placedShips );

	m_font18.get ()->DrawString ( m_spriteBatch.get (), shipsPlaced.c_str (), Vector2 ( 850, 50 ), WHITE );



	
	
	m_font18.get()->DrawString(m_spriteBatch.get(), L"---------------------------", Vector2(650, 200), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [1] for Aircraft carrier", Vector2(650, 250), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [2] for Battleship", Vector2(650, 300), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [3] for Destroyer", Vector2(650, 350), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [4] for Submarine", Vector2(650, 400), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [5] for Patrol boat", Vector2(650, 450), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"----------------------------", Vector2(650, 500), WHITE );
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [R] to Rotate", Vector2(650, 550), WHITE );

	if ( m_validShips == 5 )
	{
		m_font18.get ()->DrawString (
			m_spriteBatch.get (),
			L"Press [S] to Start the Game",
			Vector2 ( 650, 600 ),
			WHITE
		);

		m_font18.get ()->DrawString (
			m_spriteBatch.get (),
			L"Ready to play!",
			Vector2 ( 650, 150 ),
			Color ( 37.0f / 255.0f, 190.0f / 255.0f, 13.0f / 255.0f, 1 )
		);
	}
	else
	{
		m_font18.get ()->DrawString (
			m_spriteBatch.get (),
			L"Press [S] to Start the Game",
			Vector2 ( 650, 600 ),
			Color ( 1, 0, 0, 1 )
		);

		m_font18.get ()->DrawString (
			m_spriteBatch.get (),
			L"Place your ships!",
			Vector2 ( 650, 150 ),
			Color ( 0, 0, 0, 1 ) 
		);
	}

	
	m_font18.get()->DrawString(m_spriteBatch.get(), L"Press [M] for Menu", Vector2(650, 650), WHITE );

	m_spriteBatch.get()->End();
}

void BattleshipGame::ChangeState(GameStates state)
{
	m_stateMachine.ChangeState(state);
}

void BattleshipGame::Loading_OnUpdate(float timeStep)
{

	switch (m_count)
	{
	case 0:
		m_error = m_playerBoard.Initialise(
			m_renderer,
			m_meshShader,
			Vector3(6, 0, -1),
			Color(
				47.0f / 255.0f,
				181.0f / 255.0f,
				229.0f / 255.0f,
				1.0f)
			);
		break;
	case 1:
		m_error = m_enemyBoard.Initialise(
			m_renderer,
			m_meshShader,
			Vector3(-6, 0, -1),
			Color(
				255.0f / 255.0f,
				69.0f / 255.0f,
				69.0f / 255.0f,
				1.0f)
			);
		break;
	case 2:
		m_error = m_aircraftCarrier.Initialise(
			m_renderer,
			m_meshShader,
			Vector3::Zero,
			"Assets/Meshes/AirCraftCarrier.obj",
			5
			);
		if ( m_error ) m_aircraftCarrier.Visible(false);
		break;
	case 3:
		m_error = m_battleShip.Initialise(
			m_renderer,
			m_meshShader,
			Vector3::Zero,
			"Assets/Meshes/BattleShip.obj",
			4
			);
		if ( m_error ) m_battleShip.Visible(false);
		break;
	case 4:
		m_error = m_submarine.Initialise(
			m_renderer,
			m_meshShader,
			Vector3::Zero,
			"Assets/Meshes/Submarine.obj",
			3
			);
		if ( m_error ) m_submarine.Visible(false);
		break;
	case 5:
		m_error = m_destroyer.Initialise(
			m_renderer,
			m_meshShader,
			Vector3::Zero,
			"Assets/Meshes/Destroyer.obj",
			3
			);
		if ( m_error ) m_destroyer.Visible(false);
		break;
	case 6:
		m_error = m_patrolBoat.Initialise(
			m_renderer,
			m_meshShader,
			Vector3::Zero,
			"Assets/Meshes/PatrolBoat.obj",
			2
			);
		if ( m_error ) m_patrolBoat.Visible(false);
		break;
	case 7:
		m_error = m_meshShader.Initialise(
			m_renderer->GetDevice(),
			L"Assets/Shaders/VertexShader.vs",
			L"Assets/Shaders/PixelShader.ps"
			);
		break;
	case 8:
		m_error = m_texturedShader.Initialise(
			m_renderer->GetDevice(),
			L"Assets/Shaders/VertexShader.vs",
			L"Assets/Shaders/TexturedPixelShader.ps"
			);
		break;
	case 9:
		m_error = m_menu.Initialise(
			m_renderer,
			m_spriteBatch.get(),
			m_font72.get(),
			m_font18.get(),
			m_input
		);
		break;
	case 10:
		m_error = m_cursor.Initialise (
			m_renderer,
			m_meshShader,
			Vector3::Zero
		);
		if ( m_error )
		{
			m_cursor.Visible ( false );
			m_cursor.Valid ( false );
		}
		break;
	case 11:
		m_error = m_ai.Initialise ( &m_enemyBoard, &m_playerBoard );
		break;
	default:
		break;
	}

	if(m_error) ++m_count;

	if ( m_count == 12)
	{
		m_stateMachine.ChangeState(MENU_STATE);
	}
}

void BattleshipGame::Loading_Draw()
{
	using namespace DirectX::SimpleMath;

	DirectX::CommonStates states(m_renderer->GetDevice());
	m_spriteBatch.get()->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	std::wstring text = L"Loading... (" + std::to_wstring ( 100 / 12 * m_count ) + L"/100)";

	m_font24.get ()->DrawString (
		m_spriteBatch.get (),
		m_error ? text.c_str() : L"An Error occured...",
		Vector2 ( 650, 630 ),
		WHITE
	);

	m_spriteBatch.get()->End();
}

void BattleshipGame::GameOver_Draw ()
{
	DirectX::CommonStates states ( m_renderer->GetDevice () );
	m_spriteBatch.get ()->Begin ( DirectX::SpriteSortMode_Deferred, states.NonPremultiplied () );

	DirectX::SimpleMath::Vector2 stringSize;
	DirectX::SimpleMath::Vector2 textOrigin;

	std::wstring message;

	if ( m_playerBoard.ShipsLeft () == 0 )
	{
		message = L"DEFEATED!";
	}
	else
	{
		message = L"VICTORY!";
	}

	stringSize = m_font60.get ()->MeasureString ( message.c_str () );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );

	m_font60.get ()->DrawString (
		m_spriteBatch.get (),
		message.c_str(),
		Vector2 ( 640, 150 ),
		BROWN,
		0,
		textOrigin
	);

	stringSize = m_font18.get ()->MeasureString ( L"Press Return..." );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );

	m_font18.get ()->DrawString ( 
		m_spriteBatch.get (),
		L"Press Return...",
		Vector2 ( 640, 650 ),
		BLUE,
		0,
		textOrigin
	);

	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"P1:", Vector2 ( 50, 360 ), BLUE );
	m_font18.get ()->DrawString ( m_spriteBatch.get (), L"AI:", Vector2 ( 50, 430 ), GREEN );

	std::wstring shipsLeft = L"Ships left: " + std::to_wstring ( m_playerBoard.ShipsLeft () );
	stringSize = m_font18.get ()->MeasureString ( shipsLeft.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString (
		m_spriteBatch.get (),
		shipsLeft.c_str (),
		Vector2 ( 1280 / 4, 360 ),
		WHITE,
		0,
		textOrigin
	);

	shipsLeft = L"Ships left: " + std::to_wstring ( m_enemyBoard.ShipsLeft () );
	stringSize = m_font18.get ()->MeasureString ( shipsLeft.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString (
		m_spriteBatch.get (),
		shipsLeft.c_str (),
		Vector2 ( 1280 / 4, 430 ),
		WHITE,
		0,
		textOrigin
	);

	std::wstring missed = L"Missed: " + std::to_wstring ( m_enemyBoard.Missed () );
	stringSize = m_font18.get ()->MeasureString ( missed.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString ( 
		m_spriteBatch.get (),
		missed.c_str (),
		Vector2 ( 1280 / 2, 360 ),
		WHITE,
		0,
		textOrigin
	);

	missed = L"Missed: " + std::to_wstring ( m_playerBoard.Missed () );
	stringSize = m_font18.get ()->MeasureString ( missed.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString (
		m_spriteBatch.get (),
		missed.c_str (),
		Vector2 ( 1280 / 2, 430 ),
		WHITE,
		0,
		textOrigin
	);

	std::wstring hit = L"Hits: " + std::to_wstring ( m_enemyBoard.Hits() );
	stringSize = m_font18.get ()->MeasureString ( hit.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString (
		m_spriteBatch.get (),
		hit.c_str (),
		Vector2 ( 1280 / 4 * 3, 360 ),
		WHITE,
		0,
		textOrigin
	);

	hit = L"Hits: " + std::to_wstring ( m_playerBoard.Hits () );
	stringSize = m_font18.get ()->MeasureString ( hit.c_str() );
	textOrigin = Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font18.get ()->DrawString (
		m_spriteBatch.get (),
		hit.c_str (),
		Vector2 ( 1280 / 4 * 3, 430 ),
		WHITE,
		0,
		textOrigin
	);

	m_spriteBatch.get ()->End ();
}

void BattleshipGame::GameOver_OnUpdate ( float timeStep )
{
	if ( m_input->GetKeyDown ( VK_RETURN ) )
	{
		m_stateMachine.ChangeState ( MENU_STATE );
	}
}

void BattleshipGame::SelectShip ( Ship &ship )
{
	//Check if the current selection is not the aircraftcarrier
	if ( m_currentSelection == nullptr || m_currentSelection->GetID () != ship.GetID () )
	{
		m_currentSelection = &ship;

		//Only set the ship to the start position, if it wasn't 
		//set before.
		if ( !ship.IsPlaced () )
		{
			ship.SetPosition ( DirectX::SimpleMath::Vector3 ( 6, 0, -1 ) );
			ship.Visible ( true );
			ship.Placed ( true );

			int addArray[2]{ FREE, ship.GetID () };

			if ( m_playerBoard.AddValueToPosition (
						6, 
						-1,
						ship.GetDirection (),
						ship.GetShipLength (),
						addArray,
						ship.GetID(),
						2
				)
			)
			{
				ship.Valid ( true );
			}
		}

	}
}

void BattleshipGame::ChangeShipPosition ( int values )
{
	DirectX::SimpleMath::Vector3 position = m_currentSelection->GetPosition ();
	int x = (int) position.x;
	int z = (int) position.z;
	int oldX = x;
	int oldZ = z;
	int length = m_currentSelection->GetShipLength ();
	int id = m_currentSelection->GetID ();
	int addArray[2]{ id, FREE };	//Used for add value.
	Direction shipDirection = m_currentSelection->GetDirection ();

	switch ( values )
	{
	case 0:
		++position.x;
		++x;
		break;
	case 1:
		--position.x;
		--x;
		break;
	case 2:
		++position.z;
		++z;
		break;
	case 3:
		--position.z;
		--z;
		break;
	default:
		//do nothing
		break;
	}

	//Check if new position is valid.
	if ( !m_playerBoard.IsPositionValid ((int)position.x, (int)position.z, nullptr, 0) ) return;

	//Clear the old position only if the prev position was valid
	if(m_currentSelection->IsValid() ) 
	{
		m_playerBoard.AddValueToPosition ( oldX, oldZ, shipDirection, length, &id, FREE, 1 );
	}

	if ( m_playerBoard.AddValueToPosition ( x, z, shipDirection, length, addArray, id, 2 ) )
	{

		if ( !m_currentSelection->IsValid () )
		{
			m_currentSelection->Valid ( true );
		}

	}
	else
	{
		m_currentSelection->Valid ( false );
	}

	m_currentSelection->SetPosition ( position );

}

void BattleshipGame::PlayerTurn ()
{
	using namespace DirectX::SimpleMath;

	Vector3 position = m_cursor.GetPosition ();

	//The cursor should move left or right / up or down. But not both at the same time..
	if ( m_input->GetKeyUp ( VK_LEFT ) )
	{
		++position.x;
	}
	else
	{
		if ( m_input->GetKeyUp ( VK_RIGHT ) )
		{
			--position.x;
		}
	}

	if ( m_input->GetKeyUp ( VK_UP ) )
	{
		++position.z;
	}
	else
	{
		if ( m_input->GetKeyUp ( VK_DOWN ) )
		{
			--position.z;
		}
	}

	if ( m_enemyBoard.IsPositionValid ( (int) position.x, (int) position.z, nullptr, 0 ) )
	{
		m_cursor.SetPosition ( position );
		m_cursor.Valid ( true );
	}
	else
	{
		m_cursor.Valid ( false );
	}

	if ( m_cursor.IsValid () && m_input->GetKeyUp ( VK_SPACE ) )
	{
		if ( m_enemyBoard.SetMarker ( (int) position.x, (int) position.z ) )
		{
			m_cursor.Visible ( false );
			m_playerTurn = AI;
		}

	}

	if ( m_input->GetKeyDown ( 'J' ) )
	{
		m_enemyBoard.PrintBoard ();
	}
}

void BattleshipGame::AITurn ()
{
	m_ai.Turn ();
	m_playerTurn = PLAYER;
	m_cursor.Visible ( true );
}

void printHelp ()
{
	//Source: https://en.wikipedia.org/wiki/Battleship_(game)
	std::string text = "The game is played on four grids, two for each player.The grids are typically square – usually 10×10 – and the individual squares in the grid are identified by letter and number.[7] On one grid the player arranges ships and records the shots by the opponent.On the other grid the player records his / her own shots.";
	std::string text2 = "Before play begins, each player secretly arranges their ships on their primary grid. Each ship occupies a number of consecutive squares on the grid, arranged either horizontally or vertically. The number of squares for each ship is determined by the type of the ship. The ships cannot overlap (i.e., only one ship can occupy any given square in the grid). The types and numbers of ships allowed are the same for each player. These may vary depending on the rules.";
	std::string text3 = "There are two typical complements of ships, as given in the Milton Bradley version of the rules:";
	std::string text4 = "Aircraft carrier	 - Size: 5\n";
	std::string text5 = "BattleShip		  - Size: 4\n";
	std::string text6 = "Destroyer		  - Size: 3\n";
	std::string text7 = "Submarine	- Size: 3\n";
	std::string text8 = "Patrol Boat	  - Size: 2\n";
	std::string text9 = "After the ships have been positioned, the game proceeds in a series of rounds. In each round, each player takes a turn to announce a target square in the opponent's grid which is to be shot at. The opponent announces whether or not the square is occupied by a ship, and if it is a miss, the player marks their primary grid with a white peg; if a hit they mark this on their own primary grid with a red peg. The attacking player notes the hit or miss on their own tracking grid with the appropriate color peg (red for hit, white for miss), in order to build up a picture of the opponent's fleet. When all of the squares of a ship have been hit, the ship is sunk, and the ship's owner announces this (e.g. You sank my battleship!). If all of a player\'s ships have been sunk, the game is over and their opponent wins.";
	std::string text10 = "More informations under https://en.wikipedia.org/wiki/Battleship_(game)";

	std::string all = text + "\n\n\n" + text2 + "\n\n" + text3 + "\n" + text4 + text5 + text6 + text7 + text8 + "\n" + text9 + "\n\n" + text10;
	MessageBox ( NULL, all.c_str (), "Help", MB_OK );
}
