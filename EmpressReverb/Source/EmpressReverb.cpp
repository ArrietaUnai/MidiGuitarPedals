#include "EmpressReverb.h"

#include "EmpressReverbStates.h"

EmpressReverb::EmpressReverb()
    : midiOutputSelector(new MidiDeviceListBox("Midi Output Selector", *this))
{
    createAndSetLabels();
    createAndSetButtons();
    addAndMakeVisible(midiOutputSelector.get());
    createAndSetKnobs();
    createAndSetMidi();
    createAndSetImages();

    setSize(991, imagePedal.getHeight() + 100 );

    startTimer(500);

    state = std::make_shared<NormalState>(*this);
}

EmpressReverb::~EmpressReverb()
{
    stopTimer();
    midiOutputs.clear();
    midiOutputSelector.reset();
}

void EmpressReverb::timerCallback()
{
    updateDeviceList();
}

void EmpressReverb::paint(Graphics& g) {
    g.drawImageAt(imagePedal, 0, getHeight() - imagePedal.getHeight());
    if (isPedalOn()) {
        g.drawImageAt(bypassLight, 761 - bypassLight.getWidth() / 2, getHeight() - getBackgroundReferencePosition().y + 545 - bypassLight.getHeight() / 2);
    }
    state->paint(g);
}

void EmpressReverb::resized()
{
    auto margin = 10;
    auto pedalImagePosY = getHeight() - imagePedal.getHeight();

    midiOutputLabel.setBounds(margin, margin,
        100, 24);


    midiOutputSelector->setBounds( 100 + margin, margin,
        (getWidth() / 2) - (2 * margin) - 100 - margin,
        pedalImagePosY - 2 * margin);

    midiOutputChannel.setBounds((getWidth() / 2) + margin, margin,
        200, 24);


    midiChannel.setBounds((getWidth() / 2) + 200 + margin, margin,
        (getWidth() / 2) - (2 * margin) - 200 - margin,
        24);



    const int R1yM = pedalImagePosY;
    const int R1y = pedalImagePosY + 59 - 92 / 2;
    const int R2y = pedalImagePosY + 220 - 92 / 2;


    knobMode.setBounds(0, R1yM,
        MODE_KNOB_SIZE,
        MODE_KNOB_SIZE);


    knobDecay.setBounds(458 - 92 / 2, R1y,
        KNOB_SIZE,
        KNOB_SIZE);


    knobMix.setBounds(644 - 92 / 2, R1y,
        KNOB_SIZE,
        KNOB_SIZE);


    knobOutput.setBounds(833 - 92 / 2, R1y,
        KNOB_SIZE,
        KNOB_SIZE);


    knobLow.setBounds(361 - 92 / 2, R2y,
        KNOB_SIZE,
        KNOB_SIZE);


    knobHigh.setBounds(550 - 92 / 2, R2y - 1,
        KNOB_SIZE,
        KNOB_SIZE);


    knobTh1.setBounds(738 - 92 / 2, R2y + 1,
        KNOB_SIZE,
        KNOB_SIZE);

    labelThing1->setBounds(690, pedalImagePosY + 250,
        KNOB_SIZE,
        KNOB_SIZE);
    

    knobTh2.setBounds(928 - 92 / 2, R2y - 1,
        KNOB_SIZE,
        KNOB_SIZE);

    labelThing2->setBounds(870, pedalImagePosY + 252,
        KNOB_SIZE,
        KNOB_SIZE);

    selectButton.setBounds(getWidth() / 8 - BUTTON_SIZE / 2, getHeight() - (BUTTON_SIZE / 4 + BUTTON_SIZE),
        BUTTON_SIZE, BUTTON_SIZE);

    scrollButton.setBounds(getWidth() / 2 - KNOB_SIZE / 2, getHeight() - (KNOB_SIZE / 4 + KNOB_SIZE),
        KNOB_SIZE, KNOB_SIZE);

    saveButton.setBounds(getWidth() / 2 - KNOB_SIZE - SAVE_KNOB_SIZE / 2, getHeight() - (KNOB_SIZE / 4 + SAVE_KNOB_SIZE),
        SAVE_KNOB_SIZE, SAVE_KNOB_SIZE);

    bypassButton.setBounds(getWidth() - getWidth() / 8 - BUTTON_SIZE / 2, getHeight() - (BUTTON_SIZE / 4 + BUTTON_SIZE),
        BUTTON_SIZE, BUTTON_SIZE);

}

void EmpressReverb::openDevice(int index)
{
    jassert(midiOutputs[index]->outDevice.get() == nullptr);
    midiOutputs[index]->outDevice = MidiOutput::openDevice(midiOutputs[index]->deviceInfo.identifier);

    if (midiOutputs[index]->outDevice.get() == nullptr)
    {
        DBG("EmpressReverb::openDevice: open output device for index = " << index << " failed!");
    }

}

void EmpressReverb::closeDevice(int index)
{
    jassert(midiOutputs[index]->outDevice.get() != nullptr);
    midiOutputs[index]->outDevice.reset();
}

int EmpressReverb::getNumMidiOutputs() const noexcept
{
    return midiOutputs.size();
}

ReferenceCountedObjectPtr<MidiDeviceListEntry> EmpressReverb::getMidiDevice(int index) const noexcept
{
    return midiOutputs[index];
}

void EmpressReverb::sendToOutputs(ControlChange CC, int value)
{
    unsigned char data[3] = { 0xB1, (unsigned char)CC, (unsigned char)value };
    MidiMessage msg = MidiMessage(data, 3);

    if (channel == 0)
    {
        for (int i = 1; i < 17; i++)
        {
            msg.setChannel(i);
            for (auto midiOutput : midiOutputs)
                if (midiOutput->outDevice.get() != nullptr)
                    midiOutput->outDevice->sendMessageNow(msg);
        }
    }
    else {
        msg.setChannel(channel);
        for (auto midiOutput : midiOutputs)
            if (midiOutput->outDevice.get() != nullptr)
                midiOutput->outDevice->sendMessageNow(msg);
    }

}

bool EmpressReverb::isPedalOn()
{
    return PedalOn;
}

Point<int> EmpressReverb::getBackgroundReferencePosition()
{
    return Point<int>(imagePedal.getWidth(), imagePedal.getHeight());
}

Preset EmpressReverb::getCurrentPreset()
{
	return preset;
}

Image const& EmpressReverb::getPresetLight(size_t index)
{
    return presetLights.at(index);
}

void EmpressReverb::setNewPreset(Preset newPreset)
{
    preset = newPreset;
}

void EmpressReverb::midiChannelChanged()
{
    channel = midiChannel.getSelectedId();
    if (channel == 17)
    {
        channel = 0;
    }
}

void EmpressReverb::onButtonPressed(String name, ControlChange CC)
{
    switch (CC)
    {
    case BYPASS:
        PedalOn = !PedalOn;
        repaint();
        state->onBypassPressed();
        break;
    case SCROLL:
        state->onScrollPressed();
        break;
    case SAVE:
        state->onSavePressed();
        break;
    case SELECT:
        state->onSelectPressed();
        break;
    default:
        break;
    }
}

void EmpressReverb::onButtonReleased(String name, ControlChange CC)
{
    switch (CC)
    {
    case BYPASS:
        state->onBypassReleased();
        break;
    case SCROLL:
        state->onScrollReleased();
        break;
    case SAVE:
        state->onSaveReleased();
        break;
    case SELECT:
        state->onSelectReleased();
        break;
    default:
        break;
    }
}

void EmpressReverb::onKnobChanged(String name, ControlChange CC, int value)
{
    sendToOutputs(CC, value);
    switch (CC)
    {
    case DEFAULT:
        break;
    case MODE:
        preset.modeNumber = value;
        break;
    case DECAY:
        preset.knobDecay = value;
        break;
    case MIX:
        preset.knobMix = value;
        break;
    case OUTPUT:
        preset.knobOutput = value;
        break;
    case LOW:
        preset.knobLow = value;
        break;
    case HI:
        preset.knobHigh = value;
        break;
    case THING1:
        preset.knobTh1 = value;
        break;
    case THING2:
        preset.knobTh2 = value;
        break;
    default:
        break;
    }
    if (state)
        state->onKnobsEdited();
}

void EmpressReverb::createAndSetLabels()
{
    labelMode = std::make_shared<Label>("Mode Knob Label", "modes");
    labelSubmode = std::make_shared<Label>("Submode Knob Label", "submode");
    labelDecay = std::make_shared<Label>("Decay Knob Label", "decay");
    labelMix = std::make_shared<Label>("Mix Knob Label", "mix");
    labelOutput = std::make_shared<Label>("Output Knob Label", "output");
    labelLow = std::make_shared<Label>("Low Knob Label", "low");
    labelHigh = std::make_shared<Label>("High Knob Label", "hi");
    labelThing1 = std::make_shared<Label>("Thing 1 Knob Label", "thing 1");
    labelThing2 = std::make_shared<Label>("Thing 2 Knob Label", "thing 2");

    knobMode.setLabels(labelMode, labelSubmode, labelThing1, labelThing2);

    //Labels
    addLabelAndSetStyle(midiOutputLabel, Colours::white, 15.00f);
    addLabelAndSetStyle(midiOutputChannel, Colours::white, 15.00f);
    addLabelAndSetStyle(*labelMode, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelSubmode, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelDecay, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelMix, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelOutput, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelLow, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelHigh, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelThing1, Colours::black, 20.00f);
    addLabelAndSetStyle(*labelThing2, Colours::black, 20.00f);
}

void EmpressReverb::createAndSetButtons()
{
    addAndMakeVisible(selectButton);
    addAndMakeVisible(scrollButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(bypassButton);
}

void EmpressReverb::createAndSetKnobs()
{
    addAndMakeVisible(knobMode);
    addAndMakeVisible(knobDecay);
    addAndMakeVisible(knobMix);
    addAndMakeVisible(knobOutput);
    addAndMakeVisible(knobLow);
    addAndMakeVisible(knobHigh);
    addAndMakeVisible(knobTh1);
    addAndMakeVisible(knobTh2);
}

void EmpressReverb::createAndSetMidi()
{
    addAndMakeVisible(midiChannel);
    for (size_t i = 1; i < 17; i++)
    {
        midiChannel.addItem(juce::String(i), i);
    }

    midiChannel.addItem("All", 17);
    midiChannel.onChange = [this] { midiChannelChanged(); };
    midiChannel.setSelectedId(1);
}

void EmpressReverb::createAndSetImages()
{
    imagePedal = ImageFileFormat::loadFrom(File(IMAGE_PEDAL));
    bypassLight = ImageFileFormat::loadFrom(File(IMAGE_BYPASS));
}

void EmpressReverb::handleAsyncUpdate()
{
    // This is called on the message loop
    Array<MidiMessage> messages;

    {
        const ScopedLock sl(midiMonitorLock);
        messages.swapWith(incomingMessages);
    }

    String messageText;

    for (auto& m : messages)
        messageText << m.getDescription() << "\n";

    //midiMonitor.insertTextAtCaret (messageText);
}


//==============================================================================
bool EmpressReverb::hasDeviceListChanged(const Array<MidiDeviceInfo>& availableDevices)
{
    ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

    if (availableDevices.size() != midiDevices.size())
        return true;

    for (auto i = 0; i < availableDevices.size(); ++i)
        if (availableDevices[i] != midiDevices[i]->deviceInfo)
            return true;

    return false;
}

ReferenceCountedObjectPtr<MidiDeviceListEntry> EmpressReverb::findDevice(MidiDeviceInfo device) const
{
    const ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

    for (auto& d : midiDevices)
        if (d->deviceInfo == device)
            return d;

    return nullptr;
}

void EmpressReverb::closeUnpluggedDevices(const Array<MidiDeviceInfo>& currentlyPluggedInDevices)
{
    ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

    for (auto i = midiDevices.size(); --i >= 0;)
    {
        auto& d = *midiDevices[i];

        if (!currentlyPluggedInDevices.contains(d.deviceInfo))
        {
            if (d.outDevice.get() != nullptr)
                closeDevice(i);

            midiDevices.remove(i);
        }
    }
}

void EmpressReverb::updateDeviceList()
{
    auto availableDevices = MidiOutput::getAvailableDevices();

    if (hasDeviceListChanged(availableDevices))
    {

        ReferenceCountedArray<MidiDeviceListEntry>& midiDevices
            = midiOutputs;

        closeUnpluggedDevices(availableDevices);

        ReferenceCountedArray<MidiDeviceListEntry> newDeviceList;

        // add all currently plugged-in devices to the device list
        for (auto& newDevice : availableDevices)
        {
            MidiDeviceListEntry::Ptr entry = findDevice(newDevice);

            if (entry == nullptr)
                entry = new MidiDeviceListEntry(newDevice);

            newDeviceList.add(entry);
        }

        // actually update the device list
        midiDevices = newDeviceList;

        // update the selection status of the combo-box
        if (auto* midiSelector = midiOutputSelector.get())
            midiSelector->syncSelectedItemsWithDeviceList(midiDevices);
    }
}

//==============================================================================
void EmpressReverb::addLabelAndSetStyle(Label& label, Colour color, float fontSize)
{
    label.setFont(Font(fontSize, Font::plain));
    label.setJustificationType(Justification::centred);
    label.setEditable(false, false, false);
    label.setColour(Label::textColourId, color);

    addAndMakeVisible(label);
}

