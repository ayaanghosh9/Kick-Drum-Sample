# Kick Drum Synthesizer

A procedural kick drum synthesizer written in C++ that generates a `.wav` file using FM synthesis, harmonic layering, and pitch envelope decay — no samples, no libraries, just math.

## How It Works

The synthesizer models a kick drum by combining:

- **Pitch sweep** — frequency decays exponentially from a start frequency down to a target frequency, simulating the characteristic drop of a real kick
- **Amplitude envelope** — brief hold at full volume followed by exponential decay
- **Harmonic layering** — fundamental + 2nd and 3rd harmonics + a sub-octave component for low-end weight
- **Transient noise burst** — short white noise layer at the attack for click/impact

The output is written directly as a 16-bit mono PCM WAV file with no external dependencies.

## Parameters

| Parameter | Default | Description |
|---|---|---|
| `startfreq` | 100 Hz | Initial pitch at the start of the kick |
| `targetfreq` | 50 Hz | Settled pitch at the end of the decay |
| `pitchdecay` | 8.0 | Rate of pitch drop (higher = faster sweep) |
| `decayRate` | 20.0 | Amplitude decay rate |
| `holdtime` | 0.00005s | Duration of full-amplitude hold before decay |
| `noiseholdtime` | 0.0005s | Duration of transient noise burst |
| `duration` | 1.0s | Total length of the output |
| `sampleRate` | 44100 Hz | Sample rate |

## Build & Run

**Requires:** A C++ compiler (g++, clang++, MSVC)

```bash
g++ Kick.cpp -o kick
./kick
```

This produces `kick.wav` in the current directory, playable in any audio player.

## Signal Chain

```
Pitch envelope (exp decay)
    → cos(2π × frequency × time)        × 0.55  [fundamental]
    + cos(2π × frequency × time × 2)    × 0.05  [2nd harmonic]
    + cos(2π × frequency × time × 3)    × 0.05  [3rd harmonic]
    + cos(2π × frequency × time × 0.5)  × 0.30  [sub-octave]
    + white noise × noise envelope       × 0.05  [transient click]
```
