#include "SoundsManager.h"

std::string SoundsManager::GetLogMessage()
{
	std::string message = "Sounds ";
	if (_expectedSize == 0)
		message += "(" + std::to_string(_sounds.size()) + "): ";
	else
		message += "(" + std::to_string(_sounds.size()) + "/" + std::to_string(_expectedSize) + "): ";
	return message;
}

SoundsManager::SoundsManager()
{
	_sounds.clear();
	_playQueue.clear();
	_expectedSize = 0;
	_logger = Logger::GetInstance();
}

SoundsManager::~SoundsManager()
{
}

void SoundsManager::Update()
{
	auto it = _playQueue.begin();
	while (it != _playQueue.end()) 
	{
		if (it->getStatus() == sf::Sound::Stopped)
			_playQueue.erase(it++);
		else
			++it;
	}
}

bool SoundsManager::Exists(std::string name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		return true;
	else
		return false;
}

void SoundsManager::SetExpectedSize(uint16_t size)
{
	_expectedSize = size;
}

void SoundsManager::LoadFromFile(std::string name, std::string path)
{
	if (std::get<0>(_sounds[name]).loadFromFile(path) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromMemory(std::string name, const void* data, std::size_t sizeInBytes)
{
	if (std::get<0>(_sounds[name]).loadFromMemory(data, sizeInBytes) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromSamples(std::string name, const short* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	if (std::get<0>(_sounds[name]).loadFromSamples(samples, sampleCount, channelCount, sampleRate) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromStream(std::string name, sf::InputStream& stream)
{
	if (std::get<0>(_sounds[name]).loadFromStream(stream) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::RemoveSound(std::string name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		_sounds.erase(found);
}

void SoundsManager::PlaySoundIndependent(std::string name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
	{
		sf::Sound s;
		s.setBuffer(std::get<0>(found->second));
		s.setVolume(100);
		s.setLoop(false);
		_playQueue.push_back(s);
		_playQueue.back().play();
	}
}

void SoundsManager::StartIndependentSounds()
{
	for (auto& e : _playQueue)
		e.play();
}

void SoundsManager::PauseIndependentSounds()
{
	for (auto& e : _playQueue)
		e.pause();
}

void SoundsManager::StopIndependentSounds()
{
	for (auto& e : _playQueue)
		e.stop();
}

sf::Sound* SoundsManager::GetSound(std::string name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		return &std::get<1>(found->second);
	else
		return nullptr;
}

sf::SoundBuffer* SoundsManager::GetSoundBuffer(std::string name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		return &std::get<0>(found->second);
	else
		return nullptr;
}
