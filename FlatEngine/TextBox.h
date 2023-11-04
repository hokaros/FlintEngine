#pragma once
#include "Window.h"
#include "Draw.h"
#include "Timer.h"
#include "InputController.h"
#include <deque>

#define TEXTBOX_PADDING 4

class TextBox
{
public:
	TextBox(const SDL_Rect& rect, const Rgb8& outline, const Rgb8& fill, int fontSize);

	void Draw();
	void Update();
	// Maksymalna liczba znaków, które mog¹ siê zmieœciæ
	int MaxCharacters() const;

	void AddCharacter(char character);
	void RemoveLast();

	std::string GetContent() const;

private:
	SDL_Rect m_Rect;
	Rgb8 m_Outline;
	Rgb8 m_Fill;
	int m_FontSize;

	std::deque<char> m_Content;
};

