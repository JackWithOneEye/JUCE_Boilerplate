#include "PluginProcessor.h"
#include "PluginEditor.h"

JUCE_BoilerplateAudioProcessorEditor::JUCE_BoilerplateAudioProcessorEditor (JUCE_BoilerplateAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAllAndMakeVisible(*this, controls);
    setSize (400, 300);
}

JUCE_BoilerplateAudioProcessorEditor::~JUCE_BoilerplateAudioProcessorEditor()
{
}

//==============================================================================
void JUCE_BoilerplateAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void JUCE_BoilerplateAudioProcessorEditor::resized()
{
    const int rowHeight = 150;
    const auto rowWidth = getWidth() * 0.9f;
    const auto rowMarginLeft = getWidth() * 0.05f;
    const int rowGap = 5;
    
    int y = 0;
    
    controls.setBounds(rowMarginLeft, y, rowWidth, rowHeight);
    
    y = rowHeight + rowGap;
    
    // ...
    
    y += rowHeight + rowGap;
}
