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

bool SoundsManager::Exists(const std::string& name) const
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

void SoundsManager::LoadFromFile(const std::string& name, const std::string& path)
{
	if (std::get<0>(_sounds[name]).loadFromFile(path) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromMemory(const std::string& name, const void* data, std::size_t sizeInBytes)
{
	if (std::get<0>(_sounds[name]).loadFromMemory(data, sizeInBytes) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromSamples(const std::string& name, const short* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	if (std::get<0>(_sounds[name]).loadFromSamples(samples, sampleCount, channelCount, sampleRate) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::LoadFromStream(const std::string& name, sf::InputStream& stream)
{
	if (std::get<0>(_sounds[name]).loadFromStream(stream) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, GetLogMessage() + "OK");
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, GetLogMessage() + "ERROR");
	}
}

void SoundsManager::RemoveSound(const std::string& name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		_sounds.erase(found);
}

void SoundsManager::ApplyVolume(float volume)
{
	for (auto& e : _sounds)
		std::get<1>(e.second).setVolume(volume);

	for (auto& e : _playQueue)
		e.setVolume(volume);
}

void SoundsManager::PlaySoundIndependent(const std::string& name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
	{
		sf::Sound s;
		s.setBuffer(std::get<0>(found->second));
		s.setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
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

sf::Sound* SoundsManager::GetSound(const std::string& name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		return &std::get<1>(found->second);
	else
		return nullptr;
}

sf::SoundBuffer* SoundsManager::GetSoundBuffer(const std::string& name)
{
	auto found = _sounds.find(name);
	if (found != _sounds.end())
		return &std::get<0>(found->second);
	else
		return nullptr;
}
