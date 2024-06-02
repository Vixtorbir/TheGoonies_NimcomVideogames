#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"


enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void SetStage(int stage);
    void Render();
    void Release();
    bool youWin = false;
    bool youLose = false;
    bool Helmet = false;
    bool Shoes = false;
    int currentStage = 0;

private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderEnemies() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    

    Player *player;
    Enemy  *enemy;
    TileMap *level;
    std::vector<Object*> objects;
    std::vector<Enemy*> enemies;
    Camera2D camera;
    DebugMode debug;
    Sound  sfxItem;
};

