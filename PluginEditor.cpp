/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyDelayAudioProcessorEditor::PolyDelayAudioProcessorEditor(PolyDelayAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
	addAndMakeVisible(&bpm);
	addAndMakeVisible(&leftDelay);
	addAndMakeVisible(&rightDelay);
	addAndMakeVisible(&dryWet);
	addAndMakeVisible(&feedback);
	addAndMakeVisible(&countsPerRep);

	bpm.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	leftDelay.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	rightDelay.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	dryWet.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	feedback.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	countsPerRep.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

	bpm.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 20);
	rightDelay.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 20);
	leftDelay.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 20);
	dryWet.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
	feedback.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
	countsPerRep.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);


	dryWet.setNumDecimalPlacesToDisplay(2);
	feedback.setNumDecimalPlacesToDisplay(2);


	addAndMakeVisible(&bpmLabel);
	addAndMakeVisible(&leftDelayLabel);
	addAndMakeVisible(&rightDelayLabel);
	addAndMakeVisible(&dryWetLabel);
	addAndMakeVisible(&feedbackLabel);
	addAndMakeVisible(&countsPerRepLabel);

	bpmLabel.setText("bpm", dontSendNotification);
	leftDelayLabel.setText("left delay", dontSendNotification);
	rightDelayLabel.setText("right delay", dontSendNotification);
	dryWetLabel.setText("dry/wet", dontSendNotification);
	feedbackLabel.setText("feedback", dontSendNotification);
	countsPerRepLabel.setText("beats / repetition", dontSendNotification);

	bpmLabel.attachToComponent(&bpm, false);
	leftDelayLabel.attachToComponent(&leftDelay, false);
	rightDelayLabel.attachToComponent(&rightDelay, false);
	dryWetLabel.attachToComponent(&dryWet, false);
	feedbackLabel.attachToComponent(&feedback, false);
	countsPerRepLabel.attachToComponent(&countsPerRep, false);

	bpmPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "bpm", bpm);
	leftDelayPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>( processor.tree, "leftdelay", leftDelay);
	rightDelayPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "rightdelay", rightDelay);
	dryWetPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "drywet", dryWet);
	feedbackPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>( processor.tree, "feedback", feedback);
	countsPerRepPtr = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree, "countsperrep", countsPerRep);
}
PolyDelayAudioProcessorEditor::~PolyDelayAudioProcessorEditor()
{
}

//==============================================================================
void PolyDelayAudioProcessorEditor::paint (Graphics& g)
{
}

void PolyDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	juce::Rectangle<int> areaBottom = getLocalBounds();
	juce::Rectangle<int> areaTop = areaBottom.removeFromTop ( areaBottom.getHeight() / 2 );

	areaBottom = areaBottom.reduced(areaBottom.getWidth() / 16);
	areaTop = areaTop.reduced(areaTop.getWidth() / 16);
	int width = areaTop.getWidth();

	bpm.setBounds ( areaTop.removeFromLeft ( width / 3 ) );
	leftDelay.setBounds ( areaTop.removeFromLeft( width / 3 ) );
	rightDelay.setBounds( areaTop );

	countsPerRep.setBounds(areaBottom.removeFromLeft(width / 3));
	dryWet.setBounds ( areaBottom.removeFromLeft ( width / 3 ) );
	feedback.setBounds ( areaBottom );


}
