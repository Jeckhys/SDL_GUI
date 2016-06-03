#ifndef DEF_GUI_BUTTON
#define DEF_GUI_BUTTON

#include "common.h"

typedef struct s_Button * GUI_Button;

GUI_Button GUI_Button_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_Bool enabled);

void GUI_Button_SetEnabled(GUI_Button b);
void GUI_Button_SetDisabled(GUI_Button b);
void GUI_Button_SetDefaultStyle(GUI_Button b);
void GUI_Button_SetPosition(GUI_Button b, int x, int y);
void GUI_Button_SetBorderSize(GUI_Button b, int border_size);
void GUI_Button_SetTextColor(GUI_Button b, const char * text_color);
void GUI_Button_SetPositionFromAnchor(GUI_Button b, int x, int y, GUI_Anchor anchor);
void GUI_Button_SetImage(GUI_Button b, const char * image_path, const char * hover_path);
void GUI_Button_SetHoverColor(GUI_Button b, const char * border_color, const char * button_color);
void GUI_Button_SetNormalColor(GUI_Button b, const char * border_color, const char * button_color);
void GUI_Button_SetText(GUI_Button b, const char * text, const char * font_path, unsigned int font_size);
void GUI_Button_SetPositionRelativeTo(GUI_Button b, GUI_Button ref, int offset_x, int offset_y, GUI_Anchor anchor);

unsigned int GUI_Button_GetWidth(GUI_Button b);
unsigned int GUI_Button_GetHeight(GUI_Button b);
unsigned int GUI_Button_GetPositionX(GUI_Button b);
unsigned int GUI_Button_GetPositionY(GUI_Button b);

GUI_Bool GUI_Button_IsHovered(GUI_Button b);
GUI_Bool GUI_Button_IsClicked(GUI_Button b);
GUI_Bool GUI_Button_IsEnabled(GUI_Button b);
GUI_Bool GUI_Button_IsRightClicked(GUI_Button b);

void GUI_Button_Display(GUI_Button b);

void GUI_Button_Free(GUI_Button * b);

#endif
