/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomeostasisAudioProcessor::HomeostasisAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        processorChoises({"Empty","Filter","Phaser","Distortion"})
        , mainTree(*this, nullptr, "SlotsParameters", MainTreeLayout())
#endif
{
    mainTree.state.addListener(this);
    mainTree.addParameterListener("slot1", this);
    mainTree.addParameterListener("slot2", this);
    mainTree.addParameterListener("slot3", this);
    mainTree.addParameterListener("slot4", this);
}

HomeostasisAudioProcessor::~HomeostasisAudioProcessor()
{
}

//==============================================================================
const String HomeostasisAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HomeostasisAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HomeostasisAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HomeostasisAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HomeostasisAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HomeostasisAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HomeostasisAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HomeostasisAudioProcessor::setCurrentProgram (int index)
{
}

const String HomeostasisAudioProcessor::getProgramName (int index)
{
    return {};
}

void HomeostasisAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HomeostasisAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = uint32 (getTotalNumOutputChannels());
    chain.prepare(spec);
}

void HomeostasisAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    chain.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HomeostasisAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HomeostasisAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    chain.process(context);
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
   
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
 

}

//==============================================================================
bool HomeostasisAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HomeostasisAudioProcessor::createEditor()
{
    return new HomeostasisAudioProcessorEditor (*this);
}

//==============================================================================
void HomeostasisAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HomeostasisAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HomeostasisAudioProcessor();
}





//Layout for defining FX Slot Parameters

AudioProcessorValueTreeState::ParameterLayout HomeostasisAudioProcessor::MainTreeLayout ()
{
    std::vector<std::unique_ptr<AudioProcessorParameterGroup>> parameterGroups;
    parameterGroups.push_back(std::make_unique<AudioProcessorParameterGroup>("slotGroup",
                                                   "Slot Group",
                                                   "seperator",
                                                   std::make_unique<AudioParameterChoice>("slot1","Slot 1", processorChoises, 0),
                                                   std::make_unique<AudioParameterChoice>("slot2","Slot 2", processorChoises, 0),
                                                   std::make_unique<AudioParameterChoice>("slot3","Slot 3", processorChoises, 0),
                                                   std::make_unique<AudioParameterChoice>("slot4","Slot 4", processorChoises, 0),
                                                   
                                                   std::make_unique<AudioParameterBool> ("mute","Mute Input", true),
                                                   
                                                   std::make_unique<AudioParameterBool> ("bypass1", "Bypass 1", false),
                                                   std::make_unique<AudioParameterBool> ("bypass2", "Bypass 2", false),
                                                   std::make_unique<AudioParameterBool> ("bypass3", "Bypass 3", false),
                                                   std::make_unique<AudioParameterBool> ("bypass4", "Bypass 4", false)));
    
       
    parameterGroups.push_back(SVFProcessor::makeParamGroup("1"));
    parameterGroups.push_back(SVFProcessor::makeParamGroup("2"));
    parameterGroups.push_back(SVFProcessor::makeParamGroup("3"));
    parameterGroups.push_back(SVFProcessor::makeParamGroup("4"));

    parameterGroups.push_back(DistortionProcessor::makeParamGroup("1"));
    parameterGroups.push_back(DistortionProcessor::makeParamGroup("2"));
    parameterGroups.push_back(DistortionProcessor::makeParamGroup("3"));
    parameterGroups.push_back(DistortionProcessor::makeParamGroup("4"));
    
    parameterGroups.push_back(PhaserProcessor::makeParamGroup("1"));
    parameterGroups.push_back(PhaserProcessor::makeParamGroup("2"));
    parameterGroups.push_back(PhaserProcessor::makeParamGroup("3"));
    parameterGroups.push_back(PhaserProcessor::makeParamGroup("4"));
    
    return {parameterGroups.begin(), parameterGroups.end()};
}

//==============================================================================


void HomeostasisAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    
    

//========================== Check which processor chosen and instantiate it.
    String choice = processorChoises.getReference(newValue);
    int paramIndex = mainTree.getParameter(parameterID)->getParameterIndex();
    
    auto& feedbackGraph = chain.get<feedbackGraphIndex>();
    feedbackGraph.processorChanged(choice, paramIndex, mainTree);


}
