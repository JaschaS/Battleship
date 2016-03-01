#include "GameMenu.h"

/*	FIT2049 - Assignment 1
*	GameMenu.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of GameMenu.h
*/

GameMenu::GameMenu() {}

GameMenu::~GameMenu()
{

}

bool GameMenu::Initialise(
	Direct3D *renderer,
	DirectX::SpriteBatch* spriteBatch,
	DirectX::SpriteFont* fontHeader,
	DirectX::SpriteFont* font,
	InputController* input
	)
{
	if (renderer == nullptr) return false;
	if (spriteBatch == nullptr) return false;
	if (font == nullptr) return false;
	if (input == nullptr) return false;

	m_spriteBatch = spriteBatch;
	m_font = font;
	m_fontHeader = fontHeader;

	m_header = L"BATTLESHIP";


	return true;
}

void GameMenu::Update()
{

}

void GameMenu::Draw ()
{
	DirectX::SimpleMath::Vector2 stringSize;
	DirectX::SimpleMath::Vector2 textOrigin;
	std::wstring message;

	stringSize = m_fontHeader->MeasureString ( m_header );
	textOrigin = DirectX::SimpleMath::Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );

	m_fontHeader->DrawString ( m_spriteBatch, m_header, DirectX::SimpleMath::Vector2 ( 640, 200 ), BROWN, 0, textOrigin );


	message = L">>";
	stringSize = m_font->MeasureString ( message.c_str () );
	textOrigin = DirectX::SimpleMath::Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font->DrawString ( m_spriteBatch, message.c_str (), DirectX::SimpleMath::Vector2 ( 540, m_cursorPositions[m_cursor] ), BLUE, 0, textOrigin );

	message = L"[P]lay";
	stringSize = m_font->MeasureString ( message.c_str () );
	textOrigin = DirectX::SimpleMath::Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font->DrawString ( m_spriteBatch, message.c_str (), DirectX::SimpleMath::Vector2 ( 640, 400 ), m_cursor == 0 ? BLUE : WHITE, 0, textOrigin );

	message = L"[H]elp";
	stringSize = m_font->MeasureString ( message.c_str () );
	textOrigin = DirectX::SimpleMath::Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font->DrawString ( m_spriteBatch, message.c_str (), DirectX::SimpleMath::Vector2 ( 640, 450 ), m_cursor == 1 ? BLUE : WHITE, 0, textOrigin );

	message = L"[Q]uit";
	stringSize = m_font->MeasureString ( message.c_str () );
	textOrigin = DirectX::SimpleMath::Vector2 ( (int) stringSize.x / 2, (int) stringSize.y / 2 );
	m_font->DrawString ( m_spriteBatch, message.c_str (), DirectX::SimpleMath::Vector2 ( 640, 500 ), m_cursor == 2 ? BLUE : WHITE, 0, textOrigin );
}