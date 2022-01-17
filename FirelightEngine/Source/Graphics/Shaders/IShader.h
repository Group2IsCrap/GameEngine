
//Stores definitions for the VertexShader and PixelShader classes

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include "../../Utils/ErrorManager.h"

namespace Firelight::Graphics
{
	class IShader
	{
	public:
		IShader();
		virtual ~IShader();

		bool         Initialise(const char* shaderFilePath, const char* entryPoint, const char* profile);
		ID3D10Blob*  GetBuffer();

	protected:
		Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
		std::string                        m_path;
	};
}
