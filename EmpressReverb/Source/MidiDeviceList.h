#pragma once

#include <JuceHeader.h>

struct MidiDeviceListEntry : ReferenceCountedObject
{
    MidiDeviceListEntry(MidiDeviceInfo info) : deviceInfo(info) {}

    MidiDeviceInfo deviceInfo;
    std::unique_ptr<MidiOutput> outDevice;

    using Ptr = ReferenceCountedObjectPtr<MidiDeviceListEntry>;
};