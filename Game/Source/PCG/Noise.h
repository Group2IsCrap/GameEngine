#pragma once

#include "FastNoiseLite/FastNoiseLite.h"

#define NOISE_DATA_SIZE 128

class Noise
{
public:
	Noise();
	~Noise();

	void CreateNoise();
	float* GetNoiseData();

	int GetSeed();
	void SetSeed(int seed);

	float GetNoiseScale();
	void SetNoiseScale(float noiseScale);

private:
	float m_noiseData[NOISE_DATA_SIZE * NOISE_DATA_SIZE];

    FastNoiseLite* m_noise;
	float m_noiseScale;
	int m_seed;
};