#pragma once

#include <windows.h>
#include <comdef.h>
#include <string>

#include "../StringHelpers.h"

namespace Firelight
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
			m_errorType(errorType),
			m_message(msg),
			m_fileName(Utils::StringHelpers::RemoveBeforeString(fileName, "GameEngine")),
			m_functionName(functionName),
			m_line(line)
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
			m_whatMessage += "\nFile: " + m_fileName + "\n";
			m_whatMessage += "Function: " + m_functionName + "\n";
			m_whatMessage += "Line: " + std::to_string(m_line);
		}

		virtual void ConstructWhatMessage()
		{
			m_whatMessage += "Msg: " + m_message + "\n";
		}

		ErrorType GetType() const
		{
			return m_errorType;
		}

		const wchar_t* GetName() const
		{
			switch (m_errorType)
			{
			case ErrorType::e_Standard:
				return L"Error";
			case ErrorType::e_COM:
				return L"COM Error";
			case ErrorType::e_Assertion:
				return L"Assertion Failed";
			}

			return L"Unknown Error Type";
		}

		const char* what() const override
		{
			return m_whatMessage.c_str();
		}

	protected:
		ErrorType          m_errorType;

		const std::string m_message;
		const std::string m_fileName;
		const std::string m_functionName;
		const int         m_line;

		std::string m_whatMessage;
	};
}
