#include "GameEngine.h"

GameEngine* GameEngine::Instance() {
    static GameEngine* instance;
    if (instance == nullptr) {
        instance = new GameEngine();
    }
    return instance;
}

bool GameEngine::initGameEngine() {
    SDL_Window* window = SDL_CreateWindow("First SDL App", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        cout << "window init failed \n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        cout << "renderer init failed \n";
        return false;
    }

    if (TTF_Init() != 0) {
        cout << "TTF font init failed" << endl;
        return false;
    }

    return true;
}

void GameEngine::initGameWorld() {
    font = TTF_OpenFont("Assets/fonts/8bit.TTF", 30);

    divider.x = WINDOW_WIDTH / 2;
    divider.y = 0;
    divider.h = WINDOW_HEIGHT;
    divider.w = 20;

    playerPaddle.x = 20;
    playerPaddle.y = 250;
    //playerPaddle.y = WINDOW_HEIGHT * 0.5 - 50;
    playerPaddle.h = 100;
    playerPaddle.w = 20;

    aiPaddle.x = 760;
    //aiPaddle.y = 250;
    aiPaddle.x = WINDOW_WIDTH - 40;
    //aiPaddle.y = WINDOW_HEIGHT * 0.5 - 50;
    aiPaddle.h = 100;
    aiPaddle.w = 20;

    speed_x = -1;
    speed_y = -1;
    ball.x = (WINDOW_WIDTH * .5) - 30;
    ball.y = WINDOW_HEIGHT * .5;
    ball.w = 20;
    ball.h = 20;
}

void GameEngine::Render() {
    SDL_RenderClear(renderer);
    //first 3 numbers represent rbg values
    //last number represents the alpha
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &background);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 240);
    SDL_RenderFillRect(renderer, &divider);

    //setting the colors for the paddles
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &playerPaddle);
    SDL_RenderFillRect(renderer, &aiPaddle);

    //setting the color for ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball);

    string s;
    string c;
    if (aiScore != 5 || playerScore != 5) {
        s = to_string(playerScore);
        c = to_string(aiScore);
    }
    if (aiScore == 5 || playerScore == 5) {
        if (playerScore == 5) {
            s = "Player Wins";
            c = " ";
        }
        else if (aiScore == 5) {
            s = " ";
            c = "AI wins";
        }
    }
    RenderFont(s.c_str(), WINDOW_WIDTH / 2 - 300, 0, true);
    RenderFont(c.c_str(), WINDOW_WIDTH / 2 + 150, 0, true);
    SDL_RenderPresent(renderer);
}

void GameEngine::RenderFont(const char* text, int x, int y, bool isRefreshText) {
    if (isRefreshText) {
        SDL_Colour textColor = { 255,255,255,255 };
        SDL_Surface* fontSurface = TTF_RenderText_Solid(font, text, textColor);
        SDL_DestroyTexture(fontTexture);
        fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
        fontRectScore = { x,y,fontSurface->w, fontSurface->h };
    }
    SDL_RenderCopy(renderer, fontTexture, 0, &fontRectScore);
}

void GameEngine::Quit() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
}

void GameEngine::Input() {
    while (SDL_PollEvent(&event)) {
        //mouse events
        if (event.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouseX, &mouseY);
        }
        //action event
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        //for key events
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            }
        }

    }
}

void GameEngine::Update() {
    playerPaddle.y = mouseY;
    if (playerPaddle.y <0 || playerPaddle.y >(WINDOW_HEIGHT - playerPaddle.h)) {
        playerPaddle.y = WINDOW_HEIGHT - playerPaddle.h;
    }

    ball.x += speed_x;
    ball.y += speed_y;

    if (ball.x <0 || ball.x > WINDOW_WIDTH - ball.w) {
        if (ball.x < 0) {
            aiScore += 1;
        }
        else if (ball.x > WINDOW_WIDTH - ball.w) {
            playerScore += 1;
        }

        if (aiScore == 5 || playerScore == 5) {
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            speed_x = 0;
            speed_y = 0;
        }
        else {
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            speed_x = (rand() % 2 + 1) * direction[rand() % 2];
            speed_y = (rand() % 2 + 1) * direction[rand() % 2];
        }


    }

    if (ball.y <0 || ball.y >(WINDOW_HEIGHT - ball.h)) {
        speed_y = -speed_y;
    }

    if (CheckCollision(ball, playerPaddle) || CheckCollision(ball, aiPaddle)) {
        speed_x = -speed_x;
    }

    if ((int)rand() % 49 == 1) {
        aiPaddle.y = (ball.y - aiPaddle.h / 2 + ball.h / 2);
    }
    else {
        aiPaddle.y = (ball.y - aiPaddle.h / 2 + ball.h / 2) + 100;
    }

    //delays by milliseconds
    SDL_Delay(10);
}

bool GameEngine::CheckCollision(SDL_Rect A, SDL_Rect B) {
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    if (bottomA <= topB) {
        return false;
    }
    if (topA >= bottomB) {
        return false;
    }
    if (rightA <= leftB) {
        return false;
    }
    if (leftA >= rightB) {
        return false;
    }
    return true;
}