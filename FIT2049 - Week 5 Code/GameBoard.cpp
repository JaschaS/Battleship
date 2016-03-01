#include "GameBoard.h"
#include "DebugLog.h"

/*	FIT2049 - Assignment 1
*	GameBoard.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of GameBoard.h
*/

void InitialiseVectors (
	std::vector<int> &x,
	std::vector<int> &z,
	Direction shipDirection,
	int shipLength,
	int startX,
	int startZ
);

GameBoard::GameBoard() {}

GameBoard::~GameBoard() 
{
	m_boardMesh.Release();
	m_textPlaneMesh.Release();
	m_hitMarkerMesh.Release();
	m_waterMarkerMesh.Release();
}

bool GameBoard::Initialise(
	Direct3D *renderer,
	Shader &meshShader,
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Color boardColor
	)
{
	using namespace DirectX::SimpleMath;

	if (renderer == nullptr) return false;

	bool error;

	error = m_boardMesh.Initialise(renderer, "Assets/Meshes/Board.obj", boardColor, nullptr, &meshShader);
	
	//Board mesh couldn't be initialise...
	if (!error) return false;

	error = m_textPlaneMesh.Initialise(
				renderer,
				"Assets/Meshes/TextPlane.obj",
				Color(1.0f, 1.0f, 1.0f, 1.0f),
				nullptr,
				&meshShader
			);

	//Text plane couldn't be initialise...
	if (!error) return false;

	error = m_hitMarkerMesh.Initialise(
				renderer,
				"Assets/Meshes/Marker.obj",
				Color(235.0f / 255.0f, 222.0f/255.0f, 55.0f/255.0f, 1.0f),
				nullptr,
				&meshShader
			);

	//Hit marker couldn't be initialise...
	if (!error)	return false;

	error = m_waterMarkerMesh.Initialise(
				renderer,
				"Assets/Meshes/Marker.obj",
				Color(0, 0, 0, 1.0f),
				nullptr,
				&meshShader
			);

	//Water marker couldn't be initialise...
	if (!error)	return false;

	m_board = StaticObject(&m_boardMesh, position);//std::unique_ptr<StaticObject>(new StaticObject(&m_boardMesh, position));
	m_textPlane = StaticObject(&m_textPlaneMesh, position);//std::unique_ptr<StaticObject>(new StaticObject(&m_textPlaneMesh, position));

	for (unsigned int i = 0; i < 17; ++i)
	{
		m_hitMarkers.push_back(
			Marker(&m_hitMarkerMesh, DirectX::SimpleMath::Vector3(-6, 1, -1))
		);
	}

	for (unsigned int i = 0; i < 83; ++i)
	{
		m_waterMarkers.push_back(
			Marker(&m_waterMarkerMesh, DirectX::SimpleMath::Vector3::Zero)
		);
	}

	/* Initialise the board map with possible ship positions.
	 * The board position is in the center (position e 5).
	 * Add 4 to x and y. The result will be the top left corner (a 1).
	*/
	for (int i = 0, y=(int)position.z+4; i < 10; ++i, --y)
	{
		for (int j = 0, x=(int)position.x+4; j < 10; ++j, --x)
		{
			m_boardMap.push_back(BoardPositions{ x, y, 0 });
		}
	}

	//MessageBox ( NULL, sstm.str ().c_str (), "gameboard.cpp", MB_OK );
	


	return true;
}

void GameBoard::Update(float timestep)
{

}

void GameBoard::Render(Direct3D *renderer, Camera &currentCamera)
{
	m_board.Render(renderer, &currentCamera);
	m_textPlane.Render(renderer, &currentCamera);
	RenderMarkerInVector(m_hitMarkers, renderer, currentCamera);
	RenderMarkerInVector(m_waterMarkers, renderer, currentCamera);
}

bool GameBoard::SetMarker(int x, int y)
{
	Marker* marker = nullptr;
	BoardPositions *position = nullptr;
	MarkerTypes type;

	position = GetPosition ( x, y );

	//Position not valid;
	if ( position == nullptr ) return false;

	if ( position->value == BOMBED ) return false;

	if ( position->value == 0 ) type = WATER_MARKER;
	else type = HIT_MARKER;

	switch ( position->value )
	{
	case 1:
		if ( m_aircraftCarrier >= 0 ) --m_aircraftCarrier;
		break;
	case 2:
		if ( m_battleship >= 0 ) --m_battleship;
		break;
	case 3:
		if ( m_destroyer >= 0 ) --m_destroyer;
		break;
	case 4:
		if ( m_submarine >= 0 ) --m_submarine;
		break;
	case 5:
		if ( m_patrolBoat >= 0 ) --m_patrolBoat;
		break;
	default:
		break;
	}

	switch (type)
	{
	case HIT_MARKER:

		++m_hits;
		marker = GetNextFreeMarker(m_hitMarkers);

		break;
	case WATER_MARKER:

		++m_missed;
		marker = GetNextFreeMarker(m_waterMarkers);

		break;
	default:

		break;
	}

	if (marker != nullptr) 
	{
		marker->SetPosition(DirectX::SimpleMath::Vector3(x, 0, y));
		marker->SetVisible(true);

		return true;
	}

	return false;
}

bool GameBoard::AddValueToPosition (
		int startX,
		int startY,
		Direction shipDirection,
		int shipLength,
		int *checkValues,
		int addValue,
		int arraySize
	)
{

	std::vector<int> x;
	std::vector<int> y;

	InitialiseVectors ( x, y, shipDirection, shipLength, startX, startY );

	if( ArePositionsValid ( x, y, checkValues, arraySize ) )
	{

		//Positions are valid. Lets add the ship.
		SetBoardValues ( x, y, addValue );

		return true;
	}

	//Not valid.
	return false;
}

void GameBoard::PrintBoard ()
{
	DBOUT ( "\n" );
	for ( unsigned int i = 0; i < m_boardMap.size(); i += 10 )
	{
		for ( unsigned int j = i; j < (i+10); ++j )
		{
			DBOUT ( m_boardMap[j].value << " " );
		}
		DBOUT ( "\n" );
	}
}

int GameBoard::ShipsLeft ()
{
	int left = 0;

	if ( m_aircraftCarrier > 0 ) ++left;
	if ( m_battleship > 0 ) ++left;
	if ( m_destroyer > 0 ) ++left;
	if ( m_submarine > 0 ) ++left;
	if ( m_patrolBoat > 0 ) ++left;

	return left;
}

void GameBoard::Reset ()
{
	m_aircraftCarrier = 5;
	m_battleship = 4;
	m_destroyer = 3;
	m_submarine = 3;
	m_patrolBoat = 2;

	m_hits = 0;
	m_missed = 0;

	for ( int i = 0; i < m_hitMarkers.size (); ++i )
	{
		m_hitMarkers[i].SetVisible ( false );
	}

	for ( int i = 0; i < m_waterMarkers.size (); ++i )
	{
		m_waterMarkers[i].SetVisible ( false );
	}

	for ( int i = 0; i < m_boardMap.size (); ++i )
	{
		m_boardMap[i].value = 0;
	}

}

//#################### PRIVATE METHODS ###################################

Marker* GameBoard::GetNextFreeMarker(std::vector<Marker> &markers)
{
	for (unsigned int i = 0; i < markers.size(); ++i)
	{
		if (!markers[i].IsVisible()) return &markers[i];
	}

	//No marker left found, shouldn't be happen...
	return nullptr;
}

void GameBoard::RenderMarkerInVector(std::vector<Marker> &markers, Direct3D *renderer, Camera &currentCamera)
{
	for (unsigned int i = 0; i < markers.size(); ++i)
	{
		if (markers[i].IsVisible()) markers[i].Render(renderer, &currentCamera);
	}
}

bool GameBoard::IsPositionValid ( int x, int y, int *values, int arraySize )
{
	BoardPositions* position = nullptr;

	position = GetPosition ( x, y );

	//The position was not found -> is not valid.
	if ( position == nullptr ) return false;

	//Check only if x and y is valid;
	if ( arraySize <= 0 || values == nullptr ) return true;

	//The position is valid. Check if position is free.
	for ( int i = 0; i < arraySize; ++i)
	{
		if ( position->value == values[i] ) return true;
	}

	//Position is not free...
	return false;
}

bool GameBoard::ArePositionsValid ( std::vector<int> x, std::vector<int> y, int *values, int arraySize )
{
	bool valid{ true };

	for ( unsigned int i = 0; i < x.size () && i < y.size (); ++i )
	{
		valid &= IsPositionValid ( x[i], y[i], values, arraySize );
	}

	return valid;
}

BoardPositions* GameBoard::GetPosition ( int x, int y )
{
	for ( unsigned int i = 0; i < m_boardMap.size (); ++i )
	{
		BoardPositions position = m_boardMap[i];

		if ( position.x == x && position.y == y )
		{
			return &m_boardMap[i];
		}
	}

	return nullptr;
}

void GameBoard::SetBoardValues ( std::vector<int> &x, std::vector<int> &y, int value )
{
	BoardPositions* position = nullptr;

	for ( unsigned int i = 0; i < x.size () && i < y.size (); ++i )
	{
		position = GetPosition ( x[i], y[i] );

		//Shouldn't be nullptr, 'cause it was checked already...
		if ( position != nullptr )
		{
			position->x = x[i];
			position->y = y[i];
			position->value = value;
		}
	}
}

//################### CPP Methods ##########

void InitialiseVectors (
	std::vector<int> &x,
	std::vector<int> &z,
	Direction shipDirection,
	int shipLength,
	int startX,
	int startZ
	)
{
	for ( int i = 0; i < shipLength; ++i )
	{
		x.push_back ( startX );
		z.push_back ( startZ );
		switch ( shipDirection )
		{
		case HORIZONTAL:
			//Only the x value need to change...
			//Goes from left to right
			--startX;
			break;
		case VERTICAL:
			//Only the y value need to change...
			//Goes from top to bottom
			--startZ;
			break;
		default:
			break;
		}
	}
}