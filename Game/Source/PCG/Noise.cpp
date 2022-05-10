#include "Noise.h"

Noise::Noise()
    : m_noiseData()
    , m_noise(nullptr)
{}

Noise::~Noise()
{
    delete m_noise;
}

void Noise::CreateNoise()
{
    m_noise = new FastNoiseLite(1337);
	m_noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    int index = 0;

    for (int y = 0; y < 128; ++y)
    {
        for (int x = 0; x < 128; ++x)
        {
            m_noiseData[index++] = m_noise->GetNoise((float)x, (float)y);
        }
    }
}

float* Noise::GetNoiseData()
{
    return m_noiseData;
}

// Use the noise data for the islands