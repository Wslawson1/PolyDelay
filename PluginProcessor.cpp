/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyDelayAudioProcessor::PolyDelayAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ) ,
#endif
	tree(*this, nullptr, "PARAMETERS", createParameterLayout())
{
	readHead = 0;
	rightWriteHead = 0;
	leftWriteHead = 0;
	delayBuf.clear();

}

AudioProcessorValueTreeState::ParameterLayout PolyDelayAudioProcessor::createParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	params.push_back(std::make_unique<AudioParameterInt>("leftdelay", "leftDelay", 1, 256, 4));
	params.push_back(std::make_unique<AudioParameterInt>("rightdelay", "rightDelay", 1, 256, 4));
	params.push_back(std::make_unique<AudioParameterInt>("bpm", "bpm", 1, 300, 120));
	params.push_back(std::make_unique<AudioParameterFloat>("drywet", "dryWet", NormalisableRange<float>(0.0f, 1.0f), .50f));
	params.push_back(std::make_unique<AudioParameterFloat>("feedback", "feedback", NormalisableRange<float>(0.0f, .99f), .5f));
	params.push_back(std::make_unique<AudioParameterInt>("countsperrep", "countsPerRep", 1, 10, 4));

	return { params.begin(), params.end() };
}

PolyDelayAudioProcessor::~PolyDelayAudioProcessor()
{
}

//==============================================================================
const String PolyDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PolyDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PolyDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PolyDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PolyDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PolyDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PolyDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PolyDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String PolyDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void PolyDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PolyDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	delayBuf.setSize ( NUM_CHANNELS, sampleRate * MAX_DELAY_TIME );

	//readHead = 0;
	this->sampleRate = sampleRate;
	//getParams();
}

void PolyDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PolyDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void PolyDelayAudioProcessor::setParameters(int bpm, int rightDelay, int leftDelay, float dryWet, float feedback, int countsPerRep)
{
	float samplesPerMinute = sampleRate * 60;
	float bpmInSamples = (samplesPerMinute / bpm) * countsPerRep;
	leftWriteHead = (readHead + ((int)bpmInSamples / leftDelay)) % delayBuf.getNumSamples();
	rightWriteHead = (readHead + ((int)bpmInSamples / rightDelay)) % delayBuf.getNumSamples();

	this->dryWet = dryWet;
	this->feedback = feedback;
}

void PolyDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

	float* rightChannel = buffer.getWritePointer(0);
	float* leftChannel = buffer.getWritePointer(1);

	setParameters((int)*tree.getRawParameterValue("bpm"), (int)*tree.getRawParameterValue("rightdelay"), (int) *tree.getRawParameterValue("leftdelay"),
		(float) *tree.getRawParameterValue("drywet"), (float)*tree.getRawParameterValue("feedback"), (int)*tree.getRawParameterValue("countsperrep"));

	for (int sample = 0; sample < buffer.getNumSamples(); sample++)
	{
		delayBuf.setSample(0, rightWriteHead, rightChannel[sample]);
		delayBuf.setSample(1, leftWriteHead, leftChannel[sample]);
		
		delayBuf.addSample(0, rightWriteHead, delayBuf.getSample(0, readHead) * feedback);
		delayBuf.addSample(1, leftWriteHead, delayBuf.getSample(1, readHead) * feedback);

		buffer.addSample(1, sample, delayBuf.getSample(0, readHead));
		buffer.addSample(0, sample, delayBuf.getSample(1, readHead));

		rightWriteHead = (rightWriteHead + 1) % delayBuf.getNumSamples();
		leftWriteHead = (leftWriteHead + 1) % delayBuf.getNumSamples();

		readHead = (readHead + 1) % delayBuf.getNumSamples();
	}


}

//==============================================================================
bool PolyDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PolyDelayAudioProcessor::createEditor()
{
    return new PolyDelayAudioProcessorEditor (*this);
}

//==============================================================================
void PolyDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	auto treeState = tree.copyState();
	std::unique_ptr<XmlElement> xml(treeState.createXml());
	copyXmlToBinary(*xml, destData);
}

void PolyDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(tree.state.getType()))
		{
			tree.replaceState(ValueTree::fromXml(*xmlState));
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PolyDelayAudioProcessor();
}
