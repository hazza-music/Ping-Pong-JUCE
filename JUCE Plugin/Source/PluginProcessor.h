/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PingPongPass1AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PingPongPass1AudioProcessor();
    ~PingPongPass1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
private:

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSamples;
    int delayBufferChannels;
    int delayWritePosition;

    //==========================Delay Functions=====================================
    void PingPongPass1AudioProcessor::applyPingPong(juce::AudioBuffer<float>& buffer, const int numSamples);
    void PingPongPass1AudioProcessor::movePosition(int localReadPosition, int localWritePosition, float readPosition, float* delayDataL, float* delayDataR, const float inL, const float inR, float outL, float outR, float* dataL, float* dataR, float feedback, float mix, int sample);
    
    //=====================Distortion Functions=====================================
    void PingPongPass1AudioProcessor::applyDistortion(juce::AudioBuffer<float>& buffer, const int numSamples, float* channelData);
    float PingPongPass1AudioProcessor::hardClip(const float& sample, float thresh);

    //=========================IIR Filter===========================================
    void PingPongPass1AudioProcessor::updateFilter();
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients <float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients <float>> highPassFilter;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PingPongPass1AudioProcessor)
};
