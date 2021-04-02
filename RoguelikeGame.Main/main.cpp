#include "Engine/Core/Game.h"

int main(int argc, char* argv[])
{
    LogOptions options;
    options.fileAppend = false;
    options.filePath = "game.log";
    options.loggingPattern = "[{type}] {date} {time} - {message}";
    options.outputStream = LogOptions::LogOutput::CONSOLE;
    options.ignoredTypes["DEBUG"] = true;

    if (argc >= 2 && _stricmp(argv[1], "-d") == 0)
    {
        options.ignoredTypes["DEBUG"] = false;
        Settings::GetInstance()->DEBUG.NewValue(true);
    }

    Game game(options);
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