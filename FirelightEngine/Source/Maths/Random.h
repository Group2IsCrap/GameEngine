#pragma once

#include <stdlib.h>
#include <time.h>

// Stop the compiler complaining about SeedWithCurrentTime not being used when it is
#pragma warning(disable : 4505)

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

	template<typename T>
	static T InRange(T minVal, T maxVal)
	{
		return minVal + ZeroToOne<T>() * (maxVal - minVal);
	}
}
