#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

#include "button.h"
#include "menu.h"

struct s_Menu_Elem {

	int numero;

	GUI_Button button;
	
	struct s_Menu_Elem * next;

};



static struct s_Menu_Elem * GUI_MenuElem_Init(GUI_Button b, int numero)
{

	struct s_Menu_Elem * me = (struct s_Menu_Elem *)malloc(sizeof(struct s_Menu_Elem));
	assert(me != NULL);

	me->numero = numero;
	me->button = b;
	me->next = NULL;

	return me;

}

static void GUI_MenuElem_Free(struct s_Menu_Elem * me)
{

	if (me == NULL)
		return;

	GUI_MenuElem_Free(me->next);
	GUI_Button_Free(&(me->button));
	free(me);

}



struct s_Menu {

	struct s_Menu_Elem * head;

	SDL_Surface ** screen;
	SDL_Event * event;

	SDL_Surface * menu;

	SDL_Rect dimensions;
	SDL_Rect dimensions_background;

	char border_color[7];
	char background_color[7];

	GUI_Bool hover;
	GUI_Bool enabled;

	unsigned int number_buttons;

	unsigned int buttons_width;
	unsigned int buttons_height;

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

static void GUI_UpdateButtonsPositions(GUI_Menu m) {
	
	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		int button_position_x = GUI_Menu_GetPositionX(m) + GUI_Menu_GetBorderSize(m);
		int button_position_y = GUI_Menu_GetPositionY(m) + GUI_Menu_GetBorderSize(m) + (current->numero - 1) * (1 + m->buttons_height);

		GUI_Button_SetPosition(current->button, button_position_x, button_position_y);
		current = current->next;

	}

}



GUI_Menu GUI_Menu_Init(SDL_Surface ** screen, SDL_Event * event, unsigned int buttons_width, unsigned int buttons_height, GUI_Bool enabled)
{

	GUI_Menu m = (GUI_Menu)malloc(sizeof(struct s_Menu));
	assert(m != NULL);

	m->head = NULL;
	m->menu = NULL;

	assert(screen != NULL);
	m->screen = screen;

	assert(event != NULL);
	m->event = event;

	m->dimensions.w = 2 * GUI_MENU_DEFAULT_BORDER_SIZE;
	m->dimensions.h = 2 * GUI_MENU_DEFAULT_BORDER_SIZE;

	m->dimensions_background.x = 0;
	m->dimensions_background.y = 0;

	GUI_Menu_SetColor(m, GUI_MENU_DEFAULT_BORDER_COLOR, GUI_MENU_DEFAULT_BACKGROUND_COLOR);
	GUI_Menu_SetPosition(m, 0, 0);
	GUI_Menu_SetBorderSize(m, GUI_MENU_DEFAULT_BORDER_SIZE);

	m->hover = FALSE;
	m->enabled = enabled;

	m->number_buttons = 0;

	m->buttons_width = buttons_width;
	m->buttons_height = buttons_height;

	return m;

}



void GUI_Menu_CustomizeButtonsTextColor(GUI_Menu m, const char * text_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		GUI_Button_SetTextColor(current->button, text_color);
		current = current->next;

	}

}

void GUI_Menu_CustomizeButtonTextColor(GUI_Menu m, int id, const char * text_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != id)
		current = current->next;

	if (current == NULL)
		return;

	GUI_Button_SetTextColor(current->button, text_color);

}

void GUI_Menu_CustomizeButtons(GUI_Menu m, const char * border_color, const char * content_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		GUI_Button_SetNormalColor(current->button, border_color, content_color);
		current = current->next;

	}

}

void GUI_Menu_CustomizeButtonsTextFont(GUI_Menu m, const char * font_path, unsigned int font_size)
{

	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		GUI_Button_SetTextFont(current->button, font_path, font_size);
		current = current->next;

	}

}

void GUI_Menu_CustomizeButtonsHover(GUI_Menu m, const char * border_color, const char * content_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		GUI_Button_SetHoverColor(current->button, border_color, content_color);
		current = current->next;

	}

}

int GUI_Menu_CreateButton(GUI_Menu m, const char * text, const char * font_path, unsigned int font_size)
{

	GUI_Button b = GUI_Button_Init(m->screen, m->event, m->buttons_width, m->buttons_height, TRUE);

	GUI_Button_SetBorderSize(b, 0);
	GUI_Button_SetText(b, text, font_path, font_size);

	struct s_Menu_Elem * new = GUI_MenuElem_Init(b, ++m->number_buttons);
	struct s_Menu_Elem * current = m->head;

	if (m->head == NULL)
		m->head = new;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}

	GUI_UpdateButtonsPositions(m);

	if (m->dimensions.w < (m->buttons_width + 2 * GUI_Menu_GetBorderSize(m)))
		GUI_Menu_Resize(m, m->buttons_width + 2 * GUI_Menu_GetBorderSize(m), m->dimensions.h + m->buttons_height);
	else
		GUI_Menu_Resize(m, m->dimensions.w, 2 * GUI_Menu_GetBorderSize(m) + m->number_buttons * (1 + m->buttons_height) - 1);

	return new->numero;

}

void GUI_Menu_CustomizeButton(GUI_Menu m, int id, const char * border_color, const char * content_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != id)
		current = current->next;

	if (current == NULL)
		return;

	GUI_Button_SetNormalColor(current->button, border_color, content_color);

}

void GUI_Menu_CustomizeButtonTextFont(GUI_Menu m, int id, const char * font_path, unsigned int font_size)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != id)
		current = current->next;

	if (current == NULL)
		return;

	GUI_Button_SetTextFont(current->button, font_path, font_size);

}

void GUI_Menu_CustomizeButtonHover(GUI_Menu m, int id, const char * border_color, const char * content_color)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != id)
		current = current->next;

	if (current == NULL)
		return;

	GUI_Button_SetHoverColor(current->button, border_color, content_color);

}



void GUI_Menu_SetEnabled(GUI_Menu m)
{

	m->enabled = TRUE;

}

void GUI_Menu_SetDisabled(GUI_Menu m)
{

	m->enabled = FALSE;

}

void GUI_Menu_SetPosition(GUI_Menu m, int x, int y)
{

	m->dimensions.x = (x == GUI_CENTERED) ? (Sint16)(((*(m->screen))->w - m->dimensions.w) / 2) : (Sint16)(x);
	m->dimensions.y = (y == GUI_CENTERED) ? (Sint16)(((*(m->screen))->h - m->dimensions.h) / 2) : (Sint16)(y);

	m->dimensions.x = (m->dimensions.x >= ((*(m->screen))->w - m->dimensions.w)) ? (Sint16)((*(m->screen))->w - m->dimensions.w) : m->dimensions.x;
	m->dimensions.x = (m->dimensions.x < 0) ? 0 : m->dimensions.x;

	m->dimensions.y = (m->dimensions.y >= ((*(m->screen))->h - m->dimensions.h)) ? (Sint16)((*(m->screen))->h - m->dimensions.h) : m->dimensions.y;
	m->dimensions.y = (m->dimensions.y < 0) ? 0 : m->dimensions.y;

	GUI_UpdateButtonsPositions(m);

}

void GUI_Menu_SetBorderSize(GUI_Menu m, unsigned int border_size)
{

	m->dimensions_background.w = (Uint16)(m->dimensions.w - 2 * border_size);
	m->dimensions_background.h = (Uint16)(m->dimensions.h - 2 * border_size);
	m->dimensions_background.x = (Sint16)(border_size);
	m->dimensions_background.y = (Sint16)(border_size);

	GUI_Menu_SetColor(m, m->border_color, m->background_color);

}

void GUI_Menu_SetPositionFromAnchor(GUI_Menu m, int x, int y, GUI_Anchor anchor)
{

	if (anchor == TOP_LEFT || (x == GUI_CENTERED && y == GUI_CENTERED))
		GUI_Menu_SetPosition(m, GUI_CENTERED, GUI_CENTERED);
	else
	{

		if (x == GUI_CENTERED)
			m->dimensions.x = (Sint16)(((*(m->screen))->w - m->dimensions.w) / 2);
		else
		{

			m->dimensions.x = (anchor == TOP_RIGHT || anchor == BOTTOM_RIGHT) ? (Sint16)((*(m->screen))->w - m->dimensions.w - x) : (Sint16)(x);
			m->dimensions.x = (m->dimensions.x >= ((*(m->screen))->w - m->dimensions.w)) ? (Sint16)((*(m->screen))->w - m->dimensions.w) : m->dimensions.x;
			m->dimensions.x = (m->dimensions.x < 0) ? 0 : m->dimensions.x;

		}

		if (y == GUI_CENTERED)
			m->dimensions.y = (Sint16)(((*(m->screen))->h - m->dimensions.h) / 2);
		else
		{

			m->dimensions.y = (anchor == BOTTOM_RIGHT || anchor == BOTTOM_LEFT) ? (Sint16)((*(m->screen))->h - m->dimensions.h - y) : (Sint16)(y);
			m->dimensions.y = (m->dimensions.y >= ((*(m->screen))->h - m->dimensions.h)) ? (Sint16)((*(m->screen))->h - m->dimensions.h) : m->dimensions.y;
			m->dimensions.y = (m->dimensions.y < 0) ? 0 : m->dimensions.y;

		}

	}

}

void GUI_Menu_SetColor(GUI_Menu m, const char * border_color, const char * background_color)
{

	if (m->menu == NULL)
	{
		m->menu = SDL_CreateRGBSurface(SDL_HWSURFACE, m->dimensions.w, m->dimensions.h, 32, 0, 0, 0, 0);
		assert(m->menu != NULL);
	}

	strcpy_s(m->border_color, _countof(m->border_color), border_color);
	strcpy_s(m->background_color, _countof(m->background_color), background_color);

	SDL_FillRect(m->menu, NULL, SDL_MapRGB((*(m->screen))->format, GUI_GetRedFromHex(m->border_color), GUI_GetGreenFromHex(m->border_color), GUI_GetBlueFromHex(m->border_color)));
	SDL_FillRect(m->menu, &(m->dimensions_background), SDL_MapRGB((*(m->screen))->format, GUI_GetRedFromHex(m->background_color), GUI_GetGreenFromHex(m->background_color), GUI_GetBlueFromHex(m->background_color)));

}



unsigned int GUI_Menu_GetPositionX(GUI_Menu m)
{

	return m->dimensions.x;

}

unsigned int GUI_Menu_GetPositionY(GUI_Menu m)
{

	return m->dimensions.y;

}

unsigned int GUI_Menu_GetBorderSize(GUI_Menu m)
{

	return m->dimensions_background.x;

}



unsigned int GUI_Menu_GetWidth(GUI_Menu m)
{

	return m->dimensions.w;

}

unsigned int GUI_Menu_GetHeight(GUI_Menu m)
{

	return m->dimensions.h;

}

GUI_Bool GUI_Menu_IsEnabled(GUI_Menu m)
{

	return m->enabled;

}

GUI_Bool GUI_Menu_IsHovered(GUI_Menu m)
{

	if (!m->enabled)
		return FALSE;

	if (m->event->type == SDL_MOUSEMOTION)
	{

		int x = m->event->motion.x;
		int y = m->event->motion.y;

		if (x >= m->dimensions.x && x <= (m->dimensions.x + m->dimensions.w) &&
			y >= m->dimensions.y && y <= (m->dimensions.y + m->dimensions.h))
		{

			if (!m->hover)
				m->hover = TRUE;

		}
		else
		{

			if (m->hover)
				m->hover = FALSE;

		}

	}

	return m->hover;

}

GUI_Bool GUI_Menu_IsClicked(GUI_Menu m)
{

	return (GUI_Menu_IsHovered(m) && m->event->type == SDL_MOUSEBUTTONUP && m->event->button.button == SDL_BUTTON_LEFT);

}

GUI_Bool GUI_Menu_IsRightClicked(GUI_Menu m)
{

	return (GUI_Menu_IsHovered(m) && m->event->type == SDL_MOUSEBUTTONUP && m->event->button.button == SDL_BUTTON_RIGHT);

}

GUI_Bool GUI_Menu_IsButtonHovered(GUI_Menu m, int numero)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != numero)
		current = current->next;

	if (current == NULL)
		return FALSE;

	return GUI_Button_IsHovered(current->button);

}

GUI_Bool GUI_Menu_IsButtonClicked(GUI_Menu m, int numero)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != numero)
		current = current->next;

	if (current == NULL)
		return FALSE;

	return GUI_Button_IsClicked(current->button);

}

GUI_Bool GUI_Menu_IsButtonRightClicked(GUI_Menu m, int numero)
{

	struct s_Menu_Elem * current = m->head;

	while (current->numero != numero)
		current = current->next;

	if (current == NULL)
		return FALSE;

	return GUI_Button_IsRightClicked(current->button);

}



void GUI_Menu_Display(GUI_Menu m)
{

	if (!m->enabled)
		return;

	SDL_BlitSurface(m->menu, NULL, *(m->screen), &(m->dimensions));

	struct s_Menu_Elem * current = m->head;

	while (current)
	{

		GUI_Button_Display(current->button);
		current = current->next;

	}

}

void GUI_Menu_Resize(GUI_Menu m, unsigned int width, unsigned int height)
{

	m->dimensions.w = (width > (unsigned int)((*(m->screen))->w)) ? (Uint16)((*(m->screen))->w) : (Uint16)(width);
	m->dimensions.h = (height > (unsigned int)((*(m->screen))->h)) ? (Uint16)((*(m->screen))->h) : (Uint16)(height);

	SDL_FreeSurface(m->menu);
	m->menu = NULL;

	GUI_Menu_SetColor(m, m->border_color, m->background_color);
	GUI_Menu_SetPosition(m, m->dimensions.x, m->dimensions.y);
	GUI_Menu_SetBorderSize(m, m->dimensions_background.x);

}



void GUI_Menu_Free(GUI_Menu * m)
{

	if ((*m)->head)
		GUI_MenuElem_Free((*m)->head);

	free(*m);
	*m = NULL;

}
