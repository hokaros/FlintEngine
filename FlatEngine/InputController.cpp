#include "InputController.h"

InputController* InputController::main{ NULL };

InputController* InputController::Main() {
	return main;
}

InputController::InputController(SDL_Keycode* managedKeys, size_t keysCount)
	: keysCount(keysCount),
	managedKeys(new SDL_Keycode[keysCount]),
	keyDownInfo(new bool[keysCount]),
	pressedThisFrame(new bool[keysCount]) {

	if (main == NULL) {
		main = this;
	}


	std::memcpy(this->managedKeys, managedKeys, keysCount * sizeof(SDL_Keycode));
	for (size_t i = 0; i < keysCount; i++) {
		keyDownInfo[i] = false;
		pressedThisFrame[i] = false;
	}
}

InputController::~InputController() {
	delete[] managedKeys;
	delete[] keyDownInfo;

	if (main == this) {
		main = NULL;
	}
}

bool InputController::Update() {
	ClearFrameInfo();

	SDL_Event event;
	bool quit = false;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			quit = true;

		if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
			continue; //not my business

		for (size_t i = 0; i < keysCount; i++) {
			if (event.key.keysym.sym == managedKeys[i]) {
				if (event.type == SDL_KEYDOWN) {
					keyDownInfo[i] = true;
					pressedThisFrame[i] = true;
				}
				else if (event.type == SDL_KEYUP) {
					keyDownInfo[i] = false;
				}
			}
		}
	};

	return !quit;
}

bool InputController::IsKeyDown(SDL_Keycode key) const {
	int index = KeyIndex(key);
	if (index < 0)
		return false; // nie nale¿y do zarz¹dzanych klawiszy

	return keyDownInfo[index];
}

bool InputController::PressedThisFrame(SDL_Keycode key) const {
	int index = KeyIndex(key);
	if (index < 0)
		return false;

	return pressedThisFrame[index];
}

Vector InputController::GetMousePosition() const {
	int x, y;
	SDL_GetMouseState(&x, &y);

	return Vector(x, y);
}

int InputController::KeyIndex(SDL_Keycode key) const {
	for (size_t i = 0; i < keysCount; i++) {
		if (managedKeys[i] == key) {
			return i;
		}
	}

	return -1;
}

void InputController::ClearFrameInfo() {
	for (size_t i = 0; i < keysCount; i++) {
		pressedThisFrame[i] = false;
	}
}
