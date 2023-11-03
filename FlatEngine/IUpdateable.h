#pragma once

class IUpdateable {
public:
	// Wywo�ywane raz na klatk�
	virtual void Update() {}
	virtual void RenderUpdate() {}

	// Wywo�ywane przed rozpocz�ciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}

	virtual IUpdateable* Copy() = 0;
};