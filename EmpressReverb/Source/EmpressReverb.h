/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             EmpressReverb
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Handles incoming and outcoming midi messages.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make, androidstudio, xcode_iphone

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        EmpressReverb

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include "defines.h"
#include "Knobs.h"
#include "FootSwitch.h"
#include "MidiDeviceList.h"

#include "EmpressReverbStates.h"

class State;

//==============================================================================
class EmpressReverb  : public Component,
                       private Timer,
                       private AsyncUpdater
{
    friend class State;
public:
    //==============================================================================
    EmpressReverb();
    ~EmpressReverb() override;

    //==============================================================================
    void timerCallback() override;

    void paint(Graphics& g) override;

    void resized() override;

    void openDevice(int index);

    void closeDevice(int index);

    int getNumMidiOutputs() const noexcept;

    ReferenceCountedObjectPtr<MidiDeviceListEntry> getMidiDevice(int index) const noexcept;

    void sendToOutputs(ControlChange CC, int value);
    bool isPedalOn();
    Point<int> getBackgroundReferencePosition();
    Preset getCurrentPreset();
    Image const& getPresetLight(size_t index);
    void setNewPreset(Preset newPreset);

private:
    //==============================================================================
    struct MidiDeviceListBox : public ListBox,
                               private ListBoxModel
    {
        MidiDeviceListBox (const String& name,
                EmpressReverb& contentComponent)
            : ListBox (name, this),
              parent (contentComponent)
        {
            setOutlineThickness (1);
            setMultipleSelectionEnabled (true);
            setClickingTogglesRowSelection (true);
        }

        //==============================================================================
        int getNumRows() override
        {
            return parent.getNumMidiOutputs();
        }

        void paintListBoxItem (int rowNumber, Graphics& g,
                               int width, int height, bool rowIsSelected) override
        {
            auto textColour = getLookAndFeel().findColour (ListBox::textColourId);

            if (rowIsSelected)
                g.fillAll (textColour.interpolatedWith (getLookAndFeel().findColour (ListBox::backgroundColourId), 0.5));


            g.setColour (textColour);
            g.setFont (height * 0.7f);


            if (rowNumber < parent.getNumMidiOutputs())
                g.drawText (parent.getMidiDevice (rowNumber)->deviceInfo.name,
                            5, 0, width, height,
                            Justification::centredLeft, true);

        }

        //==============================================================================
        void selectedRowsChanged (int) override
        {
            auto newSelectedItems = getSelectedRows();
            if (newSelectedItems != lastSelectedItems)
            {
                for (auto i = 0; i < lastSelectedItems.size(); ++i)
                {
                    if (! newSelectedItems.contains (lastSelectedItems[i]))
                        parent.closeDevice (lastSelectedItems[i]);
                }

                for (auto i = 0; i < newSelectedItems.size(); ++i)
                {
                    if (! lastSelectedItems.contains (newSelectedItems[i]))
                        parent.openDevice (newSelectedItems[i]);
                }

                lastSelectedItems = newSelectedItems;
            }
        }

        //==============================================================================
        void syncSelectedItemsWithDeviceList (const ReferenceCountedArray<MidiDeviceListEntry>& midiDevices)
        {
            SparseSet<int> selectedRows;
            for (auto i = 0; i < midiDevices.size(); ++i)
                if (midiDevices[i]->outDevice.get() != nullptr)
                    selectedRows.addRange (Range<int> (i, i + 1));

            lastSelectedItems = selectedRows;
            updateContent();
            setSelectedRows (selectedRows, dontSendNotification);
        }

    private:
        //==============================================================================
        EmpressReverb& parent;
        SparseSet<int> lastSelectedItems;
    };
    //==============================================================================
    void midiChannelChanged();
    //==============================================================================
    //Callbacks
    void onButtonPressed(String name, ControlChange CC);
    void onButtonReleased(String name, ControlChange CC);
    //Knob Callback
    void onKnobChanged(String name, ControlChange CC, int value);
    //==============================================================================
    void createAndSetLabels();
    void createAndSetButtons();
    void createAndSetKnobs();
    void createAndSetMidi();
    void createAndSetImages();
    //==============================================================================

    void handleAsyncUpdate() override;

    //==============================================================================
    bool hasDeviceListChanged(const Array<MidiDeviceInfo>& availableDevices);

    ReferenceCountedObjectPtr<MidiDeviceListEntry> findDevice(MidiDeviceInfo device) const;

    void closeUnpluggedDevices(const Array<MidiDeviceInfo>& currentlyPluggedInDevices);

    void updateDeviceList();

    //==============================================================================
    void addLabelAndSetStyle(Label& label, Colour, float fontSize);
    //==============================================================================
    Label midiOutputLabel   { "Midi Output Label", "MIDI Output:" };
    Label midiOutputChannel { "Midi Output Channel", "MIDI Output Channel:" };
    ComboBox midiChannel;

    std::unique_ptr<MidiDeviceListBox> midiOutputSelector;
    ReferenceCountedArray<MidiDeviceListEntry> midiOutputs;

    std::shared_ptr<Label> labelMode = nullptr, labelSubmode = nullptr, labelDecay = nullptr, labelMix = nullptr, labelOutput = nullptr, labelLow = nullptr, labelHigh = nullptr, labelThing1 = nullptr, labelThing2 = nullptr;

    int channel = 1;

    std::shared_ptr<State> state = nullptr;

    Preset preset{ -1, DEFAULT_KNOB_DECAY, DEFAULT_KNOB_MIX, DEFAULT_KNOB_OUTPUT, DEFAULT_KNOB_LOW, DEFAULT_KNOB_HI, DEFAULT_KNOB_THING1, DEFAULT_KNOB_THING2, 0 };

    ModeKnob knobMode{ std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), MODE };

    Knob knobDecay  { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), DECAY, DEFAULT_KNOB_DECAY, juce::String("decay")},
         knobMix    { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), MIX, DEFAULT_KNOB_MIX, juce::String("mix")},
         knobOutput { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), OUTPUT, DEFAULT_KNOB_OUTPUT, juce::String("output")},
         knobLow    { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), LOW, DEFAULT_KNOB_LOW, juce::String("low")},
         knobHigh   { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), HI, DEFAULT_KNOB_HI, juce::String("hi")},
         knobTh1    { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), THING1, DEFAULT_KNOB_THING1, juce::String("thing 1")},
         knobTh2    { std::bind(&EmpressReverb::onKnobChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), THING2, DEFAULT_KNOB_THING2, juce::String("thing 2")};

    CriticalSection midiMonitorLock;
    Array<MidiMessage> incomingMessages;

    const std::vector<Image> presetLights = std::vector<Image>{
        ImageFileFormat::loadFrom(File(IMAGE_PRESET1)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET2)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET3)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET4)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET5)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET6)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET7))
    };

    bool PedalOn = false;
    Image imagePedal, bypassLight;
    FootSwitch bypassButton{ "bypass", std::bind(&EmpressReverb::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2), std::bind(&EmpressReverb::onButtonReleased, this, std::placeholders::_1, std::placeholders::_2), BYPASS },
               scrollButton{ "scroll", std::bind(&EmpressReverb::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2), std::bind(&EmpressReverb::onButtonReleased, this, std::placeholders::_1, std::placeholders::_2), SCROLL },
               saveButton  { "save",   std::bind(&EmpressReverb::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2), std::bind(&EmpressReverb::onButtonReleased, this, std::placeholders::_1, std::placeholders::_2), SAVE },
               selectButton{ "select", std::bind(&EmpressReverb::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2), std::bind(&EmpressReverb::onButtonReleased, this, std::placeholders::_1, std::placeholders::_2), SELECT };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EmpressReverb)
};
