#include "PixelShader.h"

#include "../GraphicsHandler.h"

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	bool PixelShader::Initialise(std::string shaderFilePath)
	{
		IShader::Initialise(shaderFilePath);

		HRESULT hr;
		hr = GraphicsHandler::Instance().GetDevice()->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create ID3D11PixelShader for: " + m_path, false);

		return true;
	}

	ID3D11PixelShader* PixelShader::GetShader()
	{
		return m_shader.Get();
	}
}