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
#include "PhaserProcessor.h"
#include "FeedbackProcessor.h"

class feedbackGraph
{
public:
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::AudioProcessorGraph::Node;
    
    AudioProcessorGraph graph;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        maxBlockSize = samplesPerBlock;
        graph.prepareToPlay(sampleRate, samplesPerBlock);
        graph.setPlayConfigDetails(numChannels, numChannels, sampleRate, maxBlockSize);

        initialiseGraph();
    }
    
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
    {
        numChannels = buffer.getNumChannels();
        graph.processBlock (buffer,midiMessages);
        
    }
    void releaseResources()
    {
        graph.releaseResources();
    }
        void initialiseGraph () // helper function which initialises the processing chains
    {
        midiInputNode = graph.addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::midiInputNode));

        inputNode = graph.addNode(std::make_unique<Wavetable>());
        outputNode = graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        
        node1 = graph.addNode(std::make_unique<ProcessorBase>());
       
        node2 = graph.addNode(std::make_unique<ProcessorBase>());
        
        node3 = graph.addNode(std::make_unique<ProcessorBase>());

        node4 = graph.addNode(std::make_unique<ProcessorBase>());
        
//        feedbackNode = graph.addNode(std::make_unique<FeedbackProcessor>());

        makeSlotConnections();
        connectMidiNodes();
        setAllNodesConfig(graph.getNodes());
    }

    void setNodesConfig (Node::Ptr node)
    {
        node->getProcessor()->setPlayConfigDetails(numChannels, numChannels, sampleRate, maxBlockSize);
    }
    
    void setAllNodesConfig (const ReferenceCountedArray<Node>& nodes)
    {
        for (auto node : nodes)
               {
                   setNodesConfig(node);
               }
    }

    void setSlotNode(int index, std::unique_ptr<AudioProcessor> processor)
    {
        if (index == 0)
        {
            node1 = graph.addNode(std::move(processor));
        }
        else if (index == 1)
        {
            node2 = graph.addNode(std::move(processor));
        }
        else if (index == 2)
        {
            node3 = graph.addNode(std::move(processor));
        }
        else if (index == 3)
        {
            node4 = graph.addNode(std::move(processor));
        }

        setAllNodesConfig(graph.getNodes());
    }

    void makeSlotConnections ()
    {
        constexpr auto midiChannelIndex = juce::AudioProcessorGraph::midiChannelIndex;
        constexpr auto left = 0;
        constexpr auto right = 1;
        constexpr auto leftFB = 3;
        constexpr auto rightFB = 4;
        
        graph.addConnection({{midiInputNode->nodeID, midiChannelIndex},{inputNode->nodeID, midiChannelIndex}});

        graph.addConnection({{inputNode->nodeID, left},{node1->nodeID, left}});
        graph.addConnection({{inputNode->nodeID, right},{node1->nodeID, right}});

        graph.addConnection({{node1->nodeID, left},{node2->nodeID, left}});
        graph.addConnection({{node1->nodeID, right},{node2->nodeID, right}});

        graph.addConnection({{node2->nodeID, left},{node3->nodeID, left}});
        graph.addConnection({{node2->nodeID, right},{node3->nodeID, right}});

        graph.addConnection({{node3->nodeID, left},{node4->nodeID, left}});
        graph.addConnection({{node3->nodeID, right},{node4->nodeID, right}});

        graph.addConnection({{node4->nodeID, left},{outputNode->nodeID, left}});
        graph.addConnection({{node4->nodeID, right},{outputNode->nodeID, right}});
        
//        graph.addConnection({{node4->nodeID, left},{feedbackNode->nodeID, left}});
//        graph.addConnection({{node4->nodeID, right},{feedbackNode->nodeID, right}});
//
//        graph.addConnection({{feedbackNode->nodeID, leftFB},{inputNode->nodeID, left}});
//        graph.addConnection({{feedbackNode->nodeID, rightFB},{inputNode->nodeID, right}});

      

        for (auto node : graph.getNodes())
        {
            node->getProcessor()->enableAllBuses();
        }
        

    }
    
    void connectMidiNodes()
    {
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
                setSlotNode(paramIndex, std::make_unique<PhaserProcessor>(tree, paramIndex));
            }
            else if (choice == "Distortion")
            {
                setSlotNode(paramIndex, std::make_unique<DistortionProcessor>(tree,paramIndex));
            }
            
            
            makeSlotConnections();

    }
private:
    Node::Ptr midiInputNode;
    Node::Ptr inputNode;                            // this is the feedback loop input
    Node::Ptr outputNode;
    Node::Ptr node1;
    Node::Ptr node2;
    Node::Ptr node3;
    Node::Ptr node4;
    Node::Ptr feedbackNode;
    
    uint32 numChannels;
    uint32 maxBlockSize;
    int sampleRate;

};
