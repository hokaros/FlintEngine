#include "Window.h"

#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

Window* Window::s_MainWindow = nullptr;

Window::Window(int width, int height)
	: m_Width(width)
	, m_Height(height) 
	, m_SceneRenderer(width, height)
{
	if (s_MainWindow == nullptr) 
	{
		s_MainWindow = this;
	}
}

Window::~Window() 
{
	DeinitImGui();

	if (m_Window != nullptr) 
	{
		SDL_DestroyWindow(m_Window);
	}
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
	}
	SDL_Quit();

	if (s_MainWindow == this) {
		s_MainWindow = nullptr;
	}
}

Window* Window::Main() 
{
	return s_MainWindow;
}

bool Window::Init() 
{
	int rc;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// tryb pe³noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(m_Width, m_Height, 0,
		&m_Window, &m_Renderer);
	if (rc != 0) 
	{
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);

	SDL_SetWindowTitle(m_Window, "Labyrinth Shooter");

	m_DebugRenderer = std::make_unique<DebugRenderer>(m_SceneRenderer);

	InitImGui();

	return m_SceneRenderer.Init(m_Renderer, {});
}

void Window::Present() 
{
	SDL_RenderCopy(m_Renderer, m_SceneRenderer.GetOutputTexture(), nullptr, nullptr);

	RenderImGui();

	SDL_RenderPresent(m_Renderer);

	SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(m_Renderer);

	ImGuiNewFrame();
}

SDL_Renderer* Window::GetRenderer() const 
{
	return m_Renderer;
}

SceneRenderer& Window::GetSceneRenderer()
{
	return m_SceneRenderer;
}

int Window::GetWidth() const 
{
	return m_Width;
}

int Window::GetHeight() const 
{
	return m_Height;
}

void Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
	ImGui_ImplSDLRenderer2_Init(m_Renderer);

	ImGuiNewFrame();
}

void Window::DeinitImGui()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Window::ImGuiNewFrame()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Window::RenderImGui()
{
	ImGui::Render();
	SDL_RenderSetScale(m_Renderer, ImGui::GetIO().DisplayFramebufferScale.x, ImGui::GetIO().DisplayFramebufferScale.y);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
