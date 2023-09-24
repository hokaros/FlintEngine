#pragma once

class IUpdateable {
public:
	// Wywo³ywane raz na klatkê
	virtual void Update() {}
	virtual void RenderUpdate() {}

	// Wywo³ywane przed rozpoczêciem gry
	virtual void Start() {}

	virtual IUpdateable* Copy() = 0;
};