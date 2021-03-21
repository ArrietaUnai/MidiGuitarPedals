#pragma once

#include <JuceHeader.h>
#include "defines.h"

typedef std::function <void(String name, ControlChange CC, int value)> KnobCallback;

class ModeKnob : public Slider
{
public:
    ModeKnob(KnobCallback func, ControlChange _CC);
    ~ModeKnob() override {}
    void setLabels(std::shared_ptr<Label> _labelMode, std::shared_ptr<Label> _labelSubmode, std::shared_ptr<Label> _labelThing1, std::shared_ptr<Label> _labelThing2);

    void paint(Graphics& g) override;
    void valueChanged() override;

private:
    std::shared_ptr<Label> labelMode = nullptr, labelSubmode = nullptr, labelThing1 = nullptr, labelThing2 = nullptr;
    KnobCallback notifyValueChanged = nullptr;
    ControlChange CC = DEFAULT;
    size_t modeColor = 0;
    std::vector<Image> ModeColorImages;
    std::vector<String> ImagePaths = {
        IMAGE_MODE_BLUE,
        IMAGE_MODE_GREEN,
        IMAGE_MODE_RED,
        IMAGE_MODE_YELLOW,
    };
    std::vector<Colour> coloursIndex{ EmpressBlue, EmpressGreen, EmpressRed, EmpressYellow };

    void setStyle();
    void updateColors();
    void updateLabels();
    void loadImages();
};

class Knob : public Slider
{
public:
    Knob(KnobCallback func, ControlChange _CC, int initValue, juce::String newName);
    ~Knob() override {}

    void valueChanged() override;
    void paint(Graphics& g) override;

private:
    ControlChange CC = DEFAULT;
    juce::String name;
    Image imageKnobMarker;
    KnobCallback notifyValueChanged = nullptr;
};