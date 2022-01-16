
#include "IShader.h"

#include "../GraphicsHandler.h"

namespace Firelight::Graphics
{
	IShader::~IShader()
	{
	}

	bool IShader::Initialise(const std::string& shaderFilePath)
	{
		m_path = shaderFilePath;

		HRESULT hr = D3DReadFileToBlob(Utils::StringHelpers::StringToWide(m_path).c_str(), m_shaderBuffer.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to read shader at path: " + m_path, false);

		return true;
	}

	ID3D10Blob* IShader::GetBuffer()
	{
		return m_shaderBuffer.Get();
	}
}