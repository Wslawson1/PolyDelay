/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PolyDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    PolyDelayAudioProcessorEditor (PolyDelayAudioProcessor&);
    ~PolyDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	Slider bpm;
	Slider rightDelay;
	Slider leftDelay;
	Slider dryWet;
	Slider feedback;
	
	Slider countsPerRep;

	Label bpmLabel;
	Label rightDelayLabel;
	Label leftDelayLabel;
	Label dryWetLabel;
	Label feedbackLabel;

	Label countsPerRepLabel;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> bpmPtr;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rightDelayPtr;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> leftDelayPtr;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> dryWetPtr;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackPtr;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> countsPerRepPtr;

    PolyDelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyDelayAudioProcessorEditor)
};
