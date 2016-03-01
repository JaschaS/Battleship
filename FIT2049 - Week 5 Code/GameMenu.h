#ifndef GAME_MENU_H
#define GAME_MENU_H

/*	FIT2049 - Assignment 1
*	GameMenu.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	The game menu which the game will use.
*/

#include "Direct3D.h"
#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"
#include "DirectXTK\SimpleMath.h"
#include "InputController.h"
#include "Colors.h"

class GameMenu
{
private:
	LPCWSTR m_header;

	DirectX::SimpleMath::Vector2 m_headerOrigin;

	DirectX::SpriteBatch *m_spriteBatch;
	DirectX::SpriteFont *m_font;
	DirectX::SpriteFont *m_fontHeader;

	int m_cursor{ 0 };
	int m_cursorPositions[3] = { 400, 450, 500 };
public:
	GameMenu();
	~GameMenu();

	bool Initialise(
		Direct3D *renderer,
		DirectX::SpriteBatch *spriteBatch,
		DirectX::SpriteFont *fontHeader,
		DirectX::SpriteFont *font,
		InputController *input
	);

	void Update();
	void Draw ();

	void CursorUp () { if(m_cursor > 0) --m_cursor; }
	void CursorDown () { if(m_cursor < 3) ++m_cursor; }
	int GetCursor () { return m_cursor; }
};

#endif
