#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

#include "progress_bar.h"

struct s_ProgressBar {

	SDL_Surface ** screen;
	SDL_Event * event;

	SDL_Surface * progress_bar;

	SDL_Rect dimensions;
	SDL_Rect dimensions_background;
	SDL_Rect dimensions_content;

	char border_color[7];
	char content_color[7];
	char background_color[7];

	int value;
	int min_value;
	int max_value;

	GUI_Bool hover;
	GUI_Bool enabled;

};



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



GUI_ProgressBar GUI_ProgressBar_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_Bool enabled)
{

	GUI_ProgressBar pb = (GUI_ProgressBar)malloc(sizeof(struct s_ProgressBar));
	assert(pb != NULL);

	assert(screen != NULL);
	pb->screen = screen;

	assert(event != NULL);
	pb->event = event;

	pb->dimensions.x = 0;
	pb->dimensions.y = 0;
	pb->dimensions.w = (Uint16)(width);
	pb->dimensions.h = (Uint16)(height);

	if (pb->dimensions.w > (*screen)->w)
		pb->dimensions.w = (Uint16)((*screen)->w);

	if (pb->dimensions.h > (*screen)->h)
		pb->dimensions.h = (Uint16)((*screen)->h);

	pb->enabled = enabled;
	pb->hover = FALSE;

	pb->value = GUI_PROGRESSBAR_DEFAULT_MINVALUE;
	pb->min_value = GUI_PROGRESSBAR_DEFAULT_MINVALUE;
	pb->max_value = GUI_PROGRESSBAR_DEFAULT_MAXVALUE;
	pb->dimensions_content.w = 0;

	pb->progress_bar = NULL;

	GUI_ProgressBar_SetBorderSize(pb, GUI_PROGRESSBAR_DEFAULT_BORDER_SIZE);
	GUI_ProgressBar_SetColor(pb, GUI_PROGRESSBAR_DEFAULT_BORDER_COLOR, GUI_PROGRESSBAR_DEFAULT_BACKGROUND_COLOR, GUI_PROGRESSBAR_DEFAULT_CONTENT_COLOR);

	return pb;

}



void GUI_ProgressBar_Increase(GUI_ProgressBar pb)
{

	GUI_ProgressBar_SetValue(pb, GUI_ProgressBar_GetValue(pb) + 1);

}

void GUI_ProgressBar_Decrease(GUI_ProgressBar pb)
{

	GUI_ProgressBar_SetValue(pb, GUI_ProgressBar_GetValue(pb) - 1);

}

void GUI_ProgressBar_SetEnabled(GUI_ProgressBar pb)
{

	pb->enabled = TRUE;

}

void GUI_ProgressBar_SetDisabled(GUI_ProgressBar pb)
{

	pb->enabled = FALSE;

}

void GUI_ProgressBar_SetValue(GUI_ProgressBar pb, int value)
{

	pb->value = value;

	if (pb->value > pb->max_value)
		pb->value = pb->max_value;

	if (pb->value < pb->min_value)
		pb->value = pb->min_value;

	pb->dimensions_content.w = (Uint16)(pb->value * pb->dimensions_background.w / (pb->max_value - pb->min_value));

	GUI_ProgressBar_SetColor(pb, pb->border_color, pb->background_color, pb->content_color);

}

void GUI_ProgressBar_SetPosition(GUI_ProgressBar pb, int x, int y)
{

	pb->dimensions.x = (x == GUI_CENTERED) ? (Sint16)(((*(pb->screen))->w - pb->dimensions.w) / 2) : (Sint16)(x);
	pb->dimensions.y = (y == GUI_CENTERED) ? (Sint16)(((*(pb->screen))->h - pb->dimensions.h) / 2) : (Sint16)(y);

	pb->dimensions.x = (pb->dimensions.x >= ((*(pb->screen))->w - pb->dimensions.w)) ? (Sint16)((*(pb->screen))->w - pb->dimensions.w) : pb->dimensions.x;
	pb->dimensions.x = (pb->dimensions.x < 0) ? 0 : pb->dimensions.x;

	pb->dimensions.y = (pb->dimensions.y >= ((*(pb->screen))->h - pb->dimensions.h)) ? (Sint16)((*(pb->screen))->h - pb->dimensions.h) : pb->dimensions.y;
	pb->dimensions.y = (pb->dimensions.y < 0) ? 0 : pb->dimensions.y;

}

void GUI_ProgressBar_SetMaxValue(GUI_ProgressBar pb, int max_value)
{

	if (max_value > pb->min_value)
		pb->max_value = max_value;

	GUI_ProgressBar_SetValue(pb, pb->value);

}

void GUI_ProgressBar_SetMinValue(GUI_ProgressBar pb, int min_value)
{

	if (min_value < pb->max_value)
		pb->min_value = min_value;

	GUI_ProgressBar_SetValue(pb, pb->value);

}

void GUI_ProgressBar_SetBorderSize(GUI_ProgressBar pb, unsigned int border_size)
{

	pb->dimensions_background.w = (Uint16)(pb->dimensions.w - 2 * border_size);
	pb->dimensions_background.h = (Uint16)(pb->dimensions.h - 2 * border_size);
	pb->dimensions_background.x = (Sint16)(border_size);
	pb->dimensions_background.y = (Sint16)(border_size);

	pb->dimensions_content.h = pb->dimensions_background.h;
	pb->dimensions_content.x = pb->dimensions_background.x;
	pb->dimensions_content.y = pb->dimensions_background.y;

	GUI_ProgressBar_SetValue(pb, pb->value);

}

void GUI_ProgressBar_SetPositionFromAnchor(GUI_ProgressBar pb, int x, int y, GUI_Anchor anchor)
{

	if (anchor == TOP_LEFT || (x == GUI_CENTERED && y == GUI_CENTERED))
		GUI_ProgressBar_SetPosition(pb, GUI_CENTERED, GUI_CENTERED);
	else
	{

		if (x == GUI_CENTERED)
			pb->dimensions.x = (Sint16)(((*(pb->screen))->w - pb->dimensions.w) / 2);
		else
		{

			pb->dimensions.x = (anchor == TOP_RIGHT || anchor == BOTTOM_RIGHT) ? (Sint16)((*(pb->screen))->w - pb->dimensions.w - x) : (Sint16)(x);
			pb->dimensions.x = (pb->dimensions.x >= ((*(pb->screen))->w - pb->dimensions.w)) ? (Sint16)((*(pb->screen))->w - pb->dimensions.w) : pb->dimensions.x;
			pb->dimensions.x = (pb->dimensions.x < 0) ? 0 : pb->dimensions.x;

		}

		if (y == GUI_CENTERED)
			pb->dimensions.y = (Sint16)(((*(pb->screen))->h - pb->dimensions.h) / 2);
		else
		{

			pb->dimensions.y = (anchor == BOTTOM_RIGHT || anchor == BOTTOM_LEFT) ? (Sint16)((*(pb->screen))->h - pb->dimensions.h - y) : (Sint16)(y);
			pb->dimensions.y = (pb->dimensions.y >= ((*(pb->screen))->h - pb->dimensions.h)) ? (Sint16)((*(pb->screen))->h - pb->dimensions.h) : pb->dimensions.y;
			pb->dimensions.y = (pb->dimensions.y < 0) ? 0 : pb->dimensions.y;

		}

	}

}

void GUI_ProgressBar_SetColor(GUI_ProgressBar pb, const char * border_color, const char * background_color, const char * content_color)
{

	if (pb->progress_bar == NULL)
	{
		pb->progress_bar = SDL_CreateRGBSurface(SDL_HWSURFACE, pb->dimensions.w, pb->dimensions.h, 32, 0, 0, 0, 0);
		assert(pb->progress_bar != NULL);
	}

	SDL_FillRect(pb->progress_bar, NULL, SDL_MapRGB((*(pb->screen))->format, GUI_GetRedFromHex(border_color), GUI_GetGreenFromHex(border_color), GUI_GetBlueFromHex(border_color)));
	SDL_FillRect(pb->progress_bar, &(pb->dimensions_background), SDL_MapRGB((*(pb->screen))->format, GUI_GetRedFromHex(background_color), GUI_GetGreenFromHex(background_color), GUI_GetBlueFromHex(background_color)));
	SDL_FillRect(pb->progress_bar, &(pb->dimensions_content), SDL_MapRGB((*(pb->screen))->format, GUI_GetRedFromHex(content_color), GUI_GetGreenFromHex(content_color), GUI_GetBlueFromHex(content_color)));

	strcpy_s(pb->border_color, _countof(pb->border_color), border_color);
	strcpy_s(pb->background_color, _countof(pb->background_color), background_color);
	strcpy_s(pb->content_color, _countof(pb->content_color), content_color);

}



int GUI_ProgressBar_GetValue(GUI_ProgressBar pb)
{

	return pb->value;

}

int GUI_ProgressBar_GetMaxValue(GUI_ProgressBar pb)
{

	return pb->max_value;

}

int GUI_ProgressBar_GetMinValue(GUI_ProgressBar pb)
{

	return pb->min_value;

}

unsigned int GUI_ProgressBar_GetWidth(GUI_ProgressBar pb)
{

	return pb->dimensions.w;

}

unsigned int GUI_ProgressBar_GetHeight(GUI_ProgressBar pb)
{

	return pb->dimensions.h;

}

unsigned int GUI_ProgressBar_GetPositionX(GUI_ProgressBar pb)
{

	return pb->dimensions.x;

}

unsigned int GUI_ProgressBar_GetPositionY(GUI_ProgressBar pb)
{

	return pb->dimensions.y;

}



GUI_Bool GUI_ProgressBar_IsFull(GUI_ProgressBar pb)
{

	return (pb->value == pb->max_value);

}

GUI_Bool GUI_ProgressBar_IsEmpty(GUI_ProgressBar pb)
{

	return (pb->value == pb->min_value);

}

GUI_Bool GUI_ProgressBar_IsEnabled(GUI_ProgressBar pb)
{

	return pb->enabled;

}

GUI_Bool GUI_ProgressBar_IsHovered(GUI_ProgressBar pb)
{

	if (!pb->enabled)
		return FALSE;

	if (pb->event->type == SDL_MOUSEMOTION)
	{

		int x = pb->event->motion.x;
		int y = pb->event->motion.y;

		if (x >= pb->dimensions.x && x <= (pb->dimensions.x + pb->dimensions.w) &&
			y >= pb->dimensions.y && y <= (pb->dimensions.y + pb->dimensions.h))
		{

			if (!pb->hover)
				pb->hover = TRUE;

		}
		else
		{

			if (pb->hover)
				pb->hover = FALSE;

		}

	}

	return pb->hover;

}

GUI_Bool GUI_ProgressBar_IsClicked(GUI_ProgressBar pb)
{

	return (GUI_ProgressBar_IsHovered(pb) && pb->event->type == SDL_MOUSEBUTTONUP && pb->event->button.button == SDL_BUTTON_LEFT);

}

GUI_Bool GUI_ProgressBar_IsRightClicked(GUI_ProgressBar pb)
{

	return (GUI_ProgressBar_IsHovered(pb) && pb->event->type == SDL_MOUSEBUTTONUP && pb->event->button.button == SDL_BUTTON_RIGHT);

}



void GUI_ProgressBar_Display(GUI_ProgressBar pb)
{

	if (!pb->enabled)
		return;

	SDL_BlitSurface(pb->progress_bar, NULL, *(pb->screen), &(pb->dimensions));

}



void GUI_ProgressBar_Free(GUI_ProgressBar * pb)
{

	assert((*pb) != NULL);

	SDL_FreeSurface((*pb)->progress_bar);

	free(*pb);
	*pb = NULL;

}
