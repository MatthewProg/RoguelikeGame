#include "Engine/Core/Game.h"

int main()
{
    LogOptions options;
    options.fileAppend = false;
    options.filePath = "game.log";
    options.loggingPattern = "[{type}] {date} {time} - {message}";
    options.outputStream = LogOptions::LogOutput::CONSOLE;
#ifdef NDEBUG
    //options.ignoredTypes["DEBUG"] = true;
#endif

    Logger::GetInstance(options);

    Game game(sf::VideoMode(1024, 576), "It's a game");
    game.Start();

    while (game.isRunning())
    {
        game.EventUpdate();
        game.Update();

        game.Clear();
        game.Draw();
        game.Display();
    }

    return 0;
}