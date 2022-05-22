#pragma once

#define PI 3.14159265358979323846f

#include <stdlib.h>
#include <time.h>
#include <cmath>
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

	template<typename T>
	static T InsideRandomUnitCircle()
	{
		float a = ZeroToOne<float>();
		float b = ZeroToOne<float>();
		return T(b * 1.0f * std::cos(2.0f * PI * a / b), b * 1.0f * std::sin(2.0f * PI * a / b), 0.0f);
	}
	
	template<typename T>
	static T RandomPointInCircle(T center, float radius)
	{
		float r = radius * sqrtf(ZeroToOne<float>());
		float theta = ZeroToOne<float>() * 2 * PI;

		return T(center.x + r * std::cosf(theta), center.y + r * std::sinf(theta), 0);
		//float a = ZeroToOne<float>();
		//float b = ZeroToOne<float>();
		//return T(b * radius * std::cos(2.0f * PI * a / b), b * radius * std::sin(2.0f * PI * a / b), 0.0f);
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
