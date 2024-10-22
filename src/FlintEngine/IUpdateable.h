#pragma once

class SceneRenderer;

class IUpdateable {
public:
	// Wywo�ywane raz na klatk�
	virtual void Update() {}
	virtual void RenderUpdate(SceneRenderer& renderer) {}
	virtual void RenderEditorSelected(SceneRenderer& renderer) {}

	// Wywo�ywane przed rozpocz�ciem gry
	virtual void Awake() {}
	virtual void Start() {}

	virtual void OnDestroy() {}
};