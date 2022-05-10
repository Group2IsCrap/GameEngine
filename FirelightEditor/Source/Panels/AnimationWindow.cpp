#include "AnimationWindow.h"

#include <Source/Graphics/AssetManager.h>

#include "../DrawHelper.h"

#include <iostream>
#include <fstream>
#include <filesystem>

static std::string s_animationName;
static bool s_newAnimation;
static std::filesystem::path s_newAnimationPath;

AnimationWindow::AnimationWindow()
{
	m_selectedAnimation = new Firelight::Animation::Animation();
	GetAllAnimations();
}

AnimationWindow::~AnimationWindow()
{
}

void AnimationWindow::Draw()
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImVec2 animWindowSize = ImVec2(300.0f, 150.0f);
	ImGui::SetNextWindowSize(animWindowSize, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(
		(windowSize.x * 0.5f) - (animWindowSize.x * 0.5f), 
		(windowSize.y * 0.5f) - (animWindowSize.y * 0.5f)), ImGuiCond_FirstUseEver);

	
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Animation Window##AnimationWindow", &m_isOpen, window_flags);
	ImGuiWindow* menu_bar_window = ImGui::FindWindowByName("Animation Window##AnimationWindow");
	DrawMenuBar(menu_bar_window);

	// Animation Name
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Name:");
	ImGui::SameLine();
	auto& animName = m_selectedAnimation->m_animationName;

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	std::strncpy(buffer, animName.c_str(), sizeof(buffer));
	ImGui::SetNextItemWidth(180.0f);
	if (ImGui::InputText("##AnimPath", buffer, sizeof(buffer)))
	{
		animName = std::string(buffer);
	}
	m_selectedAnimation->m_animationName = animName;

	// Frame Count
	// Animation Name
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Frame Count");
	ImGui::SameLine();
	auto& frameCount = m_selectedAnimation->m_frameCount;
	ImGui::InputInt("##AnimFrameCount", &frameCount);

	Firelight::Graphics::Texture* icon;

	std::filesystem::path path = m_selectedAnimation->m_path;
	std::string extension = path.extension().string();
	if (extension == ".png")
	{
		std::string filePath = path.string();
		icon = Firelight::Graphics::AssetManager::Instance().GetTexture(filePath);
	}
	else
	{
		icon = Firelight::Graphics::AssetManager::Instance().GetDefaultTexture();
	}

	
	ImGui::ImageButton((ImTextureID)icon->GetShaderResourceView().Get(), { 64, 64 }, { 0, 0 }, { 1, 1 },
		1, { 0, 0, 0, 0 }, { 1, 1, 1, 1 });

	if (s_newAnimation)
	{
		ImGui::OpenPopup("New Animation");
	}

	CreateAnimation();


	ImGui::End();
}

void AnimationWindow::DrawMenuBar(ImGuiWindow* window)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File##animFile"))
		{
			if (ImGui::MenuItem("New Animation##AnimNew"))
			{
				s_newAnimation = true;
			}
			if (ImGui::MenuItem("Save##SaveAnim"))
			{
				SaveFile(m_selectedAnimation->m_animationName.c_str());
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::PopStyleVar(1);
}

void AnimationWindow::CreateAnimation()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("New Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		s_newAnimation = false;
		ImGui::Text("Enter Animation Name\n");

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, s_animationName.c_str(), sizeof(buffer));
		if (ImGui::InputText("##AnimationName", buffer, sizeof(buffer)))
		{
			s_animationName = std::string(buffer);
		}

		std::string newPath = "Assets/Animations/" + s_animationName;

		ImGui::Spacing();

		if (ImGui::Button("Create", ImVec2(125.0f, 0)))
		{
			if (!std::filesystem::is_directory(newPath))
			{
				SaveFile(s_animationName.c_str());
			}
			s_animationName = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(125.0f, 0))) { ImGui::CloseCurrentPopup(); s_newAnimation = false; s_animationName = ""; }
		ImGui::EndPopup();
	}
}

void AnimationWindow::SaveAnimation(const char* fileName, rapidjson::StringBuffer* json)
{
	std::ofstream of(fileName, std::ofstream::out | std::ofstream::trunc);
	of << json->GetString();
	of.close();
}

void AnimationWindow::SaveFile(const char* animName)
{
	std::string fileName = animName;
	fileName += ".anim";
	std::string newPath = "Assets/Animations/" + fileName;

	rapidjson::StringBuffer* buffer = new rapidjson::StringBuffer();
	Serialiser::Writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*buffer);

	Serialiser::StartObject();
	Serialiser::Serialise("AnimationName", m_selectedAnimation->m_animationName);
	Serialiser::Serialise("FrameCount", m_selectedAnimation->m_frameCount);
	Serialiser::EndObject();
	SaveAnimation(newPath.c_str(), buffer);
}

void AnimationWindow::GetAllAnimations()
{
	int count = 1;
	for (auto& directoryEntry : std::filesystem::directory_iterator("Assets/Animations"))
	{
		const auto& path = directoryEntry.path();

		auto relativePath = std::filesystem::relative(path, "Assets/Animations");
		std::string filenameString = relativePath.filename().string();

		std::string numberString = std::to_string(count);
		const char* itemNumber = numberString.c_str();

		Firelight::Graphics::Texture* icon;

		std::string extension = path.extension().string();
		if (extension == ".anim")
		{
			std::string filePath = path.string();
			// Load file and create new Animation struct
			if (Serialiser::LoadFile("Assets/Animations/Test23.anim"))
			{
				std::string name;
				Serialiser::Deserialize("AnimationName", name);
				std::string test;
			}
		}
	}
}
