#ifndef DEF_GUI_INPUT
#define DEF_GUI_INPUT

#include "common.h"

typedef struct s_Input * GUI_Input;

GUI_Input GUI_Input_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, const char * font_path, unsigned int font_size);

void GUI_Input_SetEnabled(GUI_Input i);
void GUI_Input_SetDisabled(GUI_Input i);
void GUI_Input_SetPosition(GUI_Input i, int x, int y);
void GUI_Input_SetBorderSize(GUI_Input i, int border_size);
void GUI_Input_SetMaxSize(GUI_Input i, unsigned int max_size);
void GUI_Input_SetTextColor(GUI_Input i, const char * text_color);
void GUI_Input_SetTextOffset(GUI_Input i, unsigned int text_offset);
void GUI_Input_SetPlaceholder(GUI_Input i, const char * placeholder);
void GUI_Input_SetPositionFromAnchor(GUI_Input i, int x, int y, GUI_Anchor anchor);
void GUI_Input_SetTextFont(GUI_Input i, const char * font_path, unsigned int font_size);
void GUI_Input_SetHoverColor(GUI_Input i, const char * border_color, const char * background_color);
void GUI_Input_SetNormalColor(GUI_Input i, const char * border_color, const char * background_color);
void GUI_Input_SetText(GUI_Input i, const char * text, const char * font_path, unsigned int font_size);

const char * GUI_InputGetValue(GUI_Input i);
unsigned int GUI_Input_GetWidth(GUI_Input i);
unsigned int GUI_Input_GetHeight(GUI_Input i);
unsigned int GUI_Input_GetMaxSize(GUI_Input i);
unsigned int GUI_Input_GetPositionX(GUI_Input i);
unsigned int GUI_Input_GetPositionY(GUI_Input i);

GUI_Bool GUI_Input_IsEmpty(GUI_Input i);
GUI_Bool GUI_Input_IsEnabled(GUI_Input i);
GUI_Bool GUI_Input_IsHovered(GUI_Input i);
GUI_Bool GUI_Input_IsClicked(GUI_Input i);
GUI_Bool GUI_Input_IsRightClicked(GUI_Input i);

void GUI_Input_HandleInput(GUI_Input i);

void GUI_Input_Display(GUI_Input i);

void GUI_Input_Free(GUI_Input * i);

#endif
