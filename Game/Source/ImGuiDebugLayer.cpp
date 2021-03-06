#include "ImGuiDebugLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include "Source/Engine.h"

#include "Source/ECS/EntityWrappers/GameEntity.h"

using namespace Firelight::ECS;

ImGuiDebugLayer::ImGuiDebugLayer()
{
	
}

ImGuiDebugLayer::~ImGuiDebugLayer()
{

}

void ImGuiDebugLayer::Initialize()
{
	SetupTheme();
	spawnItemCommand = std::vector<CallbackFunctionType>(5);
}

void ImGuiDebugLayer::Render()
{
	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground ;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();

	RenderItemWindow();
	RenderKeyBindingPrototype();
	RenderDebugInformation();
	RenderECSDebug();
	RenderControls();
}

void ImGuiDebugLayer::RenderItemWindow()
{
	ImGui::Begin("Debug Menu");
	if (ImGui::Button("Spawn Rock"))
	{
		spawnItemCommand[0]();
	}
	if (ImGui::Button("Spawn All Items"))
	{
		spawnItemCommand[1]();
	}
	/// <summary>
	/// PCG Testing
	/// </summary>
	if (ImGui::Button("Generate Biomes"))
	{
		spawnItemCommand[2]();
	}

	ImGui::End();
}

void ImGuiDebugLayer::RenderControls()
{
	ImGui::Begin("Debug Controls");
	if (ImGui::Button("Play/Pause"))
	{
		Firelight::Engine::Instance().TogglePause();
	}
	ImGui::End();
}

void ImGuiDebugLayer::RenderECSDebug()
{
	ImGui::Begin("ECS Debug");
	if (ImGui::Button("Save ECS"))
	{
		Serialiser::SaveSceneJSON();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load ECS"))
	{
		Serialiser::LoadSceneJSON();
	}
	std::vector<EntityID> entities = EntityComponentSystem::Instance()->GetEntities();
	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode(("Entities (" + std::to_string(entities.size()) + ")").c_str()))
	{
		for (auto& entity : entities)
		{
			std::vector<BaseComponent*> components = EntityComponentSystem::Instance()->GetAllComponents(entity);
			IdentificationComponent* id = EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(entity);
			std::string nodeText = std::to_string(entity);
			if (id != nullptr)
			{
				nodeText +=  ": " + id->name;
			}
			else
			{
				nodeText = "Entity " + nodeText;
			}

			if (ImGui::TreeNode((nodeText + " (" + std::to_string(components.size()) + ")").c_str()))
			{
				for (int i = 0; i < components.size(); ++i)
				{
					BaseComponent* component = components[i];
					std::string componentTypeString = EntityComponentSystem::Instance()->GetTypeOfComponent(component).substr(7);
					if (componentTypeString.starts_with("Firelight::ECS::"))
					{
						componentTypeString = componentTypeString.substr(16);
					}
					if (ImGui::TreeNode(componentTypeString.c_str()))
					{
						component->RenderDebugUI();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void ImGuiDebugLayer::RenderKeyBindingPrototype()
{
	ImGui::Begin("Key Bindings");
	if(ImGui::BeginTabBar("##KeyBindingTabs"))
	{
		if (ImGui::BeginTabItem("Keyboard"))
		{
			// Get Current Key Bindings
			// Display List of Bindings with drop down for current key.
			// When a key is selected, unbind the previous key and bind the new key
			static std::vector<Firelight::KeyboardBindingData> keyBindings;
			keyBindings = Firelight::Engine::Instance().GetKeyBinder().GetCurrentKeyBindings();

			if (ImGui::BeginTable("##KeyBindingTable", 3));
			{            
				ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Mode", ImGuiTableColumnFlags_WidthFixed, 150.0f);
				ImGui::TableSetupColumn("Bound Key", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableHeadersRow();
				for (auto& binding : keyBindings)
				{
					ImGui::TableNextRow();

					AddKeyBindingWidget(binding);
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Controller"))
		{
			//Same as above, but for controller
			static std::vector<Firelight::ControllerBindingData> buttonBindings;
			buttonBindings = Firelight::Engine::Instance().GetKeyBinder().GetCurrentControllerBindings();

			if (ImGui::BeginTable("##ButtonBindingTable", 3));
			{
				ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Mode", ImGuiTableColumnFlags_WidthFixed, 175.0f);
				ImGui::TableSetupColumn("Bound Button", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableHeadersRow();
				for (auto& binding : buttonBindings)
				{
					ImGui::TableNextRow();

					AddButtonBindingWidget(binding);
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void ImGuiDebugLayer::RenderDebugInformation()
{
	ImGui::Begin("Debug Info");
	m_fpsUpdateDelay += Firelight::Engine::Instance().GetTime().GetDeltaTime();
	if (m_fpsUpdateDelay > 1.0f)
	{
		m_fpsUpdateDelay -= 1.0f;
		m_currentFPS = 1.0f / (float)Firelight::Engine::Instance().GetTime().GetDeltaTime();
	}
	ImGui::Text((std::string("FPS: ") + std::to_string(m_currentFPS)).c_str());
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Keys down:");  for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyDown(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
	ImGui::End();
}

void ImGuiDebugLayer::AddKeyBindingWidget(Firelight::KeyboardBindingData binding)
{
	ImGui::TableSetColumnIndex(0);
	ImGui::Text(binding.eventName);
	ImGui::SameLine();
	
	const char* eventTypes[] = { "Key Press", "Key Press Single", "Key Release" };
	const char* currentEventType = binding.eventType.c_str();

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(150.0f);
	if (ImGui::BeginCombo(("##" + std::string(binding.eventName) + "EventTypeCombo").c_str(), currentEventType))
	{
		for (int n = 0; n < IM_ARRAYSIZE(eventTypes); n++)
		{
			bool isSelected = (currentEventType == eventTypes[n]);
			if (ImGui::Selectable(eventTypes[n], isSelected))
			{
				Firelight::Engine::Instance().GetKeyBinder().ChangeKeyEventType(binding, eventTypes[n]);
				currentEventType = eventTypes[n];
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();

	const char* keys[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",",",".","/","\\",";","'","#","[","]","-","=","LEFT","UP","RIGHT","DOWN","NUMPAD 0" ,"NUMPAD 1" ,"NUMPAD 2" ,"NUMPAD 3" ,"NUMPAD 4" ,"NUMPAD 5" ,"NUMPAD 6" ,"NUMPAD 7" ,"NUMPAD 8" ,"NUMPAD 9" };
	std::string currentKeyString = Firelight::Engine::Instance().GetKeyBinder().GetEnumString(binding.key);
	const char* currentKey = currentKeyString.c_str();

	ImGui::TableSetColumnIndex(2);
	ImGui::SetNextItemWidth(100.0f);
	if (ImGui::BeginCombo(("##" + std::string(binding.eventName) + "KeyCombo").c_str(), currentKey))
	{
		for (int n = 0; n < IM_ARRAYSIZE(keys); n++)
		{
			bool isSelected = (currentKey == keys[n]);
			if (ImGui::Selectable(keys[n], isSelected))
			{
				if (Firelight::Engine::Instance().GetKeyBinder().ChangeEventKey(binding, keys[n]))
				{
					currentKey = keys[n];
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
		}
		ImGui::EndCombo();
	}

}

void ImGuiDebugLayer::AddButtonBindingWidget(Firelight::ControllerBindingData binding)
{
	ImGui::TableSetColumnIndex(0);
	ImGui::Text(binding.eventName);
	ImGui::SameLine();
	
	const char* eventTypes[] = { "Button Press", "Button Press Single", "Button Release" };
	const char* currentEventType = binding.eventType.c_str();

	ImGui::TableSetColumnIndex(1);
	ImGui::SetNextItemWidth(175.0f);
	if (ImGui::BeginCombo(("##" + std::string(binding.eventName) + "EventTypeCombo").c_str(), currentEventType))
	{
		for (int n = 0; n < IM_ARRAYSIZE(eventTypes); n++)
		{
			bool isSelected = (currentEventType == eventTypes[n]);
			if (ImGui::Selectable(eventTypes[n], isSelected))
			{
				Firelight::Engine::Instance().GetKeyBinder().ChangeControllerEventType(binding, eventTypes[n]);
				currentEventType = eventTypes[n];
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();

	const char* buttons[] = { "A","B","X","Y","LB","RB","LT","RT","STICK LEFT", "STICK RIGHT","UP","LEFT","DOWN","RIGHT","START","BACK"};
	std::string currentButtonString = Firelight::Engine::Instance().GetKeyBinder().GetEnumString(binding.button);
	const char* currentButton = currentButtonString.c_str();

	ImGui::TableSetColumnIndex(2);
	ImGui::SetNextItemWidth(100.0f);
	if (ImGui::BeginCombo(("##" + std::string(binding.eventName) + "ButtonCombo").c_str(), currentButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(buttons); n++)
		{
			bool isSelected = (currentButton == buttons[n]);
			if (ImGui::Selectable(buttons[n], isSelected))
			{
				if (Firelight::Engine::Instance().GetKeyBinder().ChangeEventButton(binding, buttons[n]))
				{
					currentButton = buttons[n];
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
		}
		ImGui::EndCombo();
	}

}

void ImGuiDebugLayer::SetupTheme()
{
	auto& colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.4f, 0.405f, 0.41f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.25f, 0.2505f, 0.251f, 1.0f };

	// Frame Background
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
