#pragma once

#include "Error.h"

#include <locale>
#include <codecvt>

namespace TempEngineNamespace
{
	struct COMError : Error
	{
	public:
		COMError(HRESULT hr, const std::string& msg, const std::string& fileName, const std::string& functionName, int line) :
			mHResult(hr),
			Error(msg, fileName, functionName, line, ErrorType::e_COM)
		{
		}

		virtual void ConstructWhatMessage() override
		{
			Error::ConstructWhatMessage();

			_com_error error(mHResult);

			mWhatMessage += "HR: " + Utils::StringHelpers::WideStringToString(error.ErrorMessage()) + "\n";
		}

	private:
		HRESULT mHResult;
	};
}
