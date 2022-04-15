#pragma once

#include <JuceHeader.h>
#include "BoilerplateProcessor.h"

namespace paramID
{
#define PARAMETER_ID(str) constexpr const char* str { #str };
    PARAMETER_ID(sliderParam)
    PARAMETER_ID(choiceParam)
#undef PARAMETER_ID
}

template <typename Func, typename... Items>
constexpr void forEach(Func&& func, Items&&... items)
noexcept (noexcept (std::initializer_list<int> { (func(std::forward<Items>(items)), 0)... }))
{
    (void)std::initializer_list<int> { ((void)func(std::forward<Items>(items)), 0)... };
}

class JUCE_BoilerplateAudioProcessor : public AudioProcessor, private ValueTree::Listener
{
public:
    //==============================================================================
    explicit JUCE_BoilerplateAudioProcessor(AudioProcessorValueTreeState::ParameterLayout layout);
    JUCE_BoilerplateAudioProcessor() : JUCE_BoilerplateAudioProcessor(AudioProcessorValueTreeState::ParameterLayout{}) {};
    ~JUCE_BoilerplateAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

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
    double getTailLengthSeconds() const override { return 0.0; };

    //==============================================================================
    int getNumPrograms() override { return 1; };
    int getCurrentProgram() override { return 0; };
    void setCurrentProgram(int index) override {};
    const String getProgramName(int index) override { return {}; };
    void changeProgramName(int index, const String& newName) override {};

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    using Parameter = AudioProcessorValueTreeState::Parameter;
    struct ParameterReferences
    {
        template <typename Param>
        static Param& addToLayout(AudioProcessorValueTreeState::ParameterLayout& layout, std::unique_ptr<Param> param)
        {
            auto& ref = *param;
            layout.add(std::move(param));
            return ref;
        }
        
        static String valueToTextFunction(float x) { return String(x, 2); }
        static float textToValueFunction(const String& str) { return str.getFloatValue(); }
        
        static const StringArray choiceParamOptions() { return StringArray{ "OPT 1", "OPT 2" }; }
        
        explicit ParameterReferences(AudioProcessorValueTreeState::ParameterLayout& layout)
            : sliderParam(addToLayout(layout, std::make_unique<Parameter>(paramID::sliderParam, "Slider Value", "", NormalisableRange<float>(0.0f, 1.0f), 0.0f, valueToTextFunction, textToValueFunction))),
            choiceParam(addToLayout(layout, std::make_unique<AudioParameterChoice>(paramID::choiceParam, "Choice Value", choiceParamOptions(), 0)))
        {}
        
        Parameter& sliderParam;
        AudioParameterChoice& choiceParam;
    };
    
    const ParameterReferences& getParameterValues() const noexcept { return parameters; }
    AudioProcessorValueTreeState& getVts() { return vts; }

private:
    void valueTreePropertyChanged(ValueTree&, const Identifier&) override
    {
        requiresUpdate.store(true);
    }
    
    ParameterReferences parameters;
    AudioProcessorValueTreeState vts;
    std::atomic<bool> requiresUpdate{ true };
    
    BoilerplateProcessor boilerplateProcessor[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JUCE_BoilerplateAudioProcessor)
};
