#include "ComputeShader.h"

#include "../GraphicsHandler.h"

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	bool ComputeShader::Initialise(std::string shaderFilePath)
	{
		IShader::Initialise(shaderFilePath);

		HRESULT hr;
		hr = GraphicsHandler::Instance().GetDevice()->CreateComputeShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create ID3D11ComputeShader for: " + m_path, false);

		return true;
	}

	ID3D11ComputeShader* ComputeShader::GetShader()
	{
		return m_shader.Get();
	}
}
