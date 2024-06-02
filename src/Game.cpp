#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::STAR_MENU;
    scene = nullptr;
    img_menu = nullptr;
    img_start_menu = nullptr;
    img_win = nullptr;
    img_lose = nullptr;
    musicLose = {};
    musicWin = {};
    musicStart_Menu = {};
    musicStage1 = {};
    musicStage2 = {};
    
    InitAudioDevice();


    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    UnloadMusicStream(musicStage1);
    UnloadMusicStream(musicStage2);
    UnloadMusicStream(musicStart_Menu);
    UnloadMusicStream(musicWin);
    UnloadMusicStream(musicLose);
    CloseAudioDevice();
    
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    int display = GetCurrentMonitor();
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;
    
    //Initialise window
    InitWindow((int)w, (int)h, "The Goonies");
    ToggleFullscreen();
    

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }
   
    
    musicStage1 = LoadMusicStream("music/stage1.ogg");
    musicStage2 = LoadMusicStream("music/stage2.ogg");
    musicStart_Menu = LoadMusicStream("music/Start_Menu.ogg");
    musicWin = LoadMusicStream("music/Win.ogg");
    musicLose = LoadMusicStream("music/Lose.ogg");

    PlayMusicStream(musicStart_Menu);
    

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    if (data.LoadTexture(Resource::IMG_START_MENU, "images/START_MENU.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_start_menu = data.GetTexture(Resource::IMG_START_MENU);
    if (data.LoadTexture(Resource::IMG_YOU_WIN, "images/YOU_WIN.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_win = data.GetTexture(Resource::IMG_YOU_WIN);
    if (data.LoadTexture(Resource::IMG_YOU_LOSE, "images/GAME_OVER.png") != AppStatus::OK)
    {
		return AppStatus::ERROR;
	}
    img_lose = data.GetTexture(Resource::IMG_YOU_LOSE);
    
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    } 
            
    return AppStatus::OK;
}
void Game::FinishPlay()
{

    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    if (state == GameState::STAR_MENU || state == GameState::MAIN_MENU)
    {
        PlayMusicStream(musicStart_Menu);
        UpdateMusicStream(musicStart_Menu);
	}
    else {

        StopMusicStream(musicStart_Menu);
    }
    if (state == GameState::YOU_WIN)
    {
		PlayMusicStream(musicWin);
		UpdateMusicStream(musicWin);
	}
    else {
		StopMusicStream(musicWin);
	}
    if (state == GameState::YOU_LOSE)
    {
        PlayMusicStream(musicLose);
        UpdateMusicStream(musicLose);
    }
    else {
		StopMusicStream(musicLose);
	}
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;

    if (fade_transition.IsActive())
    {
        GameState prev_frame = state;
        state = fade_transition.Update();

        //Begin play and finish play are delayed due to the fading transition effect
        if (prev_frame == GameState::MAIN_MENU && state == GameState::PLAYING)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }
        else if (prev_frame == GameState::PLAYING && state == GameState::MAIN_MENU)
        {
            FinishPlay();
        }
    }
    switch (state)
    {
        case GameState::STAR_MENU: 
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
				state = GameState::MAIN_MENU;
			}
			break;
        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ESCAPE)) state = GameState::STAR_MENU;
            if (IsKeyPressed(KEY_SPACE))
            {
               // state = GameState::PLAYING;
                fade_transition.Set(GameState::MAIN_MENU, 60, GameState::PLAYING, 60, dst);
            }
            break;

        case GameState::PLAYING:  
            if (scene->currentStage == 1 && !scene->youWin && !scene->youLose)
            {
				PlayMusicStream(musicStage1);
                UpdateMusicStream(musicStage1);
			}
            else {
				StopMusicStream(musicStage1);
            }
            if (scene->currentStage >= 2 && !scene->youWin && !scene->youLose)
            {
                PlayMusicStream(musicStage2);
                UpdateMusicStream(musicStage2);
            }
            else {
				StopMusicStream(musicStage2);
			}
            
            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
                
            }
            else if (scene->youWin)
            {
                
                state = GameState::YOU_WIN;
                
            }
            else if (scene->youLose)
            {
                state = GameState::YOU_LOSE;
                
            }
            else 
            {
                //Game logic
                scene->Update();

            }
            break;
        case GameState::YOU_WIN:
            if (IsKeyPressed(KEY_SPACE))
            {
                

				state = GameState::MAIN_MENU;
			}
			break;
        case GameState::YOU_LOSE:
            if (IsKeyPressed(KEY_SPACE))
            {
                
                state = GameState::MAIN_MENU;
            }
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::STAR_MENU:
			DrawTexture(*img_start_menu, 0, 0, WHITE);
			break;

        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            break;

        case GameState::YOU_WIN:
			DrawTexture(*img_win, 0, 0, WHITE);
			break;

        case GameState::YOU_LOSE:
			DrawTexture(*img_lose, 0, 0, WHITE);
			break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    if (fade_transition.IsActive()) fade_transition.Render();
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);

    UnloadRenderTexture(target);
}