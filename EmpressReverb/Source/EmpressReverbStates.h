#pragma once

#include "EmpressReverb.h"
#include "LightsTimer.h"

class EmpressReverb;

class State
{
public:
    virtual ~State() {}
    virtual void onBypassPressed() = 0;
    virtual void onBypassReleased() = 0;
    virtual void onScrollPressed() = 0;
    virtual void onScrollReleased() = 0;
    virtual void onSelectPressed() = 0;
    virtual void onSelectReleased() = 0;
    virtual void onSavePressed() = 0;
    virtual void onSaveReleased() = 0;

    virtual void onKnobsEdited() = 0;

    virtual void paint(Graphics& g) = 0;
protected:
    void setState(EmpressReverb& machine, std::shared_ptr<State> newState);
};

class NormalState : public State
{
public:
    NormalState(EmpressReverb& _e);
    ~NormalState() {
    }
    void onBypassPressed();
    void onBypassReleased();
    void onScrollPressed();
    void onScrollReleased();
    void onSavePressed();
    void onSaveReleased();
    void onSelectPressed();
    void onSelectReleased();
    void onKnobsEdited();
    void paint(Graphics& g);
private:
    EmpressReverb& e;
    Image bypassLight;
};

class PresetBrowseState : public State
{
public:
    PresetBrowseState(EmpressReverb& _e);
    ~PresetBrowseState();
    void onBypassPressed();
    void onBypassReleased();
    void onScrollPressed();
    void onScrollReleased();
    void onSavePressed();
    void onSaveReleased();
    void onSelectPressed();
    void onSelectReleased();
    void onKnobsEdited();
    void paint(Graphics& g);

    void presetLightsTimerCallback();
    void presetTimeoutTimerCallback();

private:
    void nextPreset();

    EmpressReverb& e;
    bool presetLightsOn = false;
    Preset browsingPreset;
    std::vector<Image> presetLights;
    LightsTimer presetLightsTimer{ std::bind(&PresetBrowseState::presetLightsTimerCallback, this) },
                presetTimeoutTimer{ std::bind(&PresetBrowseState::presetTimeoutTimerCallback, this) };
};

class PresetEditedState : public State
{
public:
    PresetEditedState(EmpressReverb& _e, std::vector<Image> images);
    ~PresetEditedState() {}
    void onBypassPressed();
    void onBypassReleased();
    void onScrollPressed();
    void onScrollReleased();
    void onSavePressed();
    void onSaveReleased();
    void onSelectPressed();
    void onSelectReleased();
    void onKnobsEdited();
    void paint(Graphics& g);
private:
    EmpressReverb& e;
    std::vector<Image> presetLights;
};

class PresetSelectState : public State
{
public:
    PresetSelectState(EmpressReverb& _e, std::vector<Image> images);
    ~PresetSelectState() {}
    void onBypassPressed();
    void onBypassReleased();
    void onScrollPressed();
    void onScrollReleased();
    void onSavePressed();
    void onSaveReleased();
    void onSelectPressed();
    void onSelectReleased();
    void onKnobsEdited();
    void paint(Graphics& g);
private:
    EmpressReverb& e;
    std::vector<Image> presetLights;
};
