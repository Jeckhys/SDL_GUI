#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "button.h"

struct s_Button {

	SDL_Surface ** screen;
	SDL_Event * event;

	SDL_Surface * color_normal;
	SDL_Surface * color_hover;
	SDL_Surface * image_normal;
	SDL_Surface * image_hover;
	SDL_Surface * text;

	SDL_Rect dimensions;
	SDL_Rect dimensions_content;
	SDL_Rect position_text;

	unsigned int font_size;

	char border_color[7];
	char border_color_hover[7];
	char content_color[7];
	char content_color_hover[7];
	char text_color[7];

	char text_value[GUI_BUTTON_MAXSIZETEXT];
	char font_path[GUI_BUTTON_MAXSIZETEXT];

	GUI_Bool hover;
	GUI_Bool enabled;

};



static int GUI_GetInfValue(int a, int b)
{

	return (a > b) ? b : a;

}

static void GUI_ButtonTextCenter(GUI_Button b)
{

	if (b->text != NULL)
	{

		b->position_text.x = (Sint16)(b->dimensions.x + (b->dimensions.w - b->text->w) / 2);
		b->position_text.y = (Sint16)(b->dimensions.y + (b->dimensions.h - b->text->h) / 2);

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



GUI_Button GUI_Button_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int width, unsigned int height, GUI_Bool enabled)
{

	GUI_Button b = (GUI_Button)malloc(sizeof(struct s_Button));
	assert(b != NULL);

	assert(screen != NULL);
	b->screen = screen;
	assert(event != NULL);
	b->event = event;

	b->dimensions.w = (Uint16)(width);
	b->dimensions.h = (Uint16)(height);

	if (b->dimensions.w > (*screen)->w)
		b->dimensions.w = (Uint16)((*screen)->w);

	if (b->dimensions.h > (*screen)->h)
		b->dimensions.h = (Uint16)((*screen)->h);

	b->image_normal = NULL;
	b->image_hover = NULL;
	b->text = NULL;

	b->hover = FALSE;

	b->color_normal = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0);
	assert(b->color_normal != NULL);
	b->color_hover = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0);
	assert(b->color_hover != NULL);

	GUI_Button_SetNormalColor(b, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_COLOR);
	GUI_Button_SetHoverColor(b, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_HOVER_COLOR);

	GUI_Button_SetBorderSize(b, GUI_BUTTON_DEFAULT_BORDER_SIZE);
	GUI_Button_SetPosition(b, 0, 0);

	strcpy_s(b->text_color, _countof(b->text_color), GUI_BUTTON_DEFAULT_TEXT_COLOR);

	b->enabled = enabled;

	return b;

}



void GUI_Button_SetEnabled(GUI_Button b)
{

	b->enabled = TRUE;

}

void GUI_Button_SetDisabled(GUI_Button b)
{

	b->enabled = FALSE;

}

void GUI_Button_SetDefaultStyle(GUI_Button b)
{

	GUI_Button_SetBorderSize(b, GUI_BUTTON_DEFAULT_BORDER_SIZE);
	GUI_Button_SetNormalColor(b, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_COLOR);
	GUI_Button_SetHoverColor(b, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_HOVER_COLOR);
	GUI_Button_SetTextColor(b, GUI_BUTTON_DEFAULT_TEXT_COLOR);

}

void GUI_Button_SetPosition(GUI_Button b, int x, int y)
{

	if (x == GUI_CENTERED)
		b->dimensions.x = (Sint16)(((*(b->screen))->w - b->dimensions.w) / 2);
	else
	{

		b->dimensions.x = (Sint16)(x);
		b->dimensions.x = (b->dimensions.x >= ((*(b->screen))->w - b->dimensions.w)) ? (Sint16)((*(b->screen))->w - b->dimensions.w) : b->dimensions.x;
		b->dimensions.x = (b->dimensions.x < 0) ? 0 : b->dimensions.x;

	}

	if (y == GUI_CENTERED)
		b->dimensions.y = (Sint16)(((*(b->screen))->h - b->dimensions.h) / 2);
	else
	{

		b->dimensions.y = (Sint16)(y);
		b->dimensions.y = (b->dimensions.y >= ((*(b->screen))->h - b->dimensions.h)) ? (Sint16)((*(b->screen))->h - b->dimensions.h) : b->dimensions.y;
		b->dimensions.y = (b->dimensions.y < 0) ? 0 : b->dimensions.y;

	}

	GUI_ButtonTextCenter(b);

}

void GUI_Button_SetBorderSize(GUI_Button b, int border_size)
{

	border_size = (border_size > GUI_GetInfValue(b->dimensions.w, b->dimensions.h) / 2) ? GUI_GetInfValue(b->dimensions.w, b->dimensions.h) / 2 : border_size;

	b->dimensions_content.w = (Uint16)(b->dimensions.w - border_size * 2);
	b->dimensions_content.h = (Uint16)(b->dimensions.h - border_size * 2);
	b->dimensions_content.x = (Sint16)(border_size);
	b->dimensions_content.y = (Sint16)(border_size);

	GUI_Button_SetNormalColor(b, b->border_color, b->content_color);
	GUI_Button_SetHoverColor(b, b->border_color_hover, b->content_color_hover);

}

void GUI_Button_SetTextColor(GUI_Button b, const char * text_color)
{

	strcpy_s(b->text_color, _countof(b->text_color), text_color);

	if(b->text != NULL)
		GUI_Button_SetText(b, b->text_value, b->font_path, b->font_size);

}

void GUI_Button_SetPositionFromAnchor(GUI_Button b, int x, int y, GUI_Anchor anchor)
{

	if (anchor == TOP_LEFT || (x == GUI_CENTERED && y == GUI_CENTERED))
		GUI_Button_SetPosition(b, x, y);
	else
	{

		if (x == GUI_CENTERED)
			b->dimensions.x = (Sint16)(((*(b->screen))->w - b->dimensions.w) / 2);
		else
		{

			b->dimensions.x = (anchor == TOP_RIGHT || anchor == BOTTOM_RIGHT) ? (Sint16)((*(b->screen))->w - b->dimensions.w - x) : (Sint16)(x);
			b->dimensions.x = (b->dimensions.x >= ((*(b->screen))->w - b->dimensions.w)) ? (Sint16)((*(b->screen))->w - b->dimensions.w) : b->dimensions.x;
			b->dimensions.x = (b->dimensions.x < 0) ? 0 : b->dimensions.x;

		}

		if (y == GUI_CENTERED)
			b->dimensions.y = (Sint16)(((*(b->screen))->h - b->dimensions.h) / 2);
		else
		{

			b->dimensions.y = (anchor == BOTTOM_RIGHT || anchor == BOTTOM_LEFT) ? (Sint16)((*(b->screen))->h - b->dimensions.h - y) : (Sint16)(y);
			b->dimensions.y = (b->dimensions.y >= ((*(b->screen))->h - b->dimensions.h)) ? (Sint16)((*(b->screen))->h - b->dimensions.h) : b->dimensions.y;
			b->dimensions.y = (b->dimensions.y < 0) ? 0 : b->dimensions.y;

		}

	}

	GUI_ButtonTextCenter(b);

}

void GUI_Button_SetImage(GUI_Button b, const char * image_path, const char * hover_path)
{

	assert(image_path != NULL);
	assert(hover_path != NULL);

	if (b->color_normal)
	{
		SDL_FreeSurface(b->color_normal);
		b->color_normal = NULL;
	}

	if (b->color_hover)
	{
		SDL_FreeSurface(b->color_hover);
		b->color_hover = NULL;
	}

	if (b->image_normal != NULL)
		SDL_FreeSurface(b->image_normal);

	if (b->image_hover != NULL)
		SDL_FreeSurface(b->image_hover);

	b->image_normal = IMG_Load(image_path);
	assert(b->image_normal != NULL);

	b->image_hover = IMG_Load(hover_path);
	assert(b->image_hover != NULL);

}

void GUI_Button_SetTextFont(GUI_Button b, const char * font_path, unsigned int font_size)
{

	assert(font_path != NULL);

	if (b->text != NULL)
		GUI_Button_SetText(b, b->text_value, font_path, font_size);
	else
	{

		b->font_size = font_size;
		strcpy_s(b->font_path, _countof(b->font_path), font_path);

	}

}

void GUI_Button_SetHoverColor(GUI_Button b, const char * border_color, const char * button_color)
{

	assert(border_color != NULL);
	assert(button_color != NULL);

	strcpy_s(b->border_color_hover, _countof(b->border_color_hover), border_color);
	strcpy_s(b->content_color_hover, _countof(b->content_color_hover), button_color);

	SDL_FillRect(b->color_hover, NULL, SDL_MapRGB((*(b->screen))->format, GUI_GetRedFromHex(border_color), GUI_GetGreenFromHex(border_color), GUI_GetBlueFromHex(border_color)));
	SDL_FillRect(b->color_hover, &(b->dimensions_content), SDL_MapRGB((*(b->screen))->format, GUI_GetRedFromHex(button_color), GUI_GetGreenFromHex(button_color), GUI_GetBlueFromHex(button_color)));

}

void GUI_Button_SetNormalColor(GUI_Button b, const char * border_color, const char * button_color)
{

	assert(border_color != NULL);
	assert(button_color != NULL);

	if (b->image_normal != NULL)
	{
		SDL_FreeSurface(b->image_normal);
		b->image_normal = NULL;
	}

	if (b->image_hover != NULL)
	{
		SDL_FreeSurface(b->image_hover);
		b->image_hover = NULL;
	}

	if (b->color_normal != NULL)
		SDL_FreeSurface(b->color_normal);

	if (b->color_hover != NULL)
		SDL_FreeSurface(b->color_hover);

	b->color_normal = SDL_CreateRGBSurface(SDL_HWSURFACE, b->dimensions.w, b->dimensions.h, 32, 0, 0, 0, 0);
	assert(b->color_normal != NULL);

	b->color_hover = SDL_CreateRGBSurface(SDL_HWSURFACE, b->dimensions.w, b->dimensions.h, 32, 0, 0, 0, 0);
	assert(b->color_hover != NULL);

	SDL_FillRect(b->color_normal, NULL, SDL_MapRGB((*(b->screen))->format, GUI_GetRedFromHex(border_color), GUI_GetGreenFromHex(border_color), GUI_GetBlueFromHex(border_color)));
	SDL_FillRect(b->color_normal, &(b->dimensions_content), SDL_MapRGB((*(b->screen))->format, GUI_GetRedFromHex(button_color), GUI_GetGreenFromHex(button_color), GUI_GetBlueFromHex(button_color)));

	strcpy_s(b->border_color, _countof(b->border_color), border_color);
	strcpy_s(b->content_color, _countof(b->content_color), button_color);

	if (b->color_hover == NULL)
		GUI_Button_SetHoverColor(b, GUI_BUTTON_DEFAULT_BORDER_COLOR, GUI_BUTTON_DEFAULT_CONTENT_HOVER_COLOR);

}

void GUI_Button_SetText(GUI_Button b, const char * text, const char * font_path, unsigned int font_size)
{

	assert(text != NULL);
	assert(font_path != NULL);

	if (b->text != NULL)
	{
		SDL_FreeSurface(b->text);
		b->text = NULL;
	}

	TTF_Font * font = TTF_OpenFont(font_path, font_size);
	assert(font != NULL);

	b->font_size = font_size;
	b->text = TTF_RenderText_Blended(font, text, GUI_GetColorFromHex(b->text_color));

	TTF_CloseFont(font);

	GUI_ButtonTextCenter(b);

	strcpy_s(b->text_value, _countof(b->text_value), text);
	strcpy_s(b->font_path, _countof(b->font_path), font_path);

}

void GUI_Button_SetPositionRelativeTo(GUI_Button b, GUI_Button ref, int offset_x, int offset_y, GUI_Anchor anchor)
{

	b->dimensions.x = (anchor == TOP_LEFT || anchor == BOTTOM_LEFT) ? ref->dimensions.x - b->dimensions.w : ref->dimensions.x + ref->dimensions.w;
	b->dimensions.y = (anchor == TOP_LEFT || anchor == TOP_RIGHT) ? ref->dimensions.y - b->dimensions.h : ref->dimensions.y + ref->dimensions.h;

	b->dimensions.x += (Sint16)(offset_x);
	b->dimensions.x = (b->dimensions.x >= ((*(b->screen))->w - b->dimensions.w)) ? (Sint16)((*(b->screen))->w - b->dimensions.w) : b->dimensions.x;
	b->dimensions.x = (b->dimensions.x < 0) ? 0 : b->dimensions.x;

	b->dimensions.y += (Sint16)(offset_y);
	b->dimensions.y = (b->dimensions.y >= ((*(b->screen))->h - b->dimensions.h)) ? (Sint16)((*(b->screen))->h - b->dimensions.h) : b->dimensions.y;
	b->dimensions.y = (b->dimensions.y < 0) ? 0 : b->dimensions.y;

	GUI_ButtonTextCenter(b);

}



unsigned int GUI_Button_GetWidth(GUI_Button b)
{

	return b->dimensions.w;

}

unsigned int GUI_Button_GetHeight(GUI_Button b)
{

	return b->dimensions.h;

}

unsigned int GUI_Button_GetPositionX(GUI_Button b)
{

	return b->dimensions.x;

}

unsigned int GUI_Button_GetPositionY(GUI_Button b)
{

	return b->dimensions.y;

}



GUI_Bool GUI_Button_IsHovered(GUI_Button b)
{

	if (!b->enabled)
		return FALSE;

	if (b->event->type == SDL_MOUSEMOTION)
	{

		int x = b->event->motion.x;
		int y = b->event->motion.y;

		if (x >= b->dimensions.x && x <= (b->dimensions.x + b->dimensions.w) &&
			y >= b->dimensions.y && y <= (b->dimensions.y + b->dimensions.h))
		{

			if (!b->hover)
				b->hover = TRUE;

		}
		else
		{

			if (b->hover)
				b->hover = FALSE;

		}

	}

	return b->hover;

}

GUI_Bool GUI_Button_IsClicked(GUI_Button b)
{

	return (GUI_Button_IsHovered(b) && b->event->type == SDL_MOUSEBUTTONUP && b->event->button.button == SDL_BUTTON_LEFT);

}

GUI_Bool GUI_Button_IsEnabled(GUI_Button b)
{

	return b->enabled;

}

GUI_Bool GUI_Button_IsRightClicked(GUI_Button b)
{

	return (GUI_Button_IsHovered(b) && b->event->type == SDL_MOUSEBUTTONUP && b->event->button.button == SDL_BUTTON_RIGHT);

}



void GUI_Button_Display(GUI_Button b)
{

	if (!b->enabled)
		return;

	/* Button is designed with an image */
	if (b->image_normal)
	{

		if (b->hover)
			SDL_BlitSurface(b->image_hover, NULL, *(b->screen), &(b->dimensions));
		else
			SDL_BlitSurface(b->image_normal, NULL, *(b->screen), &(b->dimensions));

	}
	/* Button is designed with surfaces */
	else
	{

		if (b->hover)
			SDL_BlitSurface(b->color_hover, NULL, *(b->screen), &(b->dimensions));
		else
			SDL_BlitSurface(b->color_normal, NULL, *(b->screen), &(b->dimensions));

	}

	if (b->text)
		SDL_BlitSurface(b->text, NULL, *(b->screen), &(b->position_text));

}



void GUI_Button_Free(GUI_Button * b)
{

	assert((*b) != NULL);

	if ((*b)->color_normal != NULL)
		SDL_FreeSurface((*b)->color_normal);

	if ((*b)->color_hover != NULL)
		SDL_FreeSurface((*b)->color_hover);

	if ((*b)->image_normal != NULL)
		SDL_FreeSurface((*b)->image_normal);

	if ((*b)->image_hover != NULL)
		SDL_FreeSurface((*b)->image_hover);

	if ((*b)->text != NULL)
		SDL_FreeSurface((*b)->text);

	free(*b);
	*b = NULL;

}
