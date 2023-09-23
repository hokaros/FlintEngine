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
	TextBox(const SDL_Rect& rect, Uint32 outline, Uint32 fill, int fontSize);

	void Draw();
	void Update();
	// Maksymalna liczba znaków, które mog¹ siê zmieœciæ
	int MaxCharacters() const;

	void AddCharacter(char character);
	void RemoveLast();

	std::string GetContent() const;

private:
	SDL_Rect rect;
	Uint32 outline;
	Uint32 fill;
	int fontSize;

	std::deque<char> content;
};

