#pragma once

#include <string>
#include <algorithm>

// Get it to stop complaining about some of these functions not being used
#pragma warning(disable : 4505)

namespace Firelight::Utils::StringHelpers
{
	static void StringToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return (char)std::tolower(c); });
	}

	static std::wstring StringToWide(const std::string& str)
	{
		std::wstring wideString(str.begin(), str.end());
		return wideString;
	}

	static std::string WideStringToString(const std::wstring& wideString)
	{
		if (wideString.empty())
		{
			return std::string();
		}

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wideString[0], (int)wideString.size(), NULL, 0, NULL, NULL);
		std::string returnString(sizeNeeded, 0);

		WideCharToMultiByte(CP_UTF8, 0, &wideString[0], (int)wideString.size(), &returnString[0], sizeNeeded, NULL, NULL);

		return returnString;
	}

	static std::string RemoveBeforeString(const std::string& string, const char* stringToFind)
	{
		std::size_t foundPosition = string.find(stringToFind);
		if (foundPosition != std::string::npos)
		{
			return string.substr(foundPosition);
		}

		return string;
	}

	static std::string GetDirectoryFromPath(const std::string& filePath)
	{
		size_t off1 = filePath.find_last_of('\\');
		size_t off2 = filePath.find_last_of('/');

		if (off1 == std::string::npos && off2 == std::string::npos)
		{
			return "";
		}

		if (off1 == std::string::npos)
		{
			return filePath.substr(0, off2);
		}

		if (off2 == std::string::npos)
		{
			return filePath.substr(0, off1);
		}

		return filePath.substr(0, max(off1, off2));
	}

	static std::string GetFileExtension(const std::string& fileName)
	{
		size_t offset = fileName.find_last_of(".");

		if (offset == std::string::npos)
		{
			return "";
		}

		return std::string(fileName.substr(offset + 1));
	}

	static std::string ChangeFileExtension(const std::string& filePath, const char* newExtensionNoDot)
	{
		int numChars = static_cast<int>(filePath.size());
		for (int i = numChars - 1; i >= 0; --i)
		{
			if (filePath[i] == '.')
			{
				return filePath.substr(0, static_cast<size_t>(i) + 1) + newExtensionNoDot;
			}
		}
		return "";
	}

	static void RemoveDirectoriesFromStart(std::string& filePath, int numDirectories)
	{
		int numDirectoriesFound = 0;
		size_t strLength = filePath.size();
		for (size_t i = 0; i < strLength; ++i)
		{
			if (filePath[i] == '\\' || filePath[i] == '/')
			{
				if (++numDirectoriesFound >= numDirectories)
				{
					filePath = filePath.substr(i + 1, strLength - 1 - i);
					return;
				}
			}
		}
	}

	static void ReplaceChars(std::string& string, char existingChar, char newChar)
	{
		size_t stringSize = string.length();
		for (size_t i = 0; i < stringSize; ++i)
		{
			if (string[i] == existingChar)
			{
				string[i] = newChar;
			}
		}
	}

	static int GetLineNumberFromOffset(std::string& string, size_t charOffset)
	{
		int lineNumber = 0;
		size_t numIterations = min(string.length(), charOffset);
		for (size_t i = 0; i < numIterations; ++i)
		{
			if (string[i] == '\n') ++lineNumber;
		}

		return lineNumber;
	}
}
