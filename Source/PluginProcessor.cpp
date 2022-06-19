/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PingPongPass1AudioProcessor::PingPongPass1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

PingPongPass1AudioProcessor::~PingPongPass1AudioProcessor()
{
}

//==============================================================================
const juce::String PingPongPass1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PingPongPass1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PingPongPass1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PingPongPass1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PingPongPass1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PingPongPass1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PingPongPass1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void PingPongPass1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PingPongPass1AudioProcessor::getProgramName (int index)
{
    return {};
}

void PingPongPass1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PingPongPass1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float maxDelayTime = 3.0;
    delayBufferSamples = (int)(maxDelayTime * (float)sampleRate) + 1;
    if (delayBufferSamples < 1)
        delayBufferSamples = 1;
    
    delayBufferChannels = getTotalNumInputChannels();
    delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
    delayBuffer.clear();
    delayWritePosition = 0;
}

void PingPongPass1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PingPongPass1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PingPongPass1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    applyPingPong(buffer, numSamples);

    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);
}

void PingPongPass1AudioProcessor::applyPingPong(juce::AudioBuffer<float>& buffer, const int numSamples) {

    auto balanceValue = apvts.getRawParameterValue("BALANCE");
    float balance = balanceValue->load();
    auto delayValue = apvts.getRawParameterValue("DELAY");
    float delay = balanceValue->load();
    auto feedbackValue = apvts.getRawParameterValue("FEEDBACK");
    float feedback = balanceValue->load();
    auto mixValue = apvts.getRawParameterValue("MIX");
    float mix = balanceValue->load();

    int localWritePosition = delayWritePosition;

    float* dataL = buffer.getWritePointer(0);
    float* dataR = buffer.getWritePointer(1);
    float* delayDataL = delayBuffer.getWritePointer(0);
    float* delayDataR = delayBuffer.getWritePointer(1);


    for (int sample = 0; sample < numSamples; ++sample) {

        const float inL = (1.0f - balance) * dataL[sample];
        const float inR = balance * dataR[sample];
        float outL = 0.0f;
        float outR = 0.0f;

        float readPosition =
            fmodf((float)localWritePosition - delay + (float)delayBufferSamples, delayBufferSamples);
        int localReadPosition = floorf(readPosition);

        if (localReadPosition != localWritePosition) {
            movePosition(localReadPosition, localWritePosition, readPosition, delayDataL, delayDataR, inL, inR, outL, outR, dataL, dataR, feedback, mix, sample);
        }

        if (++localWritePosition >= delayBufferSamples)
            localWritePosition -= delayBufferSamples;
    }

    delayWritePosition = localWritePosition;
}

void PingPongPass1AudioProcessor::movePosition(int localReadPosition, int localWritePosition, float readPosition, float* delayDataL, float* delayDataR, const float inL, const float inR, float outL, float outR, float* dataL, float* dataR, float feedback, float mix, int sample) {
    float fraction = readPosition - (float)localReadPosition;
    float delayed1L = delayDataL[(localReadPosition + 0)];
    float delayed1R = delayDataR[(localReadPosition + 0)];
    float delayed2L = delayDataL[(localReadPosition + 1) % delayBufferSamples];
    float delayed2R = delayDataR[(localReadPosition + 1) % delayBufferSamples];
    outL = delayed1L + fraction * (delayed2L - delayed1L);
    outR = delayed1R + fraction * (delayed2R - delayed1R);

    dataL[sample] = inL + mix * (outL - inL);
    dataR[sample] = inR + mix * (outR - inR);
    delayDataL[localWritePosition] = inL + outR * feedback;
    delayDataR[localWritePosition] = inR + outL * feedback;
}

//==============================================================================
bool PingPongPass1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PingPongPass1AudioProcessor::createEditor()
{
    return new PingPongPass1AudioProcessorEditor (*this);
}

//==============================================================================
void PingPongPass1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PingPongPass1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PingPongPass1AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PingPongPass1AudioProcessor::createParameters() {
    std::vector < std::unique_ptr < juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("BALANCE", "Balance", 0.0f, 0.9f, 0.25f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAY", "Delay", 0.0f, 3.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 0.9f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 1.0f, 1.0f));

    return { params.begin(), params.end() };

}