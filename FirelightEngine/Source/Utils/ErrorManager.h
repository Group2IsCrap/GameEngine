#pragma once

#include <vector>
#include <memory>

#include "Errors/COMError.h"
#include "Errors/AssertionError.h"

namespace Firelight::Utils
{
    class ErrorManager
    {
	public:
		~ErrorManager();
		static ErrorManager& Instance();

		void LogError(const Firelight::Error* error);
		void DisplayError(const Firelight::Error* error);

		const std::vector<std::unique_ptr<const Error>>& GetErrors() const;

	private:
		ErrorManager();

	private:
		std::vector<std::unique_ptr<const Firelight::Error>> m_errors;
    };
}

// Internal macro to log an existing error
#define INTERNAL_LOG_ERROR(err) Firelight::Utils::ErrorManager::Instance().LogError(err)
// Internal macro to display an existing error
#define INTERNAL_DISPLAY_ERROR(err) Firelight::Utils::ErrorManager::Instance().DisplayError(err)
// Internal macro to allocate a standard error
#define INTERNAL_ALLOCATE_STANDARD_ERROR(msg) Firelight::Error* err = new Firelight::Error(msg, __FILE__, __FUNCTION__, __LINE__); err->Initialise()
// Internal macro to allocate a COM error
#define INTERNAL_ALLOCATE_COM_ERROR(hr, msg) Firelight::COMError* err = new Firelight::COMError(hr, msg, __FILE__, __FUNCTION__, __LINE__); err->Initialise()
// Internal macro to allocate an assertion error
#define INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg) Firelight::AssertionError* err = new Firelight::AssertionError(#assertion, msg, __FILE__, __FUNCTION__, __LINE__); err->Initialise()

// -------------------------------------------------------------------------------------
// Standard errors
// -------------------------------------------------------------------------------------

// Log an error without a message box
#define ERROR_SILENT(msg)\
{\
	INTERNAL_ALLOCATE_STANDARD_ERROR(hr, msg);\
	INTERNAL_LOG_ERROR(err);\
}

// Alert the user of an error and log it
#define ERROR_STANDARD(msg)\
{\
	INTERNAL_ALLOCATE_STANDARD_ERROR(msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
}

// Alert the user of an error, log it and throw it
#define ERROR_THROW(msg)\
{\
	INTERNAL_ALLOCATE_STANDARD_ERROR(msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	throw *err;\
}

// Alert the user of an error, log it and throw it
#define ERROR_THROW(msg)\
{\
	INTERNAL_ALLOCATE_STANDARD_ERROR(msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	throw *err;\
}

// Alert the user of an error and quit the application
#define ERROR_FATAL(msg)\
{\
	INTERNAL_ALLOCATE_STANDARD_ERROR(msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	exit(-1);\
}

// -------------------------------------------------------------------------------------
// Assertions
// -------------------------------------------------------------------------------------

// If the assertion fails, log an assertion error without a message box
#define ASSERT_SILENT(assertion, msg)\
if (!(assertion))\
{\
	INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg);\
	INTERNAL_LOG_ERROR(err);\
}

// If the assertion fails, alert the user of an assertion error and log it
#define ASSERT(assertion, msg)\
if (!(assertion))\
{\
	INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
}

// If the assertion fails, alert the user of an assertion error, log it and throw it
#define ASSERT_THROW(assertion, msg)\
if (!(assertion))\
{\
	INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	throw *err;\
}

// If the assertion fails, alert the user of an assertion error, log it and return a value
#define ASSERT_RETURN(assertion, msg, retVal)\
if (!(assertion))\
{\
	INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	return retVal;\
}

// If the assertion fails, alert the user of an assertion error and quit the application
#define ASSERT_FATAL(assertion, msg)\
if (!(assertion))\
{\
	INTERNAL_ALLOCATE_ASSERTION_ERROR(assertion, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	exit(-1);\
}

// -------------------------------------------------------------------------------------
// Standard COM Errors
// -------------------------------------------------------------------------------------

// Log a COM error without a message box
#define COM_ERROR_SILENT(hr, msg)\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_LOG_ERROR(err);\
}

// Alert the user and log a COM error
#define COM_ERROR(hr, msg)\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
}

// Alert the user and throw a COM error
#define COM_ERROR_THROW(hr, msg)\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	throw *err;\
}

// Alert the user of a COM error and quit the application
#define COM_ERROR_FATAL(hr, msg)\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	exit(-1);\
}

// -------------------------------------------------------------------------------------
// Conditional COM Errors
// -------------------------------------------------------------------------------------

// If the HRESULT fails, log a COM error without a message box
#define COM_ERROR_SILENT_IF_FAILED(hr, msg)if (FAILED(hr))\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_LOG_ERROR(err);\
}

// If the HRESULT fails, alert the user of a COM error and log it
#define COM_ERROR_IF_FAILED(hr, msg)if (FAILED(hr))\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
}

// If the HRESULT fails, alert the user of a COM error, log it and throw it
#define COM_ERROR_THROW_IF_FAILED(hr, msg)if (FAILED(hr))\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	throw *err;\
}

// If the HRESULT fails, alert the user of a COM error, log it and return a value
#define COM_ERROR_RETURN_IF_FAILED(hr, msg, retValue) if (FAILED(hr))\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	INTERNAL_LOG_ERROR(err);\
	return retValue;\
}

// If the HRESULT fails, alert the user of a COM error and quit the application
#define COM_ERROR_FATAL_IF_FAILED(hr, msg) if (FAILED(hr))\
{\
	INTERNAL_ALLOCATE_COM_ERROR(hr, msg);\
	INTERNAL_DISPLAY_ERROR(err);\
	exit(-1);\
}
