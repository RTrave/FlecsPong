///
/// main.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <ctime>
#include <iostream>
#include <algorithm>

#include "core/Game.hpp"

char* getCmdOption(char **begin, char **end, const std::string &option)
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argsc, char *argsv[])
{
    // Seeds RNG.
    std::srand(std::time(nullptr));

    try
    {
        fp::Game pong("Flecs Pong", 640, 480,
                SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS
                        | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);
        if (cmdOptionExists(argsv, argsv + argsc, "-mt"))
        {
            return pong.run_MT();
        }
        return pong.run();
    }
    catch (const std::exception &e)
    {
        // Error message printed to console, dont need to reprint exception,
        // just wait for player input.

        std::cin.get();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        // Error message printed to console, dont need to reprint exception,
        // just wait for player input.

        std::cin.get();
        return EXIT_FAILURE;
    }
}
