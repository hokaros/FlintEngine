#pragma once

class IUpdateable {
public:
	// Wywoływane raz na klatkę
	virtual void Update() {}
	virtual void RenderUpdate() {}

	// Wywoływane przed rozpoczęciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}
};