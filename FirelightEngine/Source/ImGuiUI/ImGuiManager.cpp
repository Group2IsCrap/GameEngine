#include "ImGuiManager.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "../Graphics/GraphicsHandler.h"

namespace Firelight::ImGuiUI
{
	ImGuiManager* ImGuiManager::sm_instance = nullptr;

	ImGuiManager* ImGuiManager::Instance()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new ImGuiManager();
		}
		return sm_instance;
	}

	ImGuiManager::ImGuiManager()
	{

	}
	
	ImGuiManager::~ImGuiManager()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	bool ImGuiManager::Initialise(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* contextDevice)
	{
		ImGui_ImplWin32_EnableDpiAwareness();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device, contextDevice);

		return true;
	}
	
	void ImGuiManager::Render()
	{
		// ImGui Render
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		for (auto layer : m_itemsToRender)
		{
			layer->Render();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	
	void ImGuiManager::AddRenderLayer(ImGuiLayer* layer)
	{
		m_itemsToRender.push_back(layer);
	}
	
	void ImGuiManager::RemoveRenderLayer(ImGuiLayer* layer)
	{

	}
	
	void ImGuiManager::ClearRenderLayer()
	{
		m_itemsToRender.clear();
	}
}