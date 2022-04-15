#include "BoilerplateProcessor.h"

void BoilerplateProcessor::processBlock(float* buffer, const int numSamples)
{
    for (int n = 0; n < numSamples; ++n)
    {
        buffer[n] = processSample(buffer[n]);
    }
}

float BoilerplateProcessor::processSample(float x)
{
    return x;
}

void BoilerplateProcessor::reset(float sampleRate)
{
    fs = sampleRate;
    
    sliderValue.reset(fs, SMOOTHED_VAL_RAMP_LEN_SEC);
}

void BoilerplateProcessor::setParameters(float _sliderValue, BoilerplateChoice _choiceValue)
{
    sliderValue.setTargetValue(_sliderValue);
    choiceValue = _choiceValue;
}
