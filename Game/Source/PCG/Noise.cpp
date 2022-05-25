#include "Noise.h"

Noise::Noise()
    : m_noiseData()
    , m_noise(nullptr)
    , m_noiseScale(75.5f)
    , m_seed(2000)
{}

Noise::~Noise()
{
    delete m_noise;
}

void Noise::CreateNoise()
{
    m_noise = new FastNoiseLite(m_seed);
	m_noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    int index = 0;

    for (size_t y = 0; y < NOISE_DATA_SIZE; ++y)
    {
        for (size_t x = 0; x < NOISE_DATA_SIZE; ++x)
        {
            m_noiseData[index++] = m_noise->GetNoise((float)x * m_noiseScale, (float)y * m_noiseScale);
        }
    }
}

float* Noise::GetNoiseData()
{
    return m_noiseData;
}

int Noise::GetSeed()
{
    return m_seed;
}

void Noise::SetSeed(int seed)
{
    m_seed = seed;
}

float Noise::GetNoiseScale()
{
    return m_noiseScale;
}

void Noise::SetNoiseScale(float noiseScale)
{
    m_noiseScale = noiseScale;
}