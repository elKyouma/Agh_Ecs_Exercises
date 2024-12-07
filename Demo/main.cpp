#include "App.hpp"
#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
    App app;
	bool isQuiting = false;
    while (!isQuiting) {
        isQuiting = app.ProcessEvents();
        app.Update();
        app.Render();
	}

	return 0;
}
