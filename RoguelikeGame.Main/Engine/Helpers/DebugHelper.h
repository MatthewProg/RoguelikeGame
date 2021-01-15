#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>

#include "../Core/Logger.h"

class DebugHelper
{
private:
	bool _debug;

	unsigned short _minFPS;
	unsigned short _maxFPS;
	unsigned int _fpsCounter;
	unsigned int _tickCounter;

	std::chrono::steady_clock::time_point _measureTime;
	std::chrono::steady_clock::time_point _fpsTime;
	std::chrono::milliseconds _measureEvery;

	void Reset();

	Logger* _logger;
public:
	DebugHelper();
	~DebugHelper();

	void Status(bool tick);

	void SetMeasureEvery(std::chrono::milliseconds milis);
	void SetDebug(bool debug);

	std::chrono::milliseconds GetMeasureEvery();
	bool GetDebug();

	void ToggleFPSInfo();
};

