#pragma once

#include <JuceHeader.h>
#include "defines.h"

typedef std::function <void(String name, ControlChange CC)> ButtonCallback;

class FootSwitch : public Button
{
public:
    FootSwitch() : Button("Uknown") {}
    FootSwitch(const String& buttonName, ButtonCallback _onPressed, ButtonCallback _onReleased, ControlChange _CC);
    ~FootSwitch() override {}

    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void buttonStateChanged() override;


private:
    ButtonCallback onPressed = nullptr, onReleased = nullptr;

    ControlChange CC = ControlChange::DEFAULT;
    String name;
    ButtonState prevState = ButtonState::buttonNormal;
};
