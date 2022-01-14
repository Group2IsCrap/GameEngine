#pragma once

#include <windows.h>
#include <comdef.h>
#include <string>

#include "../StringHelpers.h"

namespace TempEngineNamespace
{
	enum class ErrorType
	{
		e_Standard,
		e_COM,
		e_Assertion
	};

	struct Error : std::exception
	{
	public:
		Error(const std::string& msg, const std::string& fileName, const std::string& functionName, int line, ErrorType errorType = ErrorType::e_Standard) :
			mErrorType(errorType),
			mMessage(msg),
			mFileName(Utils::StringHelpers::RemoveBeforeString(fileName, "FrameworkDX11")),
			mFunctionName(functionName),
			mLine(line)
		{
		}

		virtual ~Error()
		{
		}

		void Initialise()
		{
			ConstructWhatMessage();
			AddCodeAreaToMessage();
		}

		void AddCodeAreaToMessage()
		{
			mWhatMessage += "\nFile: " + mFileName + "\n";
			mWhatMessage += "Function: " + mFunctionName + "\n";
			mWhatMessage += "Line: " + std::to_string(mLine);
		}

		virtual void ConstructWhatMessage()
		{
			mWhatMessage += "Msg: " + mMessage + "\n";
		}

		ErrorType GetType() const
		{
			return mErrorType;
		}

		const wchar_t* GetName() const
		{
			switch (mErrorType)
			{
			case ErrorType::e_Standard:
				return L"Error";
			case ErrorType::e_COM:
				return L"HR Failed";
			case ErrorType::e_Assertion:
				return L"Assertion Failed";
			}

			return L"Unknown Error Type";
		}

		const char* what() const override
		{
			return mWhatMessage.c_str();
		}

	protected:
		ErrorType          mErrorType;

		const std::string mMessage;
		const std::string mFileName;
		const std::string mFunctionName;
		const int         mLine;

		std::string mWhatMessage;
	};
}
