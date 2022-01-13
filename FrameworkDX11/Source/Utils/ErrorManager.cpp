#include "ErrorManager.h"

namespace TempEngineNamespace::Utils
{
    ErrorManager::ErrorManager()
    {
    }

    ErrorManager::~ErrorManager()
    {
    }

    ErrorManager& ErrorManager::Instance()
    {
        static ErrorManager instance;
        return instance;
    }

    void ErrorManager::LogError(const TempEngineNamespace::Error* error)
    {
        mErrors.emplace_back(error);
    }

    void ErrorManager::DisplayError(const TempEngineNamespace::Error* error)
    {
        std::wstring errorMessage = StringHelpers::StringToWide(error->what());
        MessageBoxW(NULL, errorMessage.c_str(), error->GetName(), MB_ICONERROR);
    }

    const std::vector<std::unique_ptr<const TempEngineNamespace::Error>>& ErrorManager::GetErrors() const
    {
        return mErrors;
    }
}
