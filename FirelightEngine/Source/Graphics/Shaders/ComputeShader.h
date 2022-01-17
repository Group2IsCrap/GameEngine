#pragma once

#include "IShader.h"

namespace Firelight::Graphics
{
	class ComputeShader : public IShader
	{
	public:
		bool                 Initialise(const char* shaderFilePath);
		ID3D11ComputeShader* GetShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_shader;
	};
}
