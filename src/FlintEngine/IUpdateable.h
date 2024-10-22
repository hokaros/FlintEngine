#pragma once

class SceneRenderer;

class IUpdateable {
public:
	// Wywo³ywane raz na klatkê
	virtual void Update() {}
	virtual void RenderUpdate(SceneRenderer& renderer) {}
	virtual void RenderEditorSelected(SceneRenderer& renderer) {}

	// Wywo³ywane przed rozpoczêciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}
};