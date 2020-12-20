#include "DebugHelper.h"

void DebugHelper::Reset()
{
	_minFPS = USHRT_MAX;
	_maxFPS = 0;
	_fpsCounter = 0;
	_tickCounter = 0;
	_measureTime = std::chrono::steady_clock::now();
	_fpsTime = std::chrono::steady_clock::now();
}

DebugHelper::DebugHelper()
{
	_logger = Logger::GetInstance();
	_debug = false;
	_measureEvery = std::chrono::milliseconds(1000);
	Reset();
}

DebugHelper::~DebugHelper()
{
}

void DebugHelper::Status(bool tick)
{
	if (_debug == false) return;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	auto fpsDur = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _fpsTime).count();
	auto measureDur = std::chrono::duration_cast<std::chrono::milliseconds>(now - _measureTime).count();

	unsigned short fps = 1000000000 / fpsDur;
	if (fps < _minFPS) _minFPS = fps;
	if (fps > _maxFPS) _maxFPS = fps;
	
	if (tick) _tickCounter++;
	_fpsCounter++;

	if (measureDur > _measureEvery.count())
	{
		float avgFPS = _fpsCounter / (measureDur / 1000.00);
		float avgTPS = _tickCounter / (measureDur / 1000.00);
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << "FPS: avg(" << avgFPS << ")  min(" << _minFPS <<  ")  max(" << _maxFPS << ")  TPS: " << avgTPS;
		_logger->Log(Logger::LogType::DEBUG, ss.str());
		Reset();
	}
	_fpsTime = now;
}

void DebugHelper::SetMeasureEvery(std::chrono::milliseconds milis)
{
	_measureEvery = milis;
}

void DebugHelper::SetDebug(bool debug)
{
	_debug = debug;
	if (debug)
		Reset();
}

std::chrono::milliseconds DebugHelper::GetMeasureEvery()
{
	return _measureEvery;
}

bool DebugHelper::GetDebug()
{
	return _debug;
}
