#pragma once

#include "../../Maths/Vec4.h"

#include <DirectXMath.h>

namespace Firelight::Graphics
{
	struct DirectionalLightCB
	{
		DirectX::XMFLOAT3 m_colour;
		float             m_ambientStrength;

		DirectX::XMFLOAT3 m_direction;
		float             m_padding1;
	};

	struct PointLightCB
	{
		DirectX::XMFLOAT3 m_position;
		float             m_padding1;

		DirectX::XMFLOAT3 m_colour;
		float             m_padding2;

		float             m_attenuationConstant;
		float             m_attenuationLinear;
		float             m_attenuationQuadratic;
		float             m_padding3;
	};

	struct SpotLightCB
	{
		DirectX::XMFLOAT3 m_position;
		float             m_padding1;

		DirectX::XMFLOAT3 m_colour;
		float             m_padding2;

		float             m_attenuationConstant;
		float             m_attenuationLinear;
		float             m_attenuationQuadratic;
		float             m_innerCutoff;

		float             m_outerCutoff;
		DirectX::XMFLOAT3 m_direction;
	};

	struct SceneCB
	{
		int               m_numPointLights;
		int               m_numSpotLights;
		DirectX::XMFLOAT2 m_padding1;

		DirectionalLightCB m_directionalLight;
		PointLightCB       m_pointLights[10];
		SpotLightCB        m_spotLights[20];
	};

	struct PerFrameCB
	{
		float             m_gameTime;
		DirectX::XMFLOAT3 m_padding1;
	};

	struct PerPassCB
	{
		DirectX::XMFLOAT3 m_cameraPosition;
		float             m_padding1;

		DirectX::XMMATRIX m_viewProjectionMatrix;

		float             m_nearPlane;
		float             m_farPlane;
		DirectX::XMFLOAT2 m_padding2;
	};

	struct PerMaterialCB
	{
		DirectX::XMFLOAT4 m_colour;
	};

	struct PerObjectCB
	{
		DirectX::XMMATRIX m_modelMatrix;
	};

	struct TextCB
	{
		Maths::Vec2f      m_positionOffset;
		Maths::Vec2f      m_scale;

		float             m_antiAliasDistanceRange; // ? -> ? | Calculated by text code
		float             m_thickness; // 0 -> 1 | Set via text code
		int               m_hasOutline;
		float             m_outlineWidth;

		Maths::Vec4f      m_colour;
		Maths::Vec4f      m_outlineColour;
	};
}
