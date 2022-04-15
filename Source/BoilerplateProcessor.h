#pragma once

#include <JuceHeader.h>
#include "ProcessorUtils.h"

class BoilerplateProcessor
{
public:
    
    BoilerplateProcessor() {}
    ~BoilerplateProcessor() {}
    
    enum BoilerplateChoice
    {
        OPT_1,
        OPT_2
    };
    
    void processBlock(float* buffer, const int numSamples);
    
    float processSample(float x);
    
    void reset(float sampleRate);
    
    void setParameters(float _sliderValue, BoilerplateChoice _choiceValue);
    
private:
    float fs = 44100.0f;
    
    SmoothedValL sliderValue = 0.0f;
    BoilerplateChoice choiceValue = BoilerplateChoice::OPT_1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoilerplateProcessor)
};
