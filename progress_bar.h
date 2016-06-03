#ifndef DEF_GUI_PROGRESS_BAR
#define DEF_GUI_PROGRESS_BAR

#include "common.h"

typedef struct s_ProgressBar * GUI_ProgressBar;

GUI_ProgressBar GUI_ProgressBar_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_Bool enabled);

void GUI_ProgressBar_Increase(GUI_ProgressBar pb);
void GUI_ProgressBar_Decrease(GUI_ProgressBar pb);
void GUI_ProgressBar_SetEnabled(GUI_ProgressBar pb);
void GUI_ProgressBar_SetDisabled(GUI_ProgressBar pb);
void GUI_ProgressBar_SetValue(GUI_ProgressBar pb, int value);
void GUI_ProgressBar_SetPosition(GUI_ProgressBar pb, int x, int y);
void GUI_ProgressBar_SetMaxValue(GUI_ProgressBar pb, int max_value);
void GUI_ProgressBar_SetMinValue(GUI_ProgressBar pb, int min_value);
void GUI_ProgressBar_SetBorderSize(GUI_ProgressBar pb, unsigned int border_size);
void GUI_ProgressBar_SetPositionFromAnchor(GUI_ProgressBar pb, int x, int y, GUI_Anchor anchor);
void GUI_ProgressBar_SetColor(GUI_ProgressBar pb, const char * border_color, const char * background_color, const char * content_color);

int GUI_ProgressBar_GetValue(GUI_ProgressBar pb);
int GUI_ProgressBar_GetMaxValue(GUI_ProgressBar pb);
int GUI_ProgressBar_GetMinValue(GUI_ProgressBar pb);
unsigned int GUI_ProgressBar_GetWidth(GUI_ProgressBar pb);
unsigned int GUI_ProgressBar_GetHeight(GUI_ProgressBar pb);
unsigned int GUI_ProgressBar_GetPositionX(GUI_ProgressBar pb);
unsigned int GUI_ProgressBar_GetPositionY(GUI_ProgressBar pb);

GUI_Bool GUI_ProgressBar_IsFull(GUI_ProgressBar pb);
GUI_Bool GUI_ProgressBar_IsEmpty(GUI_ProgressBar pb);
GUI_Bool GUI_ProgressBar_IsEnabled(GUI_ProgressBar pb);
GUI_Bool GUI_ProgressBar_IsHovered(GUI_ProgressBar pb);
GUI_Bool GUI_ProgressBar_IsClicked(GUI_ProgressBar pb);
GUI_Bool GUI_ProgressBar_IsRightClicked(GUI_ProgressBar pb);

void GUI_ProgressBar_Display(GUI_ProgressBar pb);

void GUI_ProgressBar_Free(GUI_ProgressBar * pb);

#endif
