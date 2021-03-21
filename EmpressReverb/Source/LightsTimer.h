#pragma once

#include <JuceHeader.h>

//We create a wrapper to Timer in order to have more than one timers per class

class LightsTimer: private Timer
{
public:
	LightsTimer(std::function<void()> theCallback);
	~LightsTimer() override;

	void starLightstTimer(int timeInMilliseconds);
	void stopLightsTimer();
	void resetTimer();

	void timerCallback() override;

private:
	std::function<void()> callback = nullptr;
	int timeInMilliseconds = 100;
};