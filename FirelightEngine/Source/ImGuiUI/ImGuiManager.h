#pragma once

#include "ImGuiLayer.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace Firelight::ImGuiUI
{
	class ImGuiManager
	{
	public:
		static ImGuiManager* Instance();

		~ImGuiManager();

		bool Initialise(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* contextDevice);

		void Render();
		void AddRenderLayer(ImGuiLayer* layer);
		void RemoveRenderLayer(ImGuiLayer* layer);

	private:
		void ClearRenderLayer();

	private:
		ImGuiManager();

	private:
		static ImGuiManager* sm_instance;

		std::vector<ImGuiLayer*> m_itemsToRender;
		bool m_initialised;
	};
}
