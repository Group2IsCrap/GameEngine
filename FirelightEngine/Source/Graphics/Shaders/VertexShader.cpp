#include "VertexShader.h"

#include "../GraphicsHandler.h"

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	bool VertexShader::Initialise(std::string shaderFilePath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
	{
		IShader::Initialise(shaderFilePath);

		HRESULT hr;
		hr = GraphicsHandler::Instance().GetDevice()->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create ID3D11VertexShader for: " + m_path, false);

		hr = GraphicsHandler::Instance().GetDevice()->CreateInputLayout(layoutDesc, numElements, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create shader input layout for: " + m_path, false);

		return true;
	}

	ID3D11VertexShader* VertexShader::GetShader()
	{
		return m_shader.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout()
	{
		return m_inputLayout.Get();
	}
}
