#pragma once

#include <stdlib.h>
#include <time.h>

namespace Firelight::Maths::Random
{
	static void SeedWithCurrentTime()
	{
		srand((unsigned int)time(nullptr));
	}

	template<typename T>
	static T ZeroToOne()
	{
		return (T)(rand() % RAND_MAX) / (T)(RAND_MAX);
	}

	template<typename T>
	static T NegOneToOne()
	{
		return ZeroToOne<T>() * (T)2.0 - (T)1.0;
	}
}