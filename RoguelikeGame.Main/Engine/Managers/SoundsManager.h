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
	bool Exists(std::string name);
	void SetExpectedSize(uint16_t size);

	void LoadFromFile(std::string name, std::string path);
	void LoadFromMemory(std::string name, const void* data, std::size_t sizeInBytes);
	void LoadFromSamples(std::string name, const short* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate);
	void LoadFromStream(std::string name, sf::InputStream& stream);

	void RemoveSound(std::string name);

	void PlaySoundIndependent(std::string name);
	void StartIndependentSounds();
	void PauseIndependentSounds();
	void StopIndependentSounds();

	sf::Sound* GetSound(std::string name);
	sf::SoundBuffer* GetSoundBuffer(std::string name);
};

