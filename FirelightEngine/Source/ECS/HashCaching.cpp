#include "HashCaching.h"

std::map<std::string, size_t> HashCaching::m_hashMap = {};

size_t HashCaching::GetHash(std::string hash)
{
	if (!m_hashMap.contains(hash))
	{
		// Add new hash entry
		m_hashMap[hash] = std::hash<std::string>{}(hash);
	}

	return m_hashMap[hash];
}
