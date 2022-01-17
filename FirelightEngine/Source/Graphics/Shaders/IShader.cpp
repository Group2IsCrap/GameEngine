
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
        if (!shaderFilePath || !entryPoint || !profile)
        {
            // Using assert for the above 'if' makes D3DCompileFromFile complain
            ERROR("Failed to load shader at path: " + std::string(shaderFilePath));
            return false;
        }

        m_path = shaderFilePath;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
        flags |= D3DCOMPILE_DEBUG;
#endif

        const D3D_SHADER_MACRO defines[] =
        {
            "EXAMPLE_DEFINE", "1", NULL, NULL
        };

        ID3DBlob* errorBlob = nullptr;
        HRESULT hr = D3DCompileFromFile(Utils::StringHelpers::StringToWide(m_path).c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint, profile, flags, 0, m_shaderBuffer.GetAddressOf(), &errorBlob);

        std::string errorString;
        if (errorBlob)
        {
            errorString = (char*)errorBlob->GetBufferPointer();
            errorBlob->Release();
        }
        COM_ERROR_RETURN_IF_FAILED(hr, "Failed to read shader at path: " + m_path + "\nShader Error: " + errorString, false);

        return true;
	}

	ID3D10Blob* IShader::GetBuffer()
	{
		return m_shaderBuffer.Get();
	}
}