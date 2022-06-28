/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PingPongPass1AudioProcessorEditor::PingPongPass1AudioProcessorEditor (PingPongPass1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    balanceSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    balanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(balanceSlider);

    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    delaySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(delaySlider);

    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    feedbackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(feedbackSlider);

    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(mixSlider);
    
    balanceSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BALANCE", balanceSlider);
    delaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY", delaySlider);
    feedbackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackSlider);
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);
    setSize (600, 400);
}

PingPongPass1AudioProcessorEditor::~PingPongPass1AudioProcessorEditor()
{
}

//==============================================================================
void PingPongPass1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::TestBack_png, BinaryData::TestBack_pngSize);
    g.drawImageAt(background, 0, 0);
}

void PingPongPass1AudioProcessorEditor::resized()
{
    balanceSlider.setBounds(43, 280, 200, 100);
    delaySlider.setBounds(200, 170, 200, 100);
    feedbackSlider.setBounds(197 , 35, 200, 100);
    mixSlider.setBounds(355, 280 , 200, 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
