#include "SoundsManager.h"

SoundsManager::SoundsManager()
{
	_sounds.clear();
	_playQueue.clear();
	_logger = Logger::GetInstance();
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

void SoundsManager::LoadFromFile(const std::string& name, const std::string& path)
{
	std::string message = " sound No" + std::to_string(_sounds.size() + 1) + " (" + name + ") from \"" + path + "\"";
	if (std::get<0>(_sounds[name]).loadFromFile(path) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	}
}

void SoundsManager::LoadFromMemory(const std::string& name, const void* data, std::size_t sizeInBytes)
{
	std::string message = " sound No" + std::to_string(_sounds.size() + 1) + " (" + name + ") from memory";

	if (std::get<0>(_sounds[name]).loadFromMemory(data, sizeInBytes) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	}
}

void SoundsManager::LoadFromSamples(const std::string& name, const short* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	std::string message = " sound No" + std::to_string(_sounds.size() + 1) + " (" + name + ") from samples";

	if (std::get<0>(_sounds[name]).loadFromSamples(samples, sampleCount, channelCount, sampleRate) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
	}
}

void SoundsManager::LoadFromStream(const std::string& name, sf::InputStream& stream)
{
	std::string message = " sound No" + std::to_string(_sounds.size() + 1) + " (" + name + ") from stream";

	if (std::get<0>(_sounds[name]).loadFromStream(stream) == true)
	{
		std::get<1>(_sounds[name]).setBuffer(std::get<0>(_sounds[name]));
		std::get<1>(_sounds[name]).setVolume(Settings::GetInstance()->SOUNDS_VOLUME);
		_logger->Log(Logger::LogType::INFO, "Loaded" + message);
	}
	else
	{
		_sounds.erase(name);
		_logger->Log(Logger::LogType::ERROR, "Unable to load" + message);
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
