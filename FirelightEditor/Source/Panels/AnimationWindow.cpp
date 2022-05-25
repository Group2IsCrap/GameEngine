#include "AnimationWindow.h"

#include <Source/Graphics/AssetManager.h>
#include <Source/Engine.h>

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
	m_selectedAnimEnum = new int(-1);
	GetAllAnimations();
	if (m_animationNames.size() > 0)
	{
		m_selectedAnimEnum = new int(0);
	}
	m_isOpen = true;
}

AnimationWindow::~AnimationWindow()
{
}

void AnimationWindow::Draw()
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(
		(windowSize.x * 0.5f) - (250.0f), 
		(windowSize.y * 0.5f) - (250.0f)), ImGuiCond_FirstUseEver);

	
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Animation Window##AnimationWindow", &m_isOpen, window_flags);
	ImVec2 animWindowSize = ImGui::GetWindowSize();
	ImGuiWindow* menu_bar_window = ImGui::FindWindowByName("Animation Window##AnimationWindow");
	DrawMenuBar(menu_bar_window);

	if (ImGui::BeginTable("##AnimMainTable", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		float mainColumnWidth = ImGui::CalcItemWidth();

		// Animations
		if (m_animationNames.size() > 0)
		{
			ImGui::AlignTextToFramePadding();
			Firelight::Editor::DrawHelper::DrawEnumControl("Animations", m_selectedAnimEnum, m_animationNames);
			m_selectedAnimation = m_animations[*m_selectedAnimEnum];
		}

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

		// Loop
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Looping");
		ImGui::SameLine();
		auto& loop = m_selectedAnimation->m_loop;
		ImGui::Checkbox("##AnimLoop", &loop);

		// Frame Time
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Frame Time");
		ImGui::SameLine();
		auto& frameTime = m_selectedAnimation->m_frameTime;
		ImGui::SliderFloat("##AnimFrameTime", &frameTime, 1, 2000);

		// Frame Count
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Frame Count");
		ImGui::SameLine();
		auto& frameCount = m_selectedAnimation->m_frameCount;
		ImGui::SliderInt("##AnimFrameCount", &frameCount, 0, 20);
		if (frameCount > 500)
			frameCount = 500;
		if (m_selectedAnimation->m_textureNames.size() != frameCount)
		{
			m_selectedAnimation->m_textureNames.resize(frameCount);
			m_selectedAnimation->m_textures.resize(frameCount, Firelight::Graphics::AssetManager::Instance().GetDefaultTexture());
			m_selectedAnimation->m_index = 0;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 5, 5 });
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_PadOuterX;
		int columnCount = static_cast<int>(mainColumnWidth / 64);
		if (columnCount <= 0)
			columnCount = 1;

		if (ImGui::BeginTable("##AnimTable", columnCount, flags))
		{
			ImGui::TableNextColumn();
			for (int i = 0; i < frameCount; i++)
			{
				std::filesystem::path path = "";
				Firelight::Graphics::Texture* icon;
				if (m_selectedAnimation->m_textureNames.size() > 0)
				{
					std::string textureName = m_selectedAnimation->m_textureNames[i];
					path += textureName;
				}

				if (path != "")
				{
					icon = Firelight::Graphics::AssetManager::Instance().GetTexture(path.string());
				}
				else
				{
					icon = Firelight::Graphics::AssetManager::Instance().GetDefaultTexture();
				}

				ImGui::ImageButton((ImTextureID)icon->GetShaderResourceView().Get(), { 64, 64 }, { 0, 0 }, { 1, 1 },
					0, ImVec4(0, 0, 0, 0), { 1, 1, 1, 1 });

				ImGui::SetItemAllowOverlap();
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
					if (payload != nullptr)
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						if (path != nullptr)
						{
							std::filesystem::path filePath = Firelight::Utils::StringHelpers::WideStringToString(path);
							std::filesystem::path relativePath = filePath.relative_path();
							std::filesystem::path extention = filePath.extension();
							if (extention == ".png")
							{
								icon = Firelight::Graphics::AssetManager::Instance().GetTexture(relativePath.string());
								m_selectedAnimation->m_textureNames[i] = relativePath.string();
								m_selectedAnimation->m_textures[i] = Firelight::Graphics::AssetManager::Instance().GetTexture(relativePath.string());
							}
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TableNextColumn();
			}
			ImGui::EndTable();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (s_newAnimation)
		{
			ImGui::OpenPopup("New Animation");
		}

		CreateAnimation();

		ImGui::TableNextColumn();
		if (ImGui::Button("Play##AnimPlay", { 100, 35 }))
		{
			m_currentFrameIndex = 0;
			m_currentFrameTime = 0;
			m_selectedAnimation->m_index = 0;
			m_playAnimimation = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop##AnimStop", { 100, 35 }))
		{
			m_currentFrameIndex = 0;
			m_currentFrameTime = 0;
			m_selectedAnimation->m_index = 0;
			m_playAnimimation = false;
		}
		DrawAnimation();

		ImGui::EndTable();
	}

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
				std::vector<std::string> textureNames;
				SaveFile(s_animationName.c_str(), true, 1, 0, textureNames);
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

void AnimationWindow::DrawAnimation()
{
	if (m_selectedAnimation == nullptr || m_selectedAnimation->m_textures.size() < 1)
		return;

	m_currentFrameTime += (float)Firelight::Engine::Instance().GetTime().GetDeltaTime() * 1000.0f;
	Firelight::Graphics::Texture* texture = m_selectedAnimation->m_textures[m_selectedAnimation->m_index];
	if (texture == nullptr)
		return;

	Firelight::Maths::Vec3i imageDimensions = texture->GetDimensions();
	while (imageDimensions.x > ImGui::CalcItemWidth())
	{
		imageDimensions.x /= 2;
		imageDimensions.y /= 2;
	}
	ImGui::Image((ImTextureID)texture->GetShaderResourceView().Get(), ImVec2(static_cast<float>(imageDimensions.x), static_cast<float>(imageDimensions.y)));

	if (!m_playAnimimation)
		return;

	if (m_currentFrameTime > m_selectedAnimation->m_frameTime && m_playAnimimation)
	{
		if (!m_selectedAnimation->m_loop && m_selectedAnimation->m_index < m_selectedAnimation->m_frameCount - 1 || m_selectedAnimation->m_loop)
			m_selectedAnimation->m_index++;

		if (m_selectedAnimation->m_index >= m_selectedAnimation->m_frameCount && m_selectedAnimation->m_loop)
		{
			m_selectedAnimation->m_index = 0;
		}
		m_currentFrameTime = 0.0f;
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
	SaveFile(m_selectedAnimation->m_animationName.c_str(), m_selectedAnimation->m_loop,static_cast<int>( m_selectedAnimation->m_frameTime),
		m_selectedAnimation->m_frameCount, m_selectedAnimation->m_textureNames);
}

void AnimationWindow::SaveFile(const char* animName, bool loop, int frameTime, int frameCount, std::vector<std::string> textureNames)
{
	std::string fileName = animName;
	if (fileName.length() <= 0)
		return;

	fileName += ".anim";
	std::string newPath = "Assets/Animations/" + fileName;

	rapidjson::StringBuffer* buffer = new rapidjson::StringBuffer();
	Serialiser::Writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*buffer);

	Serialiser::StartObject();
	Serialiser::Serialise("AnimationName", animName);
	Serialiser::Serialise("Loop", loop);
	Serialiser::Serialise("FrameTime", frameTime);
	Serialiser::Serialise("FrameCount", frameCount);
	Serialiser::StartArray("TextureNames");
	for (int i = 0; i < textureNames.size(); i++)
	{
		Serialiser::Serialise(textureNames[i]);
	}
	Serialiser::EndArray();
	Serialiser::EndObject();
	SaveAnimation(newPath.c_str(), buffer);

	if (std::find(m_animationNames.begin(), m_animationNames.end(), animName) == m_animationNames.end())
	{
		Firelight::Animation::Animation* animation = new Firelight::Animation::Animation();
		animation->m_animationName = animName;
		animation->m_loop = loop;
		animation->m_frameTime = static_cast<float>(frameTime);
		animation->m_frameCount = frameCount;
		animation->m_textureNames = textureNames;

		m_animations.push_back(animation);
		m_animationNames.push_back(animName);
		m_selectedAnimation = animation;
	}
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

		//Firelight::Graphics::Texture* icon = nullptr;

		std::string extension = path.extension().string();
		if (extension == ".anim")
		{
			std::string filePath = path.string();
			// Load file and create new Animation struct
			if (Serialiser::LoadFile(path.string().c_str()))
			{
				Firelight::Animation::Animation* animation = new Firelight::Animation::Animation();
				Serialiser::Deserialise("AnimationName", animation->m_animationName);
				Serialiser::Deserialise("Loop", animation->m_loop);
				Serialiser::Deserialise("FrameTime", animation->m_frameTime);
				Serialiser::Deserialise("FrameCount", animation->m_frameCount);
				auto genericArray = (*Serialiser::FileDocument)["TextureNames"].GetArray();
				for (auto& value : genericArray)
				{
					std::string string = value.GetString();;
					animation->m_textureNames.push_back(string);
					animation->m_textures.push_back(Firelight::Graphics::AssetManager::Instance().GetTexture(string));
				}
				m_animations.push_back(animation);
				m_animationNames.push_back(animation->m_animationName);
			}
		}
	}
}
