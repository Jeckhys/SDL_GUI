#ifndef DEF_GUI_WINDOW
#define DEF_GUI_WINDOW

#include "common.h"

typedef struct s_Window * GUI_Window;

GUI_Window GUI_Window_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_WindowType type, GUI_Bool enabled);

void GUI_Window_SetEnabled(GUI_Window w);
void GUI_Window_SetDisabled(GUI_Window w);
void GUI_Window_SetBorderSize(GUI_Window w, int border_size);
void GUI_Window_SetDragnDrop(GUI_Window w, GUI_Bool dragndrop);
void GUI_Window_SetPosition(GUI_Window w, unsigned int x, unsigned int y);
void GUI_Window_SetFont(GUI_Window w, const char * font_path, unsigned int font_size);
void GUI_Window_SetButton1Color(GUI_Window w, const char * border_color, const char * button_color);
void GUI_Window_SetButtonCloseColor(GUI_Window w, const char * border_color, const char * button_color);
void GUI_Window_SetButton1ColorHover(GUI_Window w, const char * border_color, const char * button_color);
void GUI_Window_SetTextColor(GUI_Window w, const char * header_text_color, const char * content_text_color);
void GUI_Window_SetButtonCloseColorHover(GUI_Window w, const char * border_color, const char * button_color);
void GUI_Window_SetHeaderText(GUI_Window w, const char * header_text, const char * font_path, unsigned int font_size);
void GUI_Window_SetContentText(GUI_Window w, const char * content_text, const char * font_path, unsigned int font_size);
void GUI_Window_SetColor(GUI_Window w, const char * border_color, const char * header_color, const char * background_color, const char * menu_color);

GUI_Bool GUI_Window_IsEnabled(GUI_Window w);
GUI_Bool GUI_Window_IsHeaderHovered(GUI_Window w);

void GUI_Window_HandleDragnDrop(GUI_Window w);
GUI_WindowAnswer GUI_Window_HandleButtonInput(GUI_Window w);

void GUI_Window_Display(GUI_Window w);

void GUI_Window_Free(GUI_Window * w);

#endif
