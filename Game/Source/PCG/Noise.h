#pragma once

#include "FastNoiseLite/FastNoiseLite.h"

class Noise
{
public:
	Noise();
	~Noise();

	void CreateNoise();
	float* GetNoiseData();

private:
	float m_noiseData[128 * 128];

    FastNoiseLite* m_noise;
};