#pragma once

#include "Error.h"

namespace TempEngineNamespace
{
	struct AssertionError : Error
	{
	public:
		AssertionError(const char* assertion, const std::string& msg, const std::string& fileName, const std::string& functionName, int line) :
			mAssertion(assertion),
			Error(msg, fileName, functionName, line, ErrorType::e_Assertion)
		{
		}

		virtual void ConstructWhatMessage() override
		{
			mWhatMessage += "Assertion: " + mAssertion + "\n";

			Error::ConstructWhatMessage();
		}

	private:
		std::string mAssertion;
	};
}
