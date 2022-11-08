#include "GameEngine.h"

int main(int argc, char* agv[])
{
    if (!GameEngine::Instance()->initGameEngine())return -1;
    GameEngine::Instance()->initGameWorld();
    //set up the game loop
    while (GameEngine::Instance()->IsRunning()) {
        GameEngine::Instance()->Input();
        GameEngine::Instance()->Update();
        GameEngine::Instance()->Render();
    }
    GameEngine::Instance()->Quit();
    return 0;
}



