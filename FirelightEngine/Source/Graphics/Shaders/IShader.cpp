
#include "IShader.h"

#include "../GraphicsHandler.h"
#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	IShader::IShader() :
		m_shaderBuffer(nullptr),
		m_path("")
	{
	}

	IShader::~IShader()
	{
	}

	bool IShader::Initialise(const char* shaderFilePath, const char* entryPoint, const char* profile)
	{
		m_path = shaderFilePath;

		/*HRESULT hr = D3DReadFileToBlob(Utils::StringHelpers::StringToWide(m_path).c_str(), m_shaderBuffer.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to read shader at path: " + m_path, false);*/

        if (!shaderFilePath || !entryPoint || !profile)
        {
            ERROR("Failed to load shader at path");
        }
            return E_INVALIDARG;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
        flags |= D3DCOMPILE_DEBUG;
#endif

        const D3D_SHADER_MACRO defines[] =
        {
            "EXAMPLE_DEFINE", "1",
            NULL, NULL
        };

        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint, profile,
            flags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            if (errorBlob)
            {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();

            return hr;
        }

        m_shaderBuffer = shaderBlob;

        return hr;
	}

	ID3D10Blob* IShader::GetBuffer()
	{
		return m_shaderBuffer.Get();
	}
}