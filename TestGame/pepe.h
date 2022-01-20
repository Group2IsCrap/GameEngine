#pragma once

#include "Source/Maths/Vec2.h"
#include "Source/Maths/Rect.h"

#include "Source/Graphics/Data/Colour.h"
#include "Source/Graphics/Data/Texture.h"

#include "Source/Graphics/GraphicsHandler.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/SpriteBatch.h"

using namespace Firelight;

struct Pepe
{
	Pepe() :
		m_pos(0),
		m_vel(Maths::Vec2f::GetRandomVector() * 300.0f),
		m_rot(0.0f),
		m_rotSpeed(Maths::Random::NegOneToOne<float>() * 10.0f),
		m_colour((unsigned int)0),
		m_layer((int)(Maths::Random::ZeroToOne<float>() * 64.0f)),
		m_darkness((float)m_layer / 64.0f),
		m_hue(Maths::Random::ZeroToOne<float>()),
		m_texture(Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/transparency_test.png"))
	{
	}

	void Update(double deltaTime, const Maths::Vec2f& windowDimensions)
	{
		m_pos += m_vel * (float)deltaTime;

		if (m_pos.x > windowDimensions.x + 100.0f) m_pos.x -= windowDimensions.x + 200.0f;
		if (m_pos.x < -100.0f) m_pos.x += windowDimensions.x + 200.0f;
		if (m_pos.y > windowDimensions.y + 100.0f) m_pos.y -= windowDimensions.y + 200.0f;
		if (m_pos.y < -100.0f) m_pos.y += windowDimensions.y + 200.0f;

		m_rot += m_rotSpeed * (float)deltaTime;

		m_hue += (float)deltaTime * 0.3f;
		if (m_hue > 1.0f) m_hue -= 1.0f;

		Graphics::Colour::HSLA hslValue = Graphics::Colour::HSLA((BYTE)(m_hue * 255.0f), 255, (BYTE)(m_darkness * 128.0f), 128);
		m_colour = Graphics::Colour::RGBA::MakeFromHSLA(hslValue);
	}

	void Draw()
	{
		Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(m_pos.x - 100.0f, m_pos.y - 100.0f, 200.0f, 200.0f), m_texture, m_layer, (double)m_rot, m_colour);
	}

	Maths::Vec2f           m_pos;
	Maths::Vec2f           m_vel;
	float                             m_rot;
	float                             m_rotSpeed;
	Graphics::Colour::RGBA m_colour;
	int                               m_layer;
	float                             m_darkness;
	float                             m_hue;
	Graphics::Texture*     m_texture;
};
