#include "PluginProcessor.h"
#include "PluginEditor.h"

JUCE_BoilerplateAudioProcessor::JUCE_BoilerplateAudioProcessor(AudioProcessorValueTreeState::ParameterLayout layout)
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters { layout },
    vts(*this, nullptr, Identifier("Parameters"), std::move(layout))
{
    vts.state.addListener(this);
}

JUCE_BoilerplateAudioProcessor::~JUCE_BoilerplateAudioProcessor()
{
}

//==============================================================================
const String JUCE_BoilerplateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JUCE_BoilerplateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JUCE_BoilerplateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JUCE_BoilerplateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
void JUCE_BoilerplateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < 2; i++)
    {
        boilerplateProcessor[i].reset(sampleRate);
    }
}

void JUCE_BoilerplateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JUCE_BoilerplateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void JUCE_BoilerplateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    if (requiresUpdate.load())
    {
        auto sliderValue = parameters.sliderParam.get();
        auto choiceValue = parameters.choiceParam.getIndex();
        
        for (int i = 0; i < 2; i++)
        {
            boilerplateProcessor[i].setParameters(sliderValue, static_cast<BoilerplateProcessor::BoilerplateChoice>(choiceValue));
        }
        
        requiresUpdate.store(false);
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        boilerplateProcessor[channel].processBlock(channelData, buffer.getNumSamples());
    }
}

//==============================================================================
bool JUCE_BoilerplateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JUCE_BoilerplateAudioProcessor::createEditor()
{
    return new JUCE_BoilerplateAudioProcessorEditor (*this);
}

//==============================================================================
void JUCE_BoilerplateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    copyXmlToBinary(*vts.copyState().createXml(), destData);
}

void JUCE_BoilerplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    vts.replaceState(ValueTree::fromXml(*getXmlFromBinary(data, sizeInBytes)));
}

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JUCE_BoilerplateAudioProcessor();
}
