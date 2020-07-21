/*
 ==============================================================================
 
 feedbackProcessor.h
 Created: 19 Jul 2020 5:08:29pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "SVFProcessor.h"
#include "DistortionProcessor.h"
#include "Wavetable.h"


class feedbackGraph
{
public:
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::AudioProcessorGraph::Node;
    
    AudioProcessorGraph graph;
    
    void prepare (const dsp::ProcessSpec& spec)
    {
        numChannels = spec.numChannels;
        maxBlockSize = spec.maximumBlockSize;
        sampleRate = spec.sampleRate;
        buffer.setSize(numChannels, maxBlockSize);
        graph.prepareToPlay(sampleRate, maxBlockSize);
        graph.setPlayConfigDetails(numChannels, numChannels, sampleRate, maxBlockSize);
        
        initialiseGraph();
    }
    
    void process (dsp::ProcessContextReplacing<float>& context)
    {
        auto& outputblock = context.getOutputBlock();
        outputblock.copyTo (buffer);
        graph.processBlock (buffer,midiBuffer);
        outputblock.copyFrom (buffer);
    }
    void reset()
    {
        graph.releaseResources();
    }
        void initialiseGraph () // helper function which initialises the processing chains
    {
        InputNode = graph.addNode(std::make_unique<Wavetable>());
        OutpoutNode = graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        Node1 = graph.addNode(std::make_unique<ProcessorBase>());
        setNodesConfig(Node1);
       
        Node2 = graph.addNode(std::make_unique<ProcessorBase>());
        setNodesConfig(Node2);
        
        Node3 = graph.addNode(std::make_unique<ProcessorBase>());
        setNodesConfig(Node3);

        Node4 = graph.addNode(std::make_unique<ProcessorBase>());
        setNodesConfig(Node4);

        makeSlotConnections();
    }

    void setNodesConfig (Node::Ptr node)
    {
        node->getProcessor()->setPlayConfigDetails(numChannels, numChannels, sampleRate, maxBlockSize);
    }

    void setSlotNode(int index, std::unique_ptr<AudioProcessor> processor)
    {
        if (index == 0)
        {
            Node1 = graph.addNode(std::move(processor));
        }
        else if (index == 1)
        {
            Node2 = graph.addNode(std::move(processor));
        }
        else if (index == 2)
        {
            Node3 = graph.addNode(std::move(processor));
        }
        else if (index == 3)
        {
            Node4 = graph.addNode(std::move(processor));
        }

        for (auto node : graph.getNodes())
        {
            setNodesConfig(node);
        }
    }

    void makeSlotConnections ()
    {
        constexpr auto left = 0;
        constexpr auto right = 1;

        graph.addConnection({{InputNode->nodeID,left},{Node1->nodeID,left}});
        graph.addConnection({{InputNode->nodeID,right},{Node1->nodeID,right}});


        graph.addConnection({{Node1->nodeID,left},{Node2->nodeID,left}});
        graph.addConnection({{Node1->nodeID,right},{Node2->nodeID,right}});

        graph.addConnection({{Node2->nodeID,left},{Node3->nodeID,left}});
        graph.addConnection({{Node2->nodeID,right},{Node3->nodeID,right}});


        graph.addConnection({{Node3->nodeID,left},{Node4->nodeID,left}});
        graph.addConnection({{Node3->nodeID,right},{Node4->nodeID,right}});


        graph.addConnection({{Node4->nodeID,left},{OutpoutNode->nodeID,left}});
        graph.addConnection({{Node4->nodeID,right},{OutpoutNode->nodeID,right}});

        for (auto node : graph.getNodes())
        {
            node->getProcessor()->enableAllBuses();
        }
        

    }
    
    void processorChanged (String choice,int paramIndex, AudioProcessorValueTreeState& tree)         //========================== Check which processor chosen and instantiate it.
    {
         for (auto connection : graph.getConnections())
            {
                graph.removeConnection(connection);
            }


           
            if (choice == "Empty")
            {
                setSlotNode(paramIndex, std::make_unique<ProcessorBase>());
            }
            else if (choice == "Filter")
            {
                setSlotNode(paramIndex ,std::make_unique<SVFProcessor>(tree, paramIndex));
            }
            else if (choice == "Phaser")
            {
                setSlotNode(paramIndex, std::make_unique<ProcessorBase>());
            }
            else if (choice == "Distortion")
            {
                setSlotNode(paramIndex, std::make_unique<DistortionProcessor>(tree,paramIndex));
            }
            
            
            makeSlotConnections();

    }
private:
    AudioBuffer<float> buffer;
    MidiBuffer midiBuffer;
    
    Node::Ptr InputNode;                            // this is the feedback loop input
    Node::Ptr OutpoutNode;
    Node::Ptr Node1;
    Node::Ptr Node2;
    Node::Ptr Node3;
    Node::Ptr Node4;
    
    
    uint32 numChannels;
    uint32 maxBlockSize;
    int sampleRate;

};
