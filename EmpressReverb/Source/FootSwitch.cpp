#include "FootSwitch.h"

FootSwitch::FootSwitch(const String& buttonName, ButtonCallback _onPressed, ButtonCallback _onReleased, ControlChange _CC)
    : Button(buttonName), name(buttonName), CC(_CC), onPressed(_onPressed), onReleased(_onReleased)
{
    setTriggeredOnMouseDown(true);
}

void FootSwitch::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
}

void FootSwitch::buttonStateChanged()
{
    switch (getState())
    {
    case ButtonState::buttonDown:
        if (onPressed)
            onPressed(name, CC);
        prevState = getState();
        break;
    case ButtonState::buttonNormal:
        if (prevState == buttonDown) {
            if (onReleased)
                onReleased(name, CC);
        }
        prevState = getState();
        break;
    case ButtonState::buttonOver:
        if (prevState == buttonDown) {
            if (onReleased)
                onReleased(name, CC);
        }
        prevState = getState();
        break;
    default:
        break;
    }
}

//void FootSwitch::onButtonPressed()
//{
//    buttonOn = !buttonOn;
//    if (sendMidiMessage)
//    {
//        char data[3] = { 0xB1, CC, 127 };
//        MidiMessage msg = MidiMessage(data, 3);
//        sendMidiMessage(msg, MidiChannel);
//    }
//
//    if (onPressed)
//        onPressed();
//}
//
//void FootSwitch::onButtonReleased()
//{
//    if (sendMidiMessage)
//    {
//        char data[3] = { 0xB1, CC, 0 };
//        MidiMessage msg = MidiMessage(data, 3);
//        sendMidiMessage(msg, MidiChannel);
//    }
//
//    if (onReleased)
//        onReleased();
//}
