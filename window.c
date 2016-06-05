#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "button.h"
#include "window.h"

struct s_Window {

	SDL_Surface ** screen;
	SDL_Event * event;

	SDL_Surface * window;
	SDL_Surface * text_header;
	SDL_Surface * text_content;

	SDL_Rect dimensions;
	SDL_Rect dimensions_menu;
	SDL_Rect dimensions_header;
	SDL_Rect dimensions_background;
	SDL_Rect dimensions_text_header;
	SDL_Rect dimensions_text_content;

	GUI_Button button_close;
	GUI_Button button_1;
	GUI_Button button_2;

	char menu_color[7];
	char border_color[7];
	char header_color[7];
	char background_color[7];
	char header_text_color[7];
	char content_text_color[7];

	unsigned int font_size;
	unsigned int buttons_header_offset;

	char font_path[512];
	char header_text_value[GUI_WINDOW_MAXSIZETEXT_HEADER];
	char content_text_value[GUI_WINDOW_MAXSIZETEXT_CONTENT];

	GUI_Bool enabled;

	GUI_WindowType type;

};



static void GUI_UpdateTextPositions(GUI_Window w)
{

	if (w->text_header != NULL)
	{
		w->dimensions_text_header.x = (Sint16)(w->dimensions.x + w->dimensions_header.x + 2 * w->buttons_header_offset);
		w->dimensions_text_header.y = (Sint16)(w->dimensions.y + w->dimensions_header.y + (w->dimensions_header.h - w->text_header->h) / 2);
	}

	if (w->text_content != NULL)
	{
		w->dimensions_text_content.x = (Sint16)(w->dimensions.x + w->dimensions_background.x + 2 * w->buttons_header_offset);
		w->dimensions_text_content.y = (Sint16)(w->dimensions.y + w->dimensions_background.y + 2 * w->buttons_header_offset);
	}

}

static int GUI_GetInfValue(int a, int b)
{

	return (a > b) ? b : a;

}

static Uint8 GUI_GetRedFromHex(const char * hex)
{

	int sum = 0;

	sum = (hex[0] >= '0' && hex[0] <= '9') ? sum + (hex[0] - '0') * 16 : sum + (hex[0] - '0' - 7) * 16;
	sum = (hex[1] >= '0' && hex[1] <= '9') ? sum + hex[1] - '0' : sum + hex[1] - '0' - 7;
	sum = (sum > 255) ? 255 : sum;
	sum = (sum < 0) ? 0 : sum;

	return (Uint8)(sum);

}

static Uint8 GUI_GetBlueFromHex(const char * hex)
{

	int sum = 0;

	sum = (hex[4] >= '0' && hex[4] <= '9') ? sum + (hex[4] - '0') * 16 : sum + (hex[4] - '0' - 7) * 16;
	sum = (hex[5] >= '0' && hex[5] <= '9') ? sum + hex[5] - '0' : sum + hex[5] - '0' - 7;
	sum = (sum > 255) ? 255 : sum;
	sum = (sum < 0) ? 0 : sum;

	return (Uint8)(sum);

}

static Uint8 GUI_GetGreenFromHex(const char * hex)
{

	int sum = 0;

	sum = (hex[2] >= '0' && hex[2] <= '9') ? sum + (hex[2] - '0') * 16 : sum + (hex[2] - '0' - 7) * 16;
	sum = (hex[3] >= '0' && hex[3] <= '9') ? sum + hex[3] - '0' : sum + hex[3] - '0' - 7;
	sum = (sum > 255) ? 255 : sum;
	sum = (sum < 0) ? 0 : sum;

	return (Uint8)(sum);

}

static SDL_Color GUI_GetColorFromHex(const char * hex)
{

	SDL_Color color;

	color.r = GUI_GetRedFromHex(hex);
	color.g = GUI_GetGreenFromHex(hex);
	color.b = GUI_GetBlueFromHex(hex);

	return color;

}



GUI_Window GUI_Window_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_WindowType type, GUI_Bool enabled)
{

	GUI_Window w = (GUI_Window)malloc(sizeof(struct s_Window));
	assert(w != NULL);
	
	assert(screen != NULL);
	w->screen = screen;

	assert(event != NULL);
	w->event = event;

	w->dimensions.w = (Uint16)(width);
	w->dimensions.h = (Uint16)(height);

	if (w->dimensions.w > (*screen)->w)
		w->dimensions.w = (Uint16)((*screen)->w);

	if (w->dimensions.h > (*screen)->h)
		w->dimensions.h = (Uint16)((*screen)->h);

	w->window = NULL;
	w->text_header = NULL;
	w->text_content = NULL;

	w->enabled = enabled;

	w->type = type;

	w->buttons_header_offset = GUI_WINDOW_DEFAULT_HEADER_OFFSET;

	GUI_Window_SetColor(w, GUI_WINDOW_DEFAULT_BORDER_COLOR, GUI_WINDOW_DEFAULT_HEADER_COLOR, GUI_WINDOW_DEFAULT_BACKGROUND_COLOR, GUI_WINDOW_DEFAULT_MENU_COLOR);
	GUI_Window_SetBorderSize(w, GUI_WINDOW_DEFAULT_BORDER_SIZE);

	w->button_close = GUI_Button_Init(screen, event, w->dimensions_header.h - w->buttons_header_offset * 2, w->dimensions_header.h - w->buttons_header_offset * 2, TRUE);

	if (type == INFORMATION)
		w->button_1 = GUI_Button_Init(screen, event, width * 0.3, w->dimensions_menu.h - w->buttons_header_offset * 2, TRUE);
	else
		w->button_1 = NULL;

	w->button_2 = NULL;

	return w;

}



void GUI_Window_SetEnabled(GUI_Window w)
{

	w->enabled = TRUE;

}

void GUI_Window_SetDisabled(GUI_Window w)
{

	w->enabled = FALSE;

}

void GUI_Window_SetBorderSize(GUI_Window w, int border_size)
{

	border_size = (border_size > GUI_GetInfValue(w->dimensions.w, w->dimensions.h) / 2) ? GUI_GetInfValue(w->dimensions.w, w->dimensions.h) / 2 : border_size;

	w->dimensions_header.w = (Uint16)(w->dimensions.w - border_size * 2);
	w->dimensions_header.h = (Uint16)(w->dimensions.h - border_size * 2) * GUI_WINDOW_DEFAULT_HEADER_PERCENT_HEIGHT / 100;
	w->dimensions_header.x = (Sint16)(border_size);
	w->dimensions_header.y = (Sint16)(border_size);

	w->dimensions_menu.w = (Uint16)(w->dimensions.w - border_size * 2);
	w->dimensions_menu.h = (Uint16)(w->dimensions.h - border_size * 2) * GUI_WINDOW_DEFAULT_HEADER_PERCENT_HEIGHT / 100;
	w->dimensions_menu.x = (Sint16)(border_size);
	w->dimensions_menu.y = (Sint16)(w->dimensions.h - w->dimensions_menu.h - border_size);

	w->dimensions_background.w = (Uint16)(w->dimensions.w - border_size * 2);
	w->dimensions_background.h = (Uint16)(w->dimensions.h - w->dimensions_header.h - w->dimensions_menu.h - 2 * GUI_WINDOW_DEFAULT_BORDER_SIZE - 2 * GUI_WINDOW_DEFAULT_BORDER_SIZE);
	w->dimensions_background.x = (Sint16)(border_size);
	w->dimensions_background.y = (Sint16)((w->dimensions.h - w->dimensions_background.h) / 2);

	GUI_Window_SetColor(w, w->border_color, w->header_color, w->background_color, w->menu_color);

}

void GUI_Window_SetPosition(GUI_Window w, unsigned int x, unsigned int y)
{

	if (x == GUI_CENTERED)
		w->dimensions.x = (Sint16)(((*(w->screen))->w - w->dimensions.w) / 2);
	else
	{

		w->dimensions.x = (Sint16)(x);
		w->dimensions.x = (w->dimensions.x >= ((*(w->screen))->w - w->dimensions.w)) ? (Sint16)((*(w->screen))->w - w->dimensions.w) : w->dimensions.x;
		w->dimensions.x = (w->dimensions.x < 0) ? 0 : w->dimensions.x;

	}

	if (y == GUI_CENTERED)
		w->dimensions.y = (Sint16)(((*(w->screen))->h - w->dimensions.h) / 2);
	else
	{

		w->dimensions.y = (Sint16)(y);
		w->dimensions.y = (w->dimensions.y >= ((*(w->screen))->h - w->dimensions.h)) ? (Sint16)((*(w->screen))->h - w->dimensions.h) : w->dimensions.y;
		w->dimensions.y = (w->dimensions.y < 0) ? 0 : w->dimensions.y;

	}

	GUI_Button_SetPosition(w->button_close, w->dimensions.x + w->dimensions_header.x + w->dimensions_header.w - GUI_Button_GetWidth(w->button_close) - w->buttons_header_offset, w->dimensions.y + w->dimensions_header.y + w->buttons_header_offset);
	
	if(w->button_1 != NULL && w->type == INFORMATION)
		GUI_Button_SetPosition(w->button_1, w->dimensions.x + (w->dimensions_menu.w - GUI_Button_GetWidth(w->button_1)) / 2, w->dimensions.y + w->dimensions_menu.y + w->buttons_header_offset);

	GUI_UpdateTextPositions(w);

}

void GUI_Window_SetFont(GUI_Window w, const char * font_path, unsigned int font_size)
{

	assert(font_path != NULL);

	strcpy_s(w->font_path, _countof(w->font_path), font_path);
	w->font_size = font_size;

	if (w->text_header != NULL)
		GUI_Window_SetHeaderText(w, w->header_text_value, w->font_path, w->font_size);

	if (w->text_content != NULL)
		GUI_Window_SetContentText(w, w->content_text_value, w->font_path, w->font_size);

}

void GUI_Window_SetButton1Color(GUI_Window w, const char * border_color, const char * button_color)
{

	GUI_Button_SetNormalColor(w->button_1, border_color, button_color);

}

void GUI_Window_SetButtonCloseColor(GUI_Window w, const char * border_color, const char * button_color)
{

	GUI_Button_SetNormalColor(w->button_close, border_color, button_color);

}

void GUI_Window_SetButton1ColorHover(GUI_Window w, const char * border_color, const char * button_color)
{

	GUI_Button_SetHoverColor(w->button_1, border_color, button_color);

}

void GUI_Window_SetTextColor(GUI_Window w, const char * header_text_color, const char * content_text_color)
{

	strcpy_s(w->header_text_color, _countof(w->header_text_color), header_text_color);
	strcpy_s(w->content_text_color, _countof(w->content_text_color), content_text_color);

	if (w->text_header != NULL)
		GUI_Window_SetHeaderText(w, w->header_text_value, w->font_path, w->font_size);

	if (w->text_content != NULL)
		GUI_Window_SetContentText(w, w->content_text_value, w->font_path, w->font_size);

	if (strlen(w->font_path))
	{
		GUI_Button_SetText(w->button_close, "x", w->font_path, w->font_size);
		GUI_Button_SetTextColor(w->button_close, w->header_text_color);
		GUI_Button_SetText(w->button_1, "OK", w->font_path, w->font_size);
		GUI_Button_SetTextColor(w->button_1, w->header_text_color);
	}

}

void GUI_Window_SetButtonCloseColorHover(GUI_Window w, const char * border_color, const char * button_color)
{

	GUI_Button_SetHoverColor(w->button_close, border_color, button_color);

}

void GUI_Window_SetHeaderText(GUI_Window w, const char * header_text, const char * font_path, unsigned int font_size)
{

	assert(header_text != NULL);
	assert(font_path != NULL);

	strcpy_s(w->font_path, _countof(w->font_path), font_path);
	w->font_size = font_size;

	strcpy_s(w->header_text_value, _countof(w->header_text_value), header_text);

	if (w->text_header != NULL)
	{
		SDL_FreeSurface(w->text_header);
		w->text_header = NULL;
	}

	TTF_Font * font = TTF_OpenFont(font_path, font_size);
	assert(font != NULL);

	w->text_header = TTF_RenderText_Blended(font, header_text, GUI_GetColorFromHex(w->header_text_color));
	assert(w->text_header != NULL);

	TTF_CloseFont(font);

	GUI_UpdateTextPositions(w);

	GUI_Button_SetText(w->button_close, "x", w->font_path, w->font_size);
	GUI_Button_SetTextColor(w->button_close, w->header_text_color);
	GUI_Button_SetText(w->button_1, "OK", w->font_path, w->font_size);
	GUI_Button_SetTextColor(w->button_1, w->header_text_color);

}

void GUI_Window_SetContentText(GUI_Window w, const char * content_text, const char * font_path, unsigned int font_size)
{

	assert(content_text != NULL);
	assert(font_path != NULL);

	strcpy_s(w->font_path, _countof(w->font_path), font_path);
	w->font_size = font_size;

	strcpy_s(w->content_text_value, _countof(w->content_text_value), content_text);

	if (w->text_content != NULL)
	{
		SDL_FreeSurface(w->text_content);
		w->text_content = NULL;
	}

	TTF_Font * font = TTF_OpenFont(font_path, font_size);
	assert(font != NULL);

	w->text_content = TTF_RenderText_Blended(font, content_text, GUI_GetColorFromHex(w->content_text_color));
	assert(w->text_content != NULL);

	TTF_CloseFont(font);

	GUI_UpdateTextPositions(w);

	GUI_Button_SetText(w->button_close, "x", w->font_path, w->font_size);
	GUI_Button_SetTextColor(w->button_close, w->header_text_color);
	GUI_Button_SetText(w->button_1, "OK", w->font_path, w->font_size);
	GUI_Button_SetTextColor(w->button_1, w->header_text_color);

}

void GUI_Window_SetColor(GUI_Window w, const char * border_color, const char * header_color, const char * background_color, const char * menu_color)
{

	if (w->window == NULL)
	{
		w->window = SDL_CreateRGBSurface(SDL_HWSURFACE, w->dimensions.w, w->dimensions.h, 32, 0, 0, 0, 0);
		assert(w->window != NULL);
	}

	strcpy_s(w->border_color, _countof(w->border_color), border_color);
	strcpy_s(w->header_color, _countof(w->header_color), header_color);
	strcpy_s(w->background_color, _countof(w->background_color), background_color);
	strcpy_s(w->menu_color, _countof(w->menu_color), menu_color);

	SDL_FillRect(w->window, NULL, SDL_MapRGB((*(w->screen))->format, GUI_GetRedFromHex(w->border_color), GUI_GetGreenFromHex(w->border_color), GUI_GetBlueFromHex(w->border_color)));
	SDL_FillRect(w->window, &(w->dimensions_background), SDL_MapRGB((*(w->screen))->format, GUI_GetRedFromHex(w->background_color), GUI_GetGreenFromHex(w->background_color), GUI_GetBlueFromHex(w->background_color)));
	SDL_FillRect(w->window, &(w->dimensions_header), SDL_MapRGB((*(w->screen))->format, GUI_GetRedFromHex(w->header_color), GUI_GetGreenFromHex(w->header_color), GUI_GetBlueFromHex(w->header_color)));
	SDL_FillRect(w->window, &(w->dimensions_menu), SDL_MapRGB((*(w->screen))->format, GUI_GetRedFromHex(w->menu_color), GUI_GetGreenFromHex(w->menu_color), GUI_GetBlueFromHex(w->menu_color)));

}



GUI_Bool GUI_Window_IsEnabled(GUI_Window w)
{

	return w->enabled;

}



GUI_WindowAnswer GUI_Window_HandleButtonInput(GUI_Window w)
{

	if (GUI_Button_IsDownClicked(w->button_close))
	{
		w->enabled = FALSE;
		return CLOSE;
	}

	else if (w->type == INFORMATION)
	{

		if (GUI_Button_IsClicked(w->button_1))
		{
			w->enabled = FALSE;
			return OK;
		}

	}

	return NOTHING;

}



void GUI_Window_Display(GUI_Window w)
{

	if (!w->enabled)
		return;

	SDL_BlitSurface(w->window, NULL, *(w->screen), &(w->dimensions));
	SDL_BlitSurface(w->text_header, NULL, *(w->screen), &(w->dimensions_text_header));
	SDL_BlitSurface(w->text_content, NULL, *(w->screen), &(w->dimensions_text_content));
	
	GUI_Button_Display(w->button_close);

	if (w->button_1 != NULL)
		GUI_Button_Display(w->button_1);

	if (w->button_2 != NULL)
		GUI_Button_Display(w->button_2);

}



void GUI_Window_Free(GUI_Window * w)
{

	assert((*w) != NULL);

	if ((*w)->window)
		SDL_FreeSurface((*w)->window);

	if ((*w)->text_header != NULL)
		SDL_FreeSurface((*w)->text_header);

	if ((*w)->text_content != NULL)
		SDL_FreeSurface((*w)->text_content);

	if ((*w)->button_close != NULL)
		GUI_Button_Free(&((*w)->button_close));

	if ((*w)->button_1 != NULL)
		GUI_Button_Free(&((*w)->button_1));

	if ((*w)->button_2 != NULL)
		GUI_Button_Free(&((*w)->button_2));

	free(*w);
	*w = NULL;

}
