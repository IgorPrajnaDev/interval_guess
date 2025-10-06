/*
  ==============================================================================

    SimpleSineVoice.h
    Created for MeloQuiz
    Piano-like synthesizer voice with harmonics and ADSR envelope

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
// Som simples que aceita qualquer nota MIDI
class SimpleSineSound : public juce::SynthesiserSound
{
public:
    SimpleSineSound() {}
    
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

//==============================================================================
// Voz de sintetizador que gera um som de piano com harmônicos
class SimpleSineVoice : public juce::SynthesiserVoice
{
public:
    SimpleSineVoice()
    {
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SimpleSineSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.35; // Volume ajustado
        tailOff = 0.0;
        currentNoteNumber = midiNoteNumber;

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
        
        // Envelope ADSR ajustado para som de piano mais realista
        adsr.setSampleRate(getSampleRate());
        
        // Notas graves têm ataque mais lento, agudas mais rápido
        float attackTime = midiNoteNumber < 60 ? 0.003f : 0.001f;
        float releaseTime = midiNoteNumber < 60 ? 1.0f : 0.5f;
        
        adsr.setParameters({
            attackTime,  // Attack: mais rápido para notas agudas
            0.15f,       // Decay: 150ms 
            0.6f,        // Sustain: 60% do volume
            releaseTime  // Release: mais longo para graves
        });
        
        adsr.noteOn();
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            adsr.noteOff();
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
            adsr.reset();
        }
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            while (--numSamples >= 0)
            {
                // Gero som de piano usando harmônicos
                // A intensidade dos harmônicos varia com a frequência
                auto fundamental = std::sin(currentAngle);
                auto harmonic2 = 0.6 * std::sin(2.0 * currentAngle);   // Segunda harmônica (oitava)
                auto harmonic3 = 0.3 * std::sin(3.0 * currentAngle);   // Terceira harmônica
                auto harmonic4 = 0.15 * std::sin(4.0 * currentAngle);  // Quarta harmônica
                auto harmonic5 = 0.08 * std::sin(5.0 * currentAngle);  // Quinta harmônica
                auto harmonic6 = 0.04 * std::sin(6.0 * currentAngle);  // Sexta harmônica
                
                // Harmônicos ímpares mais fortes (característico do piano)
                auto harmonic7 = 0.03 * std::sin(7.0 * currentAngle);
                
                // Combino harmônicos para criar timbre de piano
                auto pianoWave = fundamental + harmonic2 + harmonic3 + harmonic4 + 
                                harmonic5 + harmonic6 + harmonic7;
                
                // Normalizo (evito clipping) - ajustado para os novos harmônicos
                pianoWave *= 0.4;
                
                // Aplico envelope ADSR
                auto envelopeValue = adsr.getNextSample();
                auto currentSample = (float)(pianoWave * level * envelopeValue);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;
                
                // Se o envelope terminou, paro a nota
                if (!adsr.isActive())
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }
        }
    }

private:
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
    int currentNoteNumber = 0;
    juce::ADSR adsr;
};
