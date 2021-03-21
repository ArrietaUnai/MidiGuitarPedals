#pragma once

#include <JuceHeader.h>

#define KNOB_SIZE 100
#define BUTTON_SIZE 140
#define MODE_KNOB_SIZE 280
#define SAVE_KNOB_SIZE 50
#define PI 3.14159

#define DEFAULT_KNOB_DECAY 20
#define DEFAULT_KNOB_MIX 20
#define DEFAULT_KNOB_OUTPUT 64
#define DEFAULT_KNOB_LOW 64
#define DEFAULT_KNOB_HI 64
#define DEFAULT_KNOB_THING1 20
#define DEFAULT_KNOB_THING2 20

enum ControlChange
{
    DEFAULT = 0,
    MODE = 20,
    DECAY = 21,
    MIX = 22,
    OUTPUT = 23,
    LOW = 24,
    HI = 25,
    THING1 = 26,
    THING2 = 27,
    BYPASS = 60,
    SCROLL = 36,
    SAVE = 38,
    SELECT = 35,
};


struct Mode
{
    Mode() = default;
    Mode(const char * _mode, const char * _submode, const char* _thing1, const char* _thing2, juce::Colour _color, char _CC, int posX, int posY) 
        : mode(_mode), submode(_submode), thing1(_thing1), thing2(_thing2), color(_color), CC(_CC), modeLightPos(juce::Point<int>(posX, posY)) {}
    Mode(const Mode& _mode) 
        : mode(_mode.mode), submode(_mode.submode), thing1(_mode.thing1), thing2(_mode.thing2), color(_mode.color), CC(_mode.CC), modeLightPos(_mode.modeLightPos) {}
    juce::String mode, submode, thing1, thing2;
    juce::Colour color;
    char CC;
    juce::Point<int> modeLightPos;
};

struct Preset
{
    Preset() = default;
    Preset(int _presetNumber, int _knobDecay, int _knobMix, int _knobOutput, int _knobLow, int _knobHigh, int _knobTh1, int _knobTh2, char _modeNumber)
        : presetNumber(_presetNumber), knobDecay(_knobDecay), knobMix(_knobMix), knobOutput(_knobOutput), knobLow(_knobLow), knobHigh(_knobHigh), knobTh1(_knobTh1), knobTh2(_knobTh2), modeNumber(_modeNumber) {}
    int knobDecay = 0, //Will be stored if in the future we want to store presets in host pc
        knobMix = 0,
        knobOutput = 0,
        knobLow = 0,
        knobHigh = 0,
        knobTh1 = 0,
        knobTh2 = 0;
    char modeNumber = 0;
    int presetNumber = 0;
    bool edited = false;
};

const std::vector<Point<int>> PresetLightPositions{
    Point<int>{109, 330},
    Point<int>{295, 330},
    Point<int>{481, 330},
    Point<int>{667, 330},
    Point<int>{854, 330}
};

#define EmpressBlue Colour(85, 88, 227)
#define EmpressGreen Colour(138, 201, 99)
#define EmpressRed Colour(232, 79, 70)
#define EmpressYellow Colour(215, 215, 45)
#define EmpressAqua Colour(58, 83, 87)
#define EmpressPurple Colour(75, 40, 64)

const std::vector<Mode> Modes{
    Mode{"Hall", "Concert Hall", "Modulation", "Early Reflection Level", EmpressBlue, 0, 58, 168},
    Mode{"Hall", "Modern Hall", "Pre-Delay Time", "Early Refrection Level", EmpressGreen, 1, 58, 168},
    Mode{"Plate", "Classic Plate", "Pre-Delay Time", "Bright Early Delay", EmpressBlue, 8, 58, 199},
    Mode{"Plate", "Studio Plate", "Pre-Delay Time", "Early Refrection Level", EmpressGreen, 9, 58, 199},
    Mode{"Spring", "Bright Spring", "Rattle Decay", "Rattle Level", EmpressBlue, 16, 58, 229},
    Mode{"Spring", "Dark Spring", "Rattle Decay", "Rattle Level", EmpressGreen, 17, 58, 229},
    Mode{"Spring", "Overdriven Spring", "Rattle Decay and Level", "Break-Up ammount", EmpressRed, 18, 58, 229},
    Mode{"Room", "L.A. Studio", "Pre-Delay Time", "Early Reflection Level", EmpressBlue, 24, 58, 259},
    Mode{"Sparkle", "Sparkle", "Sparkle Level", "Sparkle Length", EmpressBlue, 32, 192, 47},
    Mode{"Sparkle", "Glummer", "Octave down blend", "Octave up ammount", EmpressGreen, 33, 192, 47},
    Mode{"Modulation", "Modulated", "Rate", "Depth", EmpressBlue, 40, 192, 77},
    Mode{"Modulation", "Chorused Reverb", "Rate", "Depth", EmpressGreen, 41, 192, 77},
    Mode{"Modulation", "Flanged Reverb", "Rate", "Depth", EmpressRed, 42, 192, 77},
    Mode{"Modulation", "Tremolo Reverb", "Rate", "Depth", EmpressYellow, 43, 192, 77},
    Mode{"Ambient Swell", "Triggered Swell", "Swell Time", "Modulation", EmpressBlue, 48, 192, 108},
    Mode{"Ambient Swell", "Gate Swell with Octave", "Gate Attack", "Octave Up Volume", EmpressGreen, 49, 192, 108},
    Mode{"Ambient Swell", "Freezification", "Transition Speed", "Frozen Sound Level", EmpressRed, 50, 192, 108},
    Mode{"Ambient Swell", "Seed", "Diffusion", "Size", EmpressYellow, 51, 192, 108},
    Mode{"Delay+Reverb", "Single Tap Delay", "Delay Time", "Delay Feedback", EmpressBlue, 56, 192, 138},
    Mode{"Delay+Reverb", "Blend-able Delay", "Blend of delay vs reverb", "Delay Feedback", EmpressGreen, 57, 192, 138},
    Mode{"Delay+Reverb", "Filtered Feedback Delay", "Delay Time", "Delay Feedback", EmpressRed, 58, 192, 138},
    Mode{"Delay+Reverb", "Skitter", "Diffusion", "Size", EmpressYellow, 59, 192, 138},
    Mode{"Reverse", "Reverse with Decay", "Swell Time", "Reverse Modulation", EmpressBlue, 64, 192, 168},
    Mode{"Reverse", "Reverse and Stop", "Pre-Delay", "Diffusion", EmpressGreen, 65, 192, 168},
    Mode{"Reverse", "Reversed Delay", "Reverse Delay Length", "Ammunt of Reverb added to the reverse delay", EmpressRed, 66, 192, 168},
    Mode{"Ghost", "Resonant", "Mod Rate", "Resonance", EmpressBlue, 72, 192, 199},
    Mode{"Ghost", "Casper", "Modulation", "Diffusion", EmpressGreen, 73, 192, 199},
    Mode{"Lo-Fi", "50's Radio", "Delay Level", "Dirtiness", EmpressBlue, 80, 192, 229},
    Mode{"Lo-Fi", "Warble", "Warble", "Dirtiness", EmpressGreen, 81, 192, 229},
    Mode{"Beer", "Glintch", "Speed", "Tone", EmpressBlue, 88, 192, 259},
    Mode{"Beer", "Gated Reverb", "Threshold", "Release (10ms - 1s)", EmpressGreen, 89, 192, 259},
    Mode{"Beer", "Destroyer Pad", "Robot Screams", "Pitch Shift", EmpressRed, 90, 192, 259}
};

