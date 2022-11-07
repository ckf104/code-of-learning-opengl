#ifndef __game_HPP
#define __game_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_object.hpp"

class GameLevel;

// Represents the current state of the game
enum class GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;	
    GameObject              background;
    MoveObject              paddle;
    BallObject              ball;
    GameLevel*              cur;
    uint32_t                now_level;
    bool                    Keys[1024];
    uint32_t             Width, Height;
    // constructor/destructor
    Game(uint32_t width, uint32_t height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt, GLFWwindow* window);
    void Update(float dt);
    void Render();
    void SetScreen(uint32_t width, uint32_t height);
};


#endif