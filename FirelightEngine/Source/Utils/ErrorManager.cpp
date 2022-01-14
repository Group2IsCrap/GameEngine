#include "ErrorManager.h"

namespace Firelight::Utils
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

    void ErrorManager::LogError(const Firelight::Error* error)
    {
        m_errors.emplace_back(error);
    }

    void ErrorManager::DisplayError(const Firelight::Error* error)
    {
        std::wstring errorMessage = StringHelpers::StringToWide(error->what());
        MessageBoxW(NULL, errorMessage.c_str(), error->GetName(), MB_ICONERROR);
    }

    const std::vector<std::unique_ptr<const Firelight::Error>>& ErrorManager::GetErrors() const
    {
        return m_errors;
    }
}
