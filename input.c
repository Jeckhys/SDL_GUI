#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "input.h"

struct s_Input {

	SDL_Surface ** screen;
	SDL_Event * event;

	SDL_Surface * color_normal;
	SDL_Surface * color_hover;
	SDL_Surface * text;

	SDL_Rect dimensions;
	SDL_Rect dimensions_background;
	SDL_Rect position_text;
	SDL_Rect text_dimensions;

	unsigned int font_size;
	unsigned int text_size;
	unsigned int text_offset;
	unsigned int text_max_size;

	char border_color[7];
	char border_color_hover[7];
	char content_color[7];
	char content_color_hover[7];
	char text_color[7];

	char text_value[GUI_INPUT_MAXSIZETEXT];
	char font_path[512];
	char text_placeholder[64];

	GUI_Bool hover;
	GUI_Bool focus;
	GUI_Bool enabled;

};



static void GUI_DelLetter(GUI_Input i)
{

	if(i->text_size > 0)
		i->text_value[--i->text_size] = '\0';

	if (!i->text_size)
		strcpy_s(i->text_value, _countof(i->text_value), i->text_placeholder);

}

static int GUI_GetInfValue(int a, int b)
{

	return (a > b) ? b : a;

}

static GUI_Bool GUI_IsCharValid(Uint16 c)
{

	return (c >= 32 && c <= 126);

}

static void GUI_AdaptTextSize(GUI_Input i)
{

	if (i->text == NULL)
		return;

	if ((i->text->w + 2 * i->text_offset) > i->dimensions.w)
		i->text_dimensions.x = (Sint16)(i->text->w - i->dimensions.w + 2 * i->text_offset);
	else
		i->text_dimensions.x = 0;

}

static void GUI_InputTextCenter(GUI_Input i)
{

	if (i->text != NULL)
	{

		i->position_text.x = (Sint16)(i->dimensions.x + i->text_offset);
		i->position_text.y = (Sint16)(i->dimensions.y + (i->dimensions.h - i->text->h) / 2);

	}

}

static void GUI_AddLetter(GUI_Input i, Uint16 c)
{

	if (i->text_size < i->text_max_size - 1)
	{
		i->text_value[i->text_size] = (char)(c);
		i->text_value[++i->text_size] = '\0';
	}

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



GUI_Input GUI_Input_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, const char * font_path, unsigned int font_size)
{

	GUI_Input i = (GUI_Input)malloc(sizeof(struct s_Input));
	assert(i != NULL);

	assert(screen != NULL);
	i->screen = screen;
	assert(screen != NULL);
	i->event = event;

	i->text_size = 0;

	i->color_normal = NULL;
	i->color_hover = NULL;
	i->text = NULL;

	i->dimensions.w = (Uint16)(width);
	i->dimensions.h = (Uint16)(height);

	if (i->dimensions.w > (*screen)->w)
		i->dimensions.w = (Uint16)((*screen)->w);

	if (i->dimensions.h > (*screen)->h)
		i->dimensions.h = (Uint16)((*screen)->h);

	i->text_dimensions.x = 0;
	i->text_dimensions.y = 0;
	i->text_dimensions.w = (Uint16)(width - GUI_INPUT_DEFAULT_TEXT_OFFSET);
	i->text_dimensions.h = (Uint16)(width - GUI_INPUT_DEFAULT_TEXT_OFFSET);

	i->hover = FALSE;
	i->focus = FALSE;
	i->enabled = TRUE;

	i->text_offset = GUI_INPUT_DEFAULT_TEXT_OFFSET;
	i->text_max_size = GUI_INPUT_MAXSIZETEXT;

	strcpy_s(i->text_placeholder, _countof(i->text_placeholder), GUI_INPUT_DEFAULT_PLACEHOLDER);

	GUI_Input_SetNormalColor(i, GUI_INPUT_DEFAULT_BORDER_COLOR, GUI_INPUT_DEFAULT_BACKGROUND_COLOR);
	GUI_Input_SetHoverColor(i, GUI_INPUT_DEFAULT_BORDER_COLOR, GUI_INPUT_DEFAULT_BACKGROUND_HOVER_COLOR);
	GUI_Input_SetBorderSize(i, GUI_INPUT_DEFAULT_BORDER_SIZE);
	GUI_Input_SetTextColor(i, GUI_INPUT_DEFAULT_TEXT_COLOR);
	GUI_Input_SetText(i, i->text_placeholder, font_path, font_size);

	return i;

}



void GUI_Input_SetEnabled(GUI_Input i)
{

	i->enabled = TRUE;

}

void GUI_Input_SetDisabled(GUI_Input i)
{

	i->enabled = FALSE;

}

void GUI_Input_SetPosition(GUI_Input i, unsigned int x, unsigned int y)
{

	if (x == GUI_CENTERED)
		i->dimensions.x = (Sint16)(((*(i->screen))->w - i->dimensions.w) / 2);
	else
	{

		i->dimensions.x = (Sint16)(x);
		i->dimensions.x = (i->dimensions.x >= ((*(i->screen))->w - i->dimensions.w)) ? (Sint16)((*(i->screen))->w - i->dimensions.w) : i->dimensions.x;
		i->dimensions.x = (i->dimensions.x < 0) ? 0 : i->dimensions.x;

	}

	if (y == GUI_CENTERED)
		i->dimensions.y = (Sint16)(((*(i->screen))->h - i->dimensions.h) / 2);
	else
	{

		i->dimensions.y = (Sint16)(y);
		i->dimensions.y = (i->dimensions.y >= ((*(i->screen))->h - i->dimensions.h)) ? (Sint16)((*(i->screen))->h - i->dimensions.h) : i->dimensions.y;
		i->dimensions.y = (i->dimensions.y < 0) ? 0 : i->dimensions.y;

	}

	GUI_InputTextCenter(i);

}

void GUI_Input_SetBorderSize(GUI_Input i, int border_size)
{

	border_size = (border_size > GUI_GetInfValue(i->dimensions.w, i->dimensions.h) / 2) ? GUI_GetInfValue(i->dimensions.w, i->dimensions.h) / 2 : border_size;

	i->dimensions_background.w = (Uint16)(i->dimensions.w - border_size * 2);
	i->dimensions_background.h = (Uint16)(i->dimensions.h - border_size * 2);
	i->dimensions_background.x = (Sint16)(border_size);
	i->dimensions_background.y = (Sint16)(border_size);

	GUI_Input_SetNormalColor(i, i->border_color, i->content_color);
	GUI_Input_SetHoverColor(i, i->border_color_hover, i->content_color_hover);

}

void GUI_Input_SetMaxSize(GUI_Input i, unsigned int max_size)
{

	i->text_max_size = (max_size > GUI_INPUT_MAXSIZETEXT - 1) ? GUI_INPUT_MAXSIZETEXT - 1 : max_size;

	if (i->text_size > max_size)
	{
		i->text_size = max_size;
		i->text_value[i->text_max_size] = '\0';
	}

	GUI_Input_SetText(i, i->text_value, i->font_path, i->font_size);

}

void GUI_Input_SetTextColor(GUI_Input i, const char * text_color)
{

	strcpy_s(i->text_color, _countof(i->text_color), text_color);

	if (i->text != NULL)
		GUI_Input_SetText(i, i->text_value, i->font_path, i->font_size);

}

void GUI_Input_SetTextOffset(GUI_Input i, unsigned int text_offset)
{

	i->text_offset = text_offset;

	GUI_Input_SetText(i, i->text_value, i->font_path, i->font_size);

}

void GUI_Input_SetPlaceholder(GUI_Input i, const char * placeholder)
{

	strcpy_s(i->text_placeholder, _countof(i->text_placeholder), placeholder);

	if(!i->text_size)
		GUI_Input_SetText(i, i->text_placeholder, i->font_path, i->font_size);

}

void GUI_Input_SetPositionFromAnchor(GUI_Input i, unsigned int x, unsigned int y, GUI_Anchor anchor)
{

	if (anchor == TOP_LEFT || (x == GUI_CENTERED && y == GUI_CENTERED))
		GUI_Input_SetPosition(i, x, y);
	else
	{

		if (x == GUI_CENTERED)
			i->dimensions.x = (Sint16)(((*(i->screen))->w - i->dimensions.w) / 2);
		else
		{

			i->dimensions.x = (anchor == TOP_RIGHT || anchor == BOTTOM_RIGHT) ? (Sint16)((*(i->screen))->w - i->dimensions.w - x) : (Sint16)(x);
			i->dimensions.x = (i->dimensions.x >= ((*(i->screen))->w - i->dimensions.w)) ? (Sint16)((*(i->screen))->w - i->dimensions.w) : i->dimensions.x;
			i->dimensions.x = (i->dimensions.x < 0) ? 0 : i->dimensions.x;

		}

		if (y == GUI_CENTERED)
			i->dimensions.y = (Sint16)(((*(i->screen))->h - i->dimensions.h) / 2);
		else
		{

			i->dimensions.y = (anchor == BOTTOM_RIGHT || anchor == BOTTOM_LEFT) ? (Sint16)((*(i->screen))->h - i->dimensions.h - y) : (Sint16)(y);
			i->dimensions.y = (i->dimensions.y >= ((*(i->screen))->h - i->dimensions.h)) ? (Sint16)((*(i->screen))->h - i->dimensions.h) : i->dimensions.y;
			i->dimensions.y = (i->dimensions.y < 0) ? 0 : i->dimensions.y;

		}

	}

	GUI_InputTextCenter(i);

}

void GUI_Input_SetTextFont(GUI_Input i, const char * font_path, unsigned int font_size)
{

	assert(font_path != NULL);

	if (i->text != NULL)
		GUI_Input_SetText(i, i->text_value, font_path, font_size);
	else
	{

		i->font_size = font_size;
		strcpy_s(i->font_path, _countof(i->font_path), font_path);

	}

}

void GUI_Input_SetHoverColor(GUI_Input i, const char * border_color, const char * background_color)
{

	assert(border_color != NULL);
	assert(background_color != NULL);

	strcpy_s(i->border_color_hover, _countof(i->border_color_hover), border_color);
	strcpy_s(i->content_color_hover, _countof(i->content_color_hover), background_color);

	SDL_FillRect(i->color_hover, NULL, SDL_MapRGB((*(i->screen))->format, GUI_GetRedFromHex(border_color), GUI_GetGreenFromHex(border_color), GUI_GetBlueFromHex(border_color)));
	SDL_FillRect(i->color_hover, &(i->dimensions_background), SDL_MapRGB((*(i->screen))->format, GUI_GetRedFromHex(background_color), GUI_GetGreenFromHex(background_color), GUI_GetBlueFromHex(background_color)));

}

void GUI_Input_SetNormalColor(GUI_Input i, const char * border_color, const char * background_color)
{

	assert(border_color != NULL);
	assert(background_color != NULL);

	if (i->color_normal != NULL)
		SDL_FreeSurface(i->color_normal);

	if (i->color_hover != NULL)
		SDL_FreeSurface(i->color_hover);

	i->color_normal = SDL_CreateRGBSurface(SDL_HWSURFACE, i->dimensions.w, i->dimensions.h, 32, 0, 0, 0, 0);
	assert(i->color_normal != NULL);

	i->color_hover = SDL_CreateRGBSurface(SDL_HWSURFACE, i->dimensions.w, i->dimensions.h, 32, 0, 0, 0, 0);
	assert(i->color_hover != NULL);

	SDL_FillRect(i->color_normal, NULL, SDL_MapRGB((*(i->screen))->format, GUI_GetRedFromHex(border_color), GUI_GetGreenFromHex(border_color), GUI_GetBlueFromHex(border_color)));
	SDL_FillRect(i->color_normal, &(i->dimensions_background), SDL_MapRGB((*(i->screen))->format, GUI_GetRedFromHex(background_color), GUI_GetGreenFromHex(background_color), GUI_GetBlueFromHex(background_color)));

	strcpy_s(i->border_color, _countof(i->border_color), border_color);
	strcpy_s(i->content_color, _countof(i->content_color), background_color);

	if (i->color_hover == NULL)
		GUI_Input_SetHoverColor(i, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_HOVER_COLOR);

}

void GUI_Input_SetText(GUI_Input i, const char * text, const char * font_path, unsigned int font_size)
{

	assert(text != NULL);
	assert(font_path != NULL);

	if (i->text != NULL)
	{
		SDL_FreeSurface(i->text);
		i->text = NULL;
	}

	TTF_Font * font = TTF_OpenFont(font_path, font_size);
	assert(font != NULL);

	i->font_size = font_size;
	i->text = TTF_RenderText_Blended(font, text, GUI_GetColorFromHex(i->text_color));

	TTF_CloseFont(font);

	GUI_InputTextCenter(i);
	GUI_AdaptTextSize(i);

	strcpy_s(i->text_value, _countof(i->text_value), text);
	strcpy_s(i->font_path, _countof(i->font_path), font_path);

}



const char * GUI_InputGetValue(GUI_Input i)
{

	return i->text_value;

}

unsigned int GUI_Input_GetWidth(GUI_Input i)
{

	return i->dimensions.w;

}

unsigned int GUI_Input_GetHeight(GUI_Input i)
{

	return i->dimensions.h;

}

unsigned int GUI_Input_GetMaxSize(GUI_Input i)
{

	return i->text_max_size;

}

unsigned int GUI_Input_GetPositionX(GUI_Input i)
{

	return i->dimensions.x;

}

unsigned int GUI_Input_GetPositionY(GUI_Input i)
{

	return i->dimensions.y;

}



GUI_Bool GUI_Input_IsEmpty(GUI_Input i)
{

	return !i->text_size;

}

GUI_Bool GUI_Input_IsEnabled(GUI_Input i)
{

	return i->enabled;

}

GUI_Bool GUI_Input_IsHovered(GUI_Input i)
{

	if (!i->enabled)
		return FALSE;

	if (i->event->type == SDL_MOUSEMOTION)
	{

		int x = i->event->motion.x;
		int y = i->event->motion.y;

		if (x >= i->dimensions.x && x <= (i->dimensions.x + i->dimensions.w) &&
			y >= i->dimensions.y && y <= (i->dimensions.y + i->dimensions.h))
		{

			if (!i->hover)
				i->hover = TRUE;

		}
		else
		{

			if (i->hover)
				i->hover = FALSE;

		}

	}

	return i->hover;

}

GUI_Bool GUI_Input_IsClicked(GUI_Input i)
{

	return (GUI_Input_IsHovered(i) && i->event->type == SDL_MOUSEBUTTONUP && i->event->button.button == SDL_BUTTON_LEFT);

}

GUI_Bool GUI_Input_IsRightClicked(GUI_Input i)
{

	return (GUI_Input_IsHovered(i) && i->event->type == SDL_MOUSEBUTTONUP && i->event->button.button == SDL_BUTTON_RIGHT);

}



void GUI_Input_HandleInput(GUI_Input i)
{

	if (!i->enabled)
		return;

	if (GUI_Input_IsClicked(i))
		i->focus = TRUE;
	else if (!GUI_Input_IsHovered(i) && i->event->type == SDL_MOUSEBUTTONDOWN && i->event->button.button == SDL_BUTTON_LEFT)
		i->focus = FALSE;

	if (i->focus)
	{

		if (i->event->type == SDL_KEYDOWN)
		{

			if (GUI_IsCharValid(i->event->key.keysym.unicode))
				GUI_AddLetter(i, i->event->key.keysym.unicode);
			else if (i->event->key.keysym.unicode == SDLK_BACKSPACE)
				GUI_DelLetter(i);

			GUI_Input_SetText(i, i->text_value, i->font_path, i->font_size);

		}

	}

}



void GUI_Input_Display(GUI_Input i)
{

	if (!i->enabled)
		return;

	if (i->hover == TRUE || i->focus == TRUE)
		SDL_BlitSurface(i->color_hover, NULL, (*(i->screen)), &(i->dimensions));
	else
		SDL_BlitSurface(i->color_normal, NULL, (*(i->screen)), &(i->dimensions));

	if (i->text != NULL)
		SDL_BlitSurface(i->text, &(i->text_dimensions), (*(i->screen)), &(i->position_text));

}



void GUI_Input_Free(GUI_Input * i)
{

	assert((*i) != NULL);

	if ((*i)->color_normal != NULL)
		SDL_FreeSurface((*i)->color_normal);

	if ((*i)->color_hover != NULL)
		SDL_FreeSurface((*i)->color_hover);

	if ((*i)->text)
		SDL_FreeSurface((*i)->text);

	free(*i);
	*i = NULL;

}
