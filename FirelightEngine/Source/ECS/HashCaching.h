#pragma once

#include <map>
#include <string>

class HashCaching
{
public:
	static size_t GetHash(std::string hash);

public:

	static std::map<std::string, size_t> m_hashMap;
};
