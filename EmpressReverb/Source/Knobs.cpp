#include "Knobs.h"


template <class T>
int getIndexInVector(std::vector<T> vec, T val)
{
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (val == vec[i])
            return i;
    }
    return -1;
}

ModeKnob::ModeKnob(KnobCallback func, ControlChange _CC)
    : notifyValueChanged(func), CC(_CC)
{
    setStyle();
    setName("mode");
    updateLabels();
    loadImages();
}

void ModeKnob::setLabels(std::shared_ptr<Label> _labelMode, std::shared_ptr<Label> _labelSubmode, std::shared_ptr<Label> _labelThing1, std::shared_ptr<Label> _labelThing2)
{
    labelMode = _labelMode;
    labelSubmode = _labelSubmode;
    labelThing1 = _labelThing1;
    labelThing2 = _labelThing2;
    updateLabels();
}

void ModeKnob::paint(Graphics& g) {
    auto modeNumber = getIndexInVector<Colour>(coloursIndex, Modes[(size_t)getValue()].color);
    if (modeNumber < 0)
        return;
    g.drawImageAt(ModeColorImages[modeNumber], Modes[static_cast<int>(getValue())].modeLightPos.getX() - 8, Modes[static_cast<int>(getValue())].modeLightPos.getY() - 8);
}

void ModeKnob::valueChanged()
{
    updateColors();
    updateLabels();
    if (notifyValueChanged)
        notifyValueChanged("Mode", CC, Modes[static_cast<int>(getValue())].CC);
}

void ModeKnob::setStyle()
{
    setRotaryParameters(0, 2 * PI, false);
    setRange(0, Modes.size() - 1.0, 1.0);
    setValue(0);
    setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}

void ModeKnob::updateColors()
{
    setColour(Slider::ColourIds::textBoxTextColourId, Modes[(size_t)getValue()].color);
}

void ModeKnob::updateLabels()
{
    if (labelMode)
        labelMode->setText(Modes[(size_t)getValue()].mode, NotificationType::dontSendNotification);
    if (labelSubmode)
        labelSubmode->setColour(Label::backgroundColourId, Modes[(size_t)getValue()].color);
    if (labelSubmode)
        labelSubmode->setText(Modes[(size_t)getValue()].submode, NotificationType::dontSendNotification);
    if (labelThing1)
        labelThing1->setText(Modes[(size_t)getValue()].thing1, NotificationType::dontSendNotification);
    if (labelThing2)
        labelThing2->setText(Modes[(size_t)getValue()].thing2, NotificationType::dontSendNotification);
}

void ModeKnob::loadImages()
{
    for (const auto & path : ImagePaths)
    {
        ModeColorImages.push_back(ImageFileFormat::loadFrom(File(path)));
    }
}


Knob::Knob(KnobCallback func, ControlChange _CC, int initValue, juce::String newName)
    : notifyValueChanged(func), CC(_CC), name(newName)
{
    setRotaryParameters(PI + PI / 8.0, 3.0 * PI - PI / 8.0, true);
    setRange(0, 127, 1);
    setValue(initValue);
    setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::white);
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    imageKnobMarker = ImageFileFormat::loadFrom(File(IMAGE_KNOB_MARKER));
    setName(newName);
}

void Knob::valueChanged()
{
    if (notifyValueChanged)
        notifyValueChanged(name, CC, getValue());
}

void Knob::paint(Graphics& g) {
    g.drawImageTransformed(imageKnobMarker, AffineTransform::rotation((-3.0 * PI / 4.0) + (2.0 * PI - PI / 2.0) * getValue() / 128.0 , imageKnobMarker.getWidth() / 2.0, imageKnobMarker.getHeight() / 2.0));
}