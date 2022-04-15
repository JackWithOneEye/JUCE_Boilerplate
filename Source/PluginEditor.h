#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EditorComponents.h"

class JUCE_BoilerplateAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    JUCE_BoilerplateAudioProcessorEditor (JUCE_BoilerplateAudioProcessor&);
    ~JUCE_BoilerplateAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    struct PluginControls : public Component
    {
        explicit PluginControls(const JUCE_BoilerplateAudioProcessor::ParameterReferences& state)
            : slider(state.sliderParam),
            choice(state.choiceParam)
        {
            addAllAndMakeVisible(*this, slider, choice);
        }

        void resized() override
        {
            performLayout(getLocalBounds(), slider, choice);
        }

        AttachedSlider slider;
        AttachedCombo choice;
    };
    
    JUCE_BoilerplateAudioProcessor& audioProcessor;
    PluginControls controls { audioProcessor.getParameterValues() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JUCE_BoilerplateAudioProcessorEditor)
};
