#include "EmpressReverbStates.h"

void State::setState(EmpressReverb& machine, std::shared_ptr<State> newState)
{
    machine.state = newState;
}

NormalState::NormalState(EmpressReverb& _e)
    :e(_e)
{
}

void NormalState::onBypassPressed()
{
    if (e.isPedalOn())
        e.sendToOutputs(BYPASS, 127);
    else
        e.sendToOutputs(BYPASS, 0);
}

void NormalState::onBypassReleased()
{
}

void NormalState::onScrollPressed()
{
    e.sendToOutputs(SCROLL, 64);
    setState(e, std::make_shared<PresetBrowseState>(e));
}

void NormalState::onScrollReleased()
{
    e.sendToOutputs(SCROLL, 0);
}

void NormalState::onSavePressed()
{
    e.sendToOutputs(SAVE, 64);
}

void NormalState::onSaveReleased()
{
}

void NormalState::onSelectPressed()
{
    e.sendToOutputs(SELECT, 127);
}

void NormalState::onSelectReleased()
{
    e.sendToOutputs(SELECT, 0);
}

void NormalState::onKnobsEdited()
{
}

void NormalState::paint(Graphics& g)
{

}




PresetBrowseState::PresetBrowseState(EmpressReverb& _e)
    :e(_e)
{
    browsingPreset = e.getCurrentPreset();
    nextPreset();

    presetLights = std::vector<Image>{
        ImageFileFormat::loadFrom(File(IMAGE_PRESET1)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET2)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET3)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET4)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET5)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET6)),
        ImageFileFormat::loadFrom(File(IMAGE_PRESET7)),
    };
    
    presetLightsTimer.starLightstTimer(250);
    presetTimeoutTimer.starLightstTimer(5000);
}

PresetBrowseState::~PresetBrowseState()
{
    presetLightsTimer.stopLightsTimer();
    presetTimeoutTimer.stopLightsTimer();
}

void PresetBrowseState::onBypassPressed()
{
    if (e.isPedalOn())
        e.sendToOutputs(BYPASS, 127);
    else
        e.sendToOutputs(BYPASS, 0);
}

void PresetBrowseState::onBypassReleased()
{
}

void PresetBrowseState::onScrollPressed()
{
    nextPreset();

    e.sendToOutputs(SCROLL, 64);
    
    presetTimeoutTimer.resetTimer();

    presetLightsOn = true;
    e.repaint();
}

void PresetBrowseState::onScrollReleased()
{

}

void PresetBrowseState::onSavePressed()
{
    e.sendToOutputs(SAVE, 64);
}

void PresetBrowseState::onSaveReleased()
{
}

void PresetBrowseState::onSelectPressed()
{
    e.sendToOutputs(SELECT, 64);
    e.setNewPreset(browsingPreset);
    presetLightsOn = true;
    e.repaint();
    setState(e, std::make_shared<PresetSelectState>(e, presetLights));
}

void PresetBrowseState::onSelectReleased()
{
}

void PresetBrowseState::onKnobsEdited()
{
    presetLightsOn = true;
    e.repaint();
    browsingPreset.edited = true;
    e.setNewPreset(browsingPreset);
    setState(e, std::make_shared<PresetEditedState>(e, presetLights));
}

void PresetBrowseState::paint(Graphics& g)
{
    if (presetLightsOn)
    {
        int lightColorIndex = browsingPreset.presetNumber / 5;
        for (size_t i = 0; i <= browsingPreset.presetNumber % 5; i++)
        {
            g.drawImageAt(presetLights[lightColorIndex], PresetLightPositions[i].x, e.getHeight() - e.getBackgroundReferencePosition().y + PresetLightPositions[i].y);
        }
    }
}

void PresetBrowseState::presetLightsTimerCallback()
{
    presetLightsOn = !presetLightsOn;
    e.repaint();
}

void PresetBrowseState::presetTimeoutTimerCallback()
{
    if (e.getCurrentPreset().presetNumber == -1) {
        presetLightsOn = false;
        e.repaint();
        setState(e, std::make_shared<NormalState>(e));
    }
    else {
        presetLightsOn = true;
        e.repaint();
        if (e.getCurrentPreset().edited)
            setState(e, std::make_shared<PresetEditedState>(e, presetLights));
        else
            setState(e, std::make_shared<PresetSelectState>(e, presetLights));
    }
}

void PresetBrowseState::nextPreset()
{
    if (browsingPreset.presetNumber + 1 >= 35)
        browsingPreset.presetNumber = 0;
    else
        browsingPreset.presetNumber += 1;
}




PresetEditedState::PresetEditedState(EmpressReverb& _e, std::vector<Image> images)
    :e(_e)
{
    for (auto image : images)
    {
        auto imageCopy = image.createCopy();
        imageCopy.multiplyAllAlphas(0.6);
        presetLights.push_back(imageCopy);
    }
}

void PresetEditedState::onBypassPressed()
{
    if (e.isPedalOn())
        e.sendToOutputs(BYPASS, 127);
    else
        e.sendToOutputs(BYPASS, 0);
}

void PresetEditedState::onBypassReleased()
{
}

void PresetEditedState::onScrollPressed()
{
    e.sendToOutputs(SCROLL, 64);
    setState(e, std::make_shared<PresetBrowseState>(e));
}

void PresetEditedState::onScrollReleased()
{
}

void PresetEditedState::onSavePressed()
{
    e.sendToOutputs(SAVE, 64);
}

void PresetEditedState::onSaveReleased()
{
}

void PresetEditedState::onSelectPressed()
{
    e.sendToOutputs(SELECT, 127);
}

void PresetEditedState::onSelectReleased()
{
    e.sendToOutputs(SELECT, 0);
}

void PresetEditedState::onKnobsEdited()
{
}

void PresetEditedState::paint(Graphics& g)
{
    size_t lightColorIndex = e.getCurrentPreset().presetNumber / 5;
    for (size_t i = 0; i <= e.getCurrentPreset().presetNumber % 5; i++)
    {
        g.drawImageAt(presetLights[lightColorIndex], PresetLightPositions[i].x, e.getHeight() - e.getBackgroundReferencePosition().y + PresetLightPositions[i].y);
    }
}




PresetSelectState::PresetSelectState(EmpressReverb& _e, std::vector<Image> images)
    :e(_e), presetLights(images)
{
    auto preset = e.getCurrentPreset();
    preset.edited = false;
    e.setNewPreset(preset);
}

void PresetSelectState::onBypassPressed()
{
    if (e.isPedalOn())
        e.sendToOutputs(BYPASS, 127);
    else 
        e.sendToOutputs(BYPASS, 0);
}

void PresetSelectState::onBypassReleased()
{
}

void PresetSelectState::onScrollPressed()
{
    e.sendToOutputs(SCROLL, 64);
    setState(e, std::make_shared<PresetBrowseState>(e));
}

void PresetSelectState::onScrollReleased()
{
}

void PresetSelectState::onSavePressed()
{
    e.sendToOutputs(SAVE, 64);
}

void PresetSelectState::onSaveReleased()
{
}

void PresetSelectState::onSelectPressed()
{
    e.sendToOutputs(SELECT, 127);
}

void PresetSelectState::onSelectReleased()
{
    e.sendToOutputs(SELECT, 0);
}

void PresetSelectState::onKnobsEdited()
{
    auto preset = e.getCurrentPreset();
    preset.edited = true;
    e.setNewPreset(preset);
    e.repaint();
    setState(e, std::make_shared<PresetEditedState>(e, presetLights));
}

void PresetSelectState::paint(Graphics& g)
{
    if (e.getCurrentPreset().edited == false)
    {
        size_t lightColorIndex = e.getCurrentPreset().presetNumber / 5;
        for (size_t i = 0; i <= e.getCurrentPreset().presetNumber % 5; i++)
        {
            g.drawImageAt(presetLights[lightColorIndex], PresetLightPositions[i].x, e.getHeight() - e.getBackgroundReferencePosition().y + PresetLightPositions[i].y);
        }
    }
}
