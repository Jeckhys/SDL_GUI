#ifndef DEF_GUI_MENU
#define DEF_GUI_MENU

#include "common.h"

typedef struct s_Menu * GUI_Menu;

GUI_Menu GUI_Menu_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int buttons_width, unsigned int buttons_height, GUI_Bool enabled);

void GUI_Menu_CustomizeButtonsTextColor(GUI_Menu m, const char * text_color);
void GUI_Menu_CustomizeButtonTextColor(GUI_Menu m, int id, const char * text_color);
void GUI_Menu_CustomizeButtons(GUI_Menu m, const char * border_color, const char * content_color);
void GUI_Menu_CustomizeButtonsTextFont(GUI_Menu m, const char * font_path, unsigned int font_size);
void GUI_Menu_CustomizeButtonsHover(GUI_Menu m, const char * border_color, const char * content_color);
int GUI_Menu_CreateButton(GUI_Menu m, const char * text, const char * font_path, unsigned int font_size);
void GUI_Menu_CustomizeButton(GUI_Menu m, int id, const char * border_color, const char * content_color);
void GUI_Menu_CustomizeButtonTextFont(GUI_Menu m, int id, const char * font_path, unsigned int font_size);
void GUI_Menu_CustomizeButtonHover(GUI_Menu m, int id, const char * border_color, const char * content_color);

void GUI_Menu_SetEnabled(GUI_Menu m);
void GUI_Menu_SetDisabled(GUI_Menu m);
void GUI_Menu_SetPosition(GUI_Menu, int x, int y);
void GUI_Menu_SetBorderSize(GUI_Menu m, unsigned int border_size);
void GUI_Menu_SetPositionFromAnchor(GUI_Menu m, int x, int y, GUI_Anchor anchor);
void GUI_Menu_SetColor(GUI_Menu m, const char * border_color, const char * background_color);

unsigned int GUI_Menu_GetWidth(GUI_Menu m);
unsigned int GUI_Menu_GetHeight(GUI_Menu m);
unsigned int GUI_Menu_GetPositionX(GUI_Menu m);
unsigned int GUI_Menu_GetPositionY(GUI_Menu m);
unsigned int GUI_Menu_GetBorderSize(GUI_Menu m);

GUI_Bool GUI_Menu_IsEnabled(GUI_Menu m);
GUI_Bool GUI_Menu_IsHovered(GUI_Menu m);
GUI_Bool GUI_Menu_IsClicked(GUI_Menu m);
GUI_Bool GUI_Menu_IsRightClicked(GUI_Menu m);
GUI_Bool GUI_Menu_IsButtonHovered(GUI_Menu m, int numero);
GUI_Bool GUI_Menu_IsButtonClicked(GUI_Menu m, int numero);
GUI_Bool GUI_Menu_IsButtonRightClicked(GUI_Menu m, int numero);

void GUI_Menu_Display(GUI_Menu m);
void GUI_Menu_Resize(GUI_Menu m, unsigned int width, unsigned int height);

void GUI_Menu_Free(GUI_Menu * m);

#endif
