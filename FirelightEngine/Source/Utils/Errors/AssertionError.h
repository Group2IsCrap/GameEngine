#pragma once

#include "Error.h"

namespace Firelight
{
	struct AssertionError : Error
	{
	public:
		AssertionError(const char* assertion, const std::string& msg, const std::string& fileName, const std::string& functionName, int line) :
			m_assertion(assertion),
			Error(msg, fileName, functionName, line, ErrorType::e_Assertion)
		{
		}

		virtual void ConstructWhatMessage() override
		{
			m_whatMessage += "Assertion: " + m_assertion + "\n";

			Error::ConstructWhatMessage();
		}

	private:
		std::string m_assertion;
	};
}
