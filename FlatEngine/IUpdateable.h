#pragma once

class IUpdateable {
public:
	// Wywo³ywane raz na klatkê
	virtual void Update() {}
	virtual void RenderUpdate() {}

	// Wywo³ywane przed rozpoczêciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}

	virtual IUpdateable* Copy() = 0;
};