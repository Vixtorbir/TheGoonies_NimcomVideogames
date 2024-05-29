#include "FadeTransition.h"

FadeTransition::FadeTransition()
{
    is_active = false;
    from = GameState::MAIN_MENU;
    to = GameState::MAIN_MENU;
    frames_from = 0;
    frames_to = 0;
    frames_counter = 0;
    rect = {};
    state = FadeState::OUT;
}
FadeTransition::~FadeTransition()
{
}
void FadeTransition::Set(GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->to = to;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = FadeState::OUT;
}
void FadeTransition::Set(GameState from, int frames_from, GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->from = from;
    this->to = to;
    this->frames_from = frames_from;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = FadeState::OUT;
}
bool FadeTransition::IsActive()
{
    return is_active;
}
GameState FadeTransition::Update()
{
    if (is_active)
    {
        frames_counter++;

        //Update the fade state based on the current state
        if (state == FadeState::OUT)
        {
            //Check if it has reached the end
            if (frames_counter >= frames_from)
            {
                state = FadeState::IN;
                frames_counter = 0;
                is_active = false;
            }
        }
       
    }
    //If the fade effect is currently in the "fade-out" state,
    //return the source state (from), otherwise return the target state (to).
    return state == FadeState::OUT ? from : to;
}

//Instead of being a square changing its color, it is a rectangle that we modify its withd to simulate it moves to the right
void FadeTransition::Render()
{
    float width;

    if (is_active)
    {
        if (state == FadeState::OUT)
        {
            width = (float)rect.width * ((float)frames_counter / frames_from);
            //Black rectangle with alpha value from 0 to 1
			DrawRectangle(rect.x, rect.y, width, rect.height, BLACK);
        }
    }
}
