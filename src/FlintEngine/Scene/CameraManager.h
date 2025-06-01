#pragma once

class Camera;

namespace scene
{
	class CameraManager
	{
	public:
		Camera* GetCurrentCamera();

		void RegisterCamera(Camera& camera);
		void UnregisterCamera(Camera& camera);

	private:
		Camera* m_CurrentCamera = nullptr;
	};
}