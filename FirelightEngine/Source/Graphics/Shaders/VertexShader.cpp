#include "VertexShader.h"

#include <d3dcompiler.h>

#include "../GraphicsHandler.h"

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	bool VertexShader::Initialise(const char* shaderFilePath)
	{
        if (!IShader::Initialise(shaderFilePath, "main", "vs_5_0"))
        {
            return false;
        }

		HRESULT hr;
		hr = GraphicsHandler::Instance().GetDevice()->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create ID3D11VertexShader for: " + m_path, false);

        bool result = GenerateInputLayoutFromShaderBlob();

		return result;
	}

	bool VertexShader::GenerateInputLayoutFromShaderBlob()
	{
        // Reflect shader info
        Microsoft::WRL::ComPtr<ID3D11ShaderReflection> vertexShaderReflection;
        HRESULT hr = D3DReflect(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)vertexShaderReflection.GetAddressOf());
        COM_ERROR_RETURN_IF_FAILED(hr, "Failed to generate input layout for vertex shader because D3DReflect failed. Shader path: " + m_path, false);

        // Get shader info
        D3D11_SHADER_DESC shaderDesc;
        vertexShaderReflection->GetDesc(&shaderDesc);

        // Read input layout description from shader info
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescriptions;
        for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

            // Fill out input element desc
            D3D11_INPUT_ELEMENT_DESC elementDesc;
            elementDesc.SemanticName = paramDesc.SemanticName;
            elementDesc.SemanticIndex = paramDesc.SemanticIndex;
            elementDesc.InputSlot = 0;
            elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;

            // Determine DXGI format
            if (paramDesc.Mask == 1)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
            }
            else if (paramDesc.Mask <= 3)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
            }
            else if (paramDesc.Mask <= 7)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            }
            else if (paramDesc.Mask <= 15)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            }

            // Save element desc
            inputLayoutDescriptions.push_back(elementDesc);
        }

        // Try to create Input Layout
        hr = GraphicsHandler::Instance().GetDevice()->CreateInputLayout(&inputLayoutDescriptions[0], (UINT)inputLayoutDescriptions.size(), m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
        COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create input layout for vertex shader at path: " + m_path, false);

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
