#pragma once

#include <stdlib.h>
#include <time.h>
#include <random>
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
		return minVal + (T)(ZeroToOne<double>() * (double)(maxVal - minVal));
	}

	//randome int
	template<typename T>
	T RandomRange(T range_from, T range_to) {
		std::random_device   rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<T>    distr(range_from, range_to);
		return distr(generator);
	}
	//randome float,double
	template<typename T>
	T RandomFloatRange(T range_from, T range_to) {
		std::random_device   rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_real_distribution<T>    distr(range_from, range_to);
		return distr(generator);
	}
}
