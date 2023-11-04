#include "TextBox.h"
#include "DebugRenderer.h" // TODO: remove

TextBox::TextBox(const SDL_Rect& rect, const Rgb8& outline, const Rgb8& fill, int fontSize)
	: m_Rect(rect)
	, m_Outline(outline)
	, m_Fill(fill)
	, m_FontSize(fontSize) 
{

}

void TextBox::Draw() 
{
	// Ramka
	Rect rect = SDLRectToRect(m_Rect);
	DebugRenderer::DrawRect(rect, m_Fill);
	DebugRenderer::DrawWireRect(rect, m_Outline);

	// Wyświetlenie zebranego tekstu
	char buffer[32];
	int i = 0;
	for (char c : m_Content) 
	{
		buffer[i++] = c;
	}
	buffer[i] = '\0';

	Window::Main()->DrawString(
		m_Rect.x + TEXTBOX_PADDING,
		m_Rect.y + m_Rect.h / 2 - m_FontSize / 2,
		buffer,
		m_FontSize
	);
}

void TextBox::Update() 
{
	// Odczytanie klawiszy
	for (char digit = '0'; digit <= '9'; digit++) {
		SDL_Keycode key = (SDL_Keycode)digit;

		if (InputController::Main()->PressedThisFrame(key)) {
			AddCharacter(digit);
		}
	}

	if (InputController::Main()->PressedThisFrame((SDL_Keycode)'.')) {
		AddCharacter('.');
	}

	if (InputController::Main()->PressedThisFrame(SDLK_BACKSPACE)) {
		RemoveLast();
	}
}

int TextBox::MaxCharacters() const 
{
	return (m_Rect.w - 2* TEXTBOX_PADDING) / m_FontSize;
}

void TextBox::AddCharacter(char character) 
{
	if (m_Content.size() < MaxCharacters())
	{
		m_Content.push_back(character);
	}
}

void TextBox::RemoveLast() 
{
	if (m_Content.empty())
		return;  // jestem tylko biednym textboxem

	m_Content.pop_back();
}

std::string TextBox::GetContent() const 
{
	char buffer[32];
	int i = 0;
	for (char c : m_Content)
	{
		buffer[i++] = c;
	}
	buffer[i] = '\0';

	return std::string(buffer);
}