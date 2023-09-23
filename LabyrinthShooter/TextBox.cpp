#include "TextBox.h"

TextBox::TextBox(const SDL_Rect& rect, Uint32 outline, Uint32 fill, int fontSize)
	: rect(rect), outline(outline), fill(fill), fontSize(fontSize) {

}

void TextBox::Draw() {
	// Ramka
	DrawRectangle(
		Window::Main()->GetScreen(),
		rect.x,
		rect.y,
		rect.w,
		rect.h,
		outline, fill
	);

	// Wyœwietlenie zebranego tekstu
	char buffer[32];
	int i = 0;
	for (char c : content) {
		buffer[i++] = c;
	}
	buffer[i] = '\0';

	Window::Main()->DrawString(
		rect.x + TEXTBOX_PADDING,
		rect.y + rect.h / 2 - fontSize / 2,
		buffer, fontSize
	);
}

void TextBox::Update() {
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

int TextBox::MaxCharacters() const {
	return (rect.w - 2* TEXTBOX_PADDING) / fontSize;
}

void TextBox::AddCharacter(char character) {
	if (content.size() < MaxCharacters()) {
		content.push_back(character);
	}
}

void TextBox::RemoveLast() {
	if (content.empty())
		return;  // jestem tylko biednym textboxem

	content.pop_back();
}

std::string TextBox::GetContent() const {
	char buffer[32];
	int i = 0;
	for (char c : content) {
		buffer[i++] = c;
	}
	buffer[i] = '\0';

	return std::string(buffer);
}