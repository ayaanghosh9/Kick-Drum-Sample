#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

int main() {

    int sampleRate = 44100;
    float startfreq = 100;
    float targetfreq = 50;
    int duration = 1.0;
    int totalSamples=duration*sampleRate;
    float samples[44100*duration];
    float decayRate = 20.0;
    float pitchdecay = 8.0;
    float holdtime = 0.00005;
    float noiseholdtime = 0.0005;
    for (int i=0; i<=totalSamples; i++){
        float time = (float)i/sampleRate;
        float envelope;
        float noiseEnvelope;
        float frequency = targetfreq+((startfreq - targetfreq)*exp(-time*pitchdecay));
        if (time < holdtime) {
            envelope = 1.0;
        }   else {
            envelope = exp(decayRate * -(time - holdtime));
            }
        if (time < noiseholdtime) {
            noiseEnvelope = 1.0;
        }   else {
            noiseEnvelope = exp(-time*40.0);
            }
        float noise = (((float)rand() / RAND_MAX) * 2.0 - 1.0) * 0.5;
        samples[i] = 
        (cos(2*M_PI*frequency*time) * 0.55 * envelope) 
        + (noise * noiseEnvelope * 0.05)
        + (cos(2*M_PI*frequency*time*2) * 0.05 * envelope) 
        + (cos(2*M_PI*frequency*time*3) * 0.05 * envelope)
        + (cos(2*M_PI*frequency*time*0.5) * 0.3 * envelope);
    }

    float maxSample = 0.0;
    for (int i = 0; i < totalSamples; i++) {
        if (fabs(samples[i]) > maxSample) {
            maxSample = fabs(samples[i]);
        }
    }

    std::cout << "Max sample value: " << maxSample << std::endl;

    std::ofstream file("kick.wav", std::ios::binary);
    int chunkSize = 36 + totalSamples * 2 * duration;
    short audioFormat = 1;
    short numChannels = 1;
    int byteRate = sampleRate * 2;
    short blockAlign = 2;
    short bitsPerSample = 16;
    int dataSize = totalSamples * 2;
    int fmtSize = 16;

    file.write("RIFF", 4);
    file.write((char*)&chunkSize, 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    file.write((char*)&fmtSize, 4);
    file.write((char*)&audioFormat, 2);
    file.write((char*)&numChannels, 2);
    file.write((char*)&sampleRate, 4);
    file.write((char*)&byteRate, 4);
    file.write((char*)&blockAlign, 2);
    file.write((char*)&bitsPerSample, 2);
    file.write("data", 4);
    file.write((char*)&dataSize, 4);

    for (int i = 0; i < totalSamples; i++) {
    short sample = samples[i] * 32767;
    file.write((char*)&sample, 2);
    }

    file.close();
    std::cout << "kick.wav created" << std::endl;
    return 0;
}