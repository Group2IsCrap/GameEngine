#pragma once

#include "IShader.h"

namespace Firelight::Graphics
{
	class PixelShader : public IShader
	{
	public:
		bool               Initialise(const char* shaderFilePath);
		ID3D11PixelShader* GetShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	};
}
