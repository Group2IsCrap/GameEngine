#pragma once

#include "IShader.h"

namespace Firelight::Graphics
{
	class VertexShader : public IShader
	{
	public:
		bool                Initialise(const char* shaderFilePath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);
		ID3D11VertexShader* GetShader();
		ID3D11InputLayout*  GetInputLayout();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;
	};
}
