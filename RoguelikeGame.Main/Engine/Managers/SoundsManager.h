#pragma once

#include <list>
#include "SFML/Audio.hpp"

#include "../Core/Logger.h"

class SoundsManager
{
private:
	std::map<std::string, std::tuple<sf::SoundBuffer, sf::Sound>> _sounds;
	std::list<sf::Sound> _playQueue;
	unsigned short _expectedSize; //for logging purposes

	Logger* _logger;
	std::string GetLogMessage();
public:
	SoundsManager();
	~SoundsManager();

	void Update();
	bool Exists(const std::string& name) const;
	void SetExpectedSize(uint16_t size);

	void LoadFromFile(const std::string& name, const std::string& path);
	void LoadFromMemory(const std::string& name, const void* data, std::size_t sizeInBytes);
	void LoadFromSamples(const std::string& name, const short* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate);
	void LoadFromStream(const std::string& name, sf::InputStream& stream);

	void RemoveSound(const std::string& name);

	void PlaySoundIndependent(const std::string& name);
	void StartIndependentSounds();
	void PauseIndependentSounds();
	void StopIndependentSounds();

	sf::Sound* GetSound(const std::string& name);
	sf::SoundBuffer* GetSoundBuffer(const std::string& name);
};

