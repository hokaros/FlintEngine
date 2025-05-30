#pragma once

class SceneRenderer;

class IUpdateable {
public:
	// Wywoływane raz na klatkę
	virtual void Update() {}
	virtual void RenderUpdate(SceneRenderer& renderer) {}
	virtual void RenderEditorSelected(SceneRenderer& renderer) {}

	// Wywoływane przed rozpoczęciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}
};