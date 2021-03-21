#include "LightsTimer.h"

LightsTimer::LightsTimer(std::function<void()> theCallback) : callback(theCallback)
{
}

LightsTimer::~LightsTimer()
{
}

void LightsTimer::starLightstTimer(int _timeInMilliseconds)
{
	timeInMilliseconds = _timeInMilliseconds;
	startTimer(timeInMilliseconds);
}

void LightsTimer::stopLightsTimer()
{
	stopTimer();
}

void LightsTimer::resetTimer()
{
	stopTimer();
	startTimer(timeInMilliseconds);
}

void LightsTimer::timerCallback()
{
	if (callback)
		callback();
}
