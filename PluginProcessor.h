/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define NUM_CHANNELS 2
#define MAX_DELAY_TIME 10.0

//==============================================================================
/**
*/
class PolyDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PolyDelayAudioProcessor();
    ~PolyDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	void setParameters(int, int, int, float, float, int);

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	AudioProcessorValueTreeState tree;
private:
	int readHead;
	int leftWriteHead;
	int rightWriteHead;
	float sampleRate;
	int countsPerRep;

	AudioBuffer<float> delayBuf;
	//int bpm;

	float feedback;
	float dryWet;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyDelayAudioProcessor)
};
