/*	TRYBY
	- 9x9, 10 bomb
	- 16x16, 40 bomb
	- 30x16, 99 bomb

	ZASADY
	- bomba = -1 wartość pola
	- pole obok co najmniej 1 bomby = ilość bomb wokół
	- pole bez bomb wokół = 0

	LISTA ODSŁONIĘTYCH
	- 0 - nieodsłonięte
	- 10 - odsłonięte
	- -2 - flaga
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

void wypisz(int** tab, unsigned char x, unsigned char y)
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			if (tab[i][j] == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x40);
			else if (tab[i][j] == 0) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
			else if (tab[i][j] == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x09);
			else if (tab[i][j] == 2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
			else if (tab[i][j] == 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
			else if (tab[i][j] == 4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
			else if (tab[i][j] == 5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x05);
			else if (tab[i][j] == 6) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x06);
			else if (tab[i][j] == 7) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
			else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x08);

			printf("%2d", tab[i][j]);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
		putchar('\n');
	}
}

void bombs_draw(int** tab, unsigned char x, unsigned char y, unsigned short bombs, unsigned short* bombs_list)
{
	unsigned char x_b, y_b;
	unsigned short size = x * y;
	unsigned short q = bombs;
	while(bombs)
	{
		y_b = (rand() % size) / x;
		x_b = (rand() % size) % x;
		if (tab[y_b][x_b] != -1)
		{
			tab[y_b][x_b] = -1;
			bombs_list[q - bombs] = y_b * x + x_b;
			bombs--;
		}
	}
}

void set_field_numbers(int** tab, unsigned char x, unsigned char y, unsigned short bombs, unsigned short* bombs_list)
{
	printf("   KOORDYNATY: \n");
	unsigned char x_b, y_b;
	unsigned short x_b_p, y_b_p;
	short pos_x, pos_y;
	for (int i = 0; i < bombs; i++)
	{
		y_b = bombs_list[i] / x;
		x_b = bombs_list[i] % x;
		printf("\n%3d.bomba: %3hu, y_b: %3hhu, x_b: %3hhu\n", i + 1, bombs_list[i], y_b, x_b);

		for (int j = 0; j < 9; j++)
		{
			y_b_p = j / 3;
			x_b_p = j % 3;
			pos_y = y_b + y_b_p -1;
			pos_x = x_b + x_b_p -1;

			if (!(pos_x < 0 || pos_y < 0 || pos_x > x - 1 || pos_y > y - 1 || tab[pos_y][pos_x] == -1))
				tab[pos_y][pos_x] += 1;
		}
	}
}

void clear_board(int** tab, unsigned char x, unsigned char y)
{
	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
			tab[i][j] = 0;
}

unsigned short* create_bombs_list(unsigned short bombs)
{
	unsigned short* bombs_list;
	bombs_list = (unsigned short*)malloc(sizeof(unsigned short) * bombs);
	return bombs_list;
}

int **create_board(unsigned char x, unsigned char y)
{
	int** p;
	int* temp;

	p = (int**)malloc(
		sizeof(int*) * y + //tab ptr
		sizeof(int) * y * x //tab int * ilo�� wierszy
	);

	temp = (int*)&p[y]; // y - ostatni element tablicy wska�nik�w
	for (int i = 0; i < y; i++)
	{
		p[i] = temp;
		temp += x; // x - wielko�� wiersza
	}

	return p;
}

void lvl_select(unsigned char* x, unsigned char* y, unsigned short* bombs, unsigned char* mode, unsigned short* margin_x, unsigned short* margin_y)
{
	switch (*mode)
	{
	case 1:
		*x = 9;
		*y = 9;
		*bombs = 10;
		*margin_x = 365;
		*margin_y = 265;
		break;
	case 2:
		*x = 16;
		*y = 16;
		*bombs = 40;
		*margin_x = 260;
		*margin_y = 160;
		break;
	case 3:
		*x = 30;
		*y = 16;
		*bombs = 99;
		*margin_x = 50;
		*margin_y = 160;
		break;
	default:
		*x = 9;
		*y = 9;
		*bombs = 10;
		*margin_x = 365;
		*margin_y = 265;
		break;
	}
}


void allegro_display_menu()
{
	ALLEGRO_BITMAP* baner;
	ALLEGRO_BITMAP* mode1;
	ALLEGRO_BITMAP* mode2;
	ALLEGRO_BITMAP* mode3;

	baner = al_load_bitmap("img\\baner.bmp");
	mode1 = al_load_bitmap("img\\mode1.bmp");
	mode2 = al_load_bitmap("img\\mode2.bmp");
	mode3 = al_load_bitmap("img\\mode3.bmp");

	al_draw_bitmap(baner, 0, 0, 0);
	al_draw_bitmap(mode1, 370, 200, 0);
	al_draw_bitmap(mode2, 370, 300, 0);
	al_draw_bitmap(mode3, 370, 400, 0);

	al_flip_display();

	al_destroy_bitmap(baner);
	al_destroy_bitmap(mode1);
	al_destroy_bitmap(mode2);
	al_destroy_bitmap(mode3);
}

void allegro_draw_fields(int** tab, int** clicked, unsigned char x, unsigned char y, unsigned short margin_x, unsigned short margin_y)
{
	ALLEGRO_BITMAP* hidden;
	ALLEGRO_BITMAP* flag;
	ALLEGRO_BITMAP* bomb;
	ALLEGRO_BITMAP* empty;
	ALLEGRO_BITMAP* one;
	ALLEGRO_BITMAP* two;
	ALLEGRO_BITMAP* three;
	ALLEGRO_BITMAP* four;
	ALLEGRO_BITMAP* five;
	ALLEGRO_BITMAP* six;
	ALLEGRO_BITMAP* seven;
	ALLEGRO_BITMAP* eight;

	hidden = al_load_bitmap("img\\hidden.bmp");
	flag = al_load_bitmap("img\\flag.bmp");
	bomb = al_load_bitmap("img\\bomb.bmp");
	empty = al_load_bitmap("img\\empty.bmp");
	one = al_load_bitmap("img\\1.bmp");
	two = al_load_bitmap("img\\2.bmp");
	three = al_load_bitmap("img\\3.bmp");
	four = al_load_bitmap("img\\4.bmp");
	five = al_load_bitmap("img\\5.bmp");
	six = al_load_bitmap("img\\6.bmp");
	seven = al_load_bitmap("img\\7.bmp");
	eight = al_load_bitmap("img\\8.bmp");

	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
		{
			if (clicked[i][j] == 0)
				al_draw_bitmap(hidden, margin_x + j*30, margin_y + i*30, 0);
			else if(clicked[i][j] == -2)
				al_draw_bitmap(flag, margin_x + j * 30, margin_y + i * 30, 0);
			else if (clicked[i][j] == 10)
			{
				switch (tab[i][j])
				{
				case -1: al_draw_bitmap(bomb, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 0: al_draw_bitmap(empty, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 1: al_draw_bitmap(one, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 2: al_draw_bitmap(two, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 3: al_draw_bitmap(three, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 4: al_draw_bitmap(four, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 5: al_draw_bitmap(five, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 6: al_draw_bitmap(six, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 7: al_draw_bitmap(seven, margin_x + j * 30, margin_y + i * 30, 0); break;
				case 8: al_draw_bitmap(eight, margin_x + j * 30, margin_y + i * 30, 0); break;
				}
			}
		}


	al_destroy_bitmap(hidden);
	al_destroy_bitmap(flag);
	al_destroy_bitmap(bomb);
	al_destroy_bitmap(empty);
	al_destroy_bitmap(one);
	al_destroy_bitmap(two);
	al_destroy_bitmap(three);
	al_destroy_bitmap(four);
	al_destroy_bitmap(five);
	al_destroy_bitmap(six);
	al_destroy_bitmap(seven);
	al_destroy_bitmap(eight);
}

void allegro_display_win()
{
	al_clear_to_color(al_map_rgb(0, 0, 0), 0, 0);
	ALLEGRO_BITMAP* baner;
	ALLEGRO_BITMAP* win;
	baner = al_load_bitmap("img\\baner.bmp");
	win = al_load_bitmap("img\\win.bmp");
	al_draw_bitmap(baner, 0, 0, 0);
	al_draw_bitmap(win, 0, 80, 0);
	al_flip_display();
	al_destroy_bitmap(baner);
	al_destroy_bitmap(win);
}

void allegro_display_stats(short bombs_remain, int time)
{
	char* bombs_string = malloc(4);
	snprintf(bombs_string, 4, "%hi", bombs_remain);
	
	char* time_string = malloc(6);
	snprintf(time_string, 6, "%d", time);

	ALLEGRO_FONT* roboto_regular;
	ALLEGRO_FONT* roboto_bold_italic;
	roboto_regular = al_load_font("fonts\\Roboto-Regular.ttf", 20, NULL);
	roboto_bold_italic = al_load_font("fonts\\Roboto-BoldItalic.ttf", 20, NULL);

	al_draw_text(roboto_regular, al_map_rgb(255, 73, 0), 200, 110, ALLEGRO_ALIGN_LEFT, "Bomby: ");
	al_draw_text(roboto_bold_italic, al_map_rgb(255, 122, 0), 270, 110, ALLEGRO_ALIGN_LEFT, bombs_string);
	
	al_draw_text(roboto_regular, al_map_rgb(255, 73, 0), 700, 110, ALLEGRO_ALIGN_LEFT, "Czas: ");
	al_draw_text(roboto_bold_italic, al_map_rgb(255, 122, 0), 770, 110, ALLEGRO_ALIGN_LEFT, time_string);

	al_destroy_font(roboto_regular);
	al_destroy_font(roboto_bold_italic);
}


int handle_field_click(int** tab, int** clicked, unsigned short bombs, unsigned char x, unsigned char y, unsigned short margin_x, unsigned short margin_y, float mouse_x, float mouse_y, ALLEGRO_EVENT* event, short* bombs_remain, unsigned short* clicks_made)
{
	int clicked_x = (mouse_x - margin_x) / 30;
	int clicked_y = (mouse_y - margin_y) / 30;
	if (clicked_x >= 0 && clicked_x < x && mouse_x - margin_x >= 0 && clicked_y >= 0 && clicked_y < y && mouse_y - margin_y >= 0)
	{
		if (event->mouse.button & 1 && clicked[clicked_y][clicked_x] != -2 && clicked[clicked_y][clicked_x] != 10)
		{
				clicked[clicked_y][clicked_x] = 10;
				if (tab[clicked_y][clicked_x] == -1)
				{
					for (int i = 0; i < y; i++)
						for (int j = 0; j < x; j++)
							clicked[i][j] = 10;
					return -1;
				}
				*clicks_made = *clicks_made + 1;
				if (*clicks_made == (x * y) - bombs) return 1;
		}
		if (event->mouse.button & 2 && clicked[clicked_y][clicked_x] != 10) //stawianie flagi
		{
			if (clicked[clicked_y][clicked_x] == 0)
			{
				clicked[clicked_y][clicked_x] = -2;
				*bombs_remain = *bombs_remain-1;
			}
			else if (clicked[clicked_y][clicked_x] == -2)
			{
				clicked[clicked_y][clicked_x] = 0;
				*bombs_remain = *bombs_remain + 1;
			}
		}
	}
	return 0;
}


int main()
{
	system("CHCP 1250 >NUL");
	srand(time(NULL));

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_TIMER* game_timer;
	ALLEGRO_BITMAP* baner;


	unsigned char running = 1;
	unsigned char mode = 0;
	unsigned short margin_x = 0, margin_y = 0;
	unsigned short clicks_made = 0;
	int game_state = 0; // 0 - w trakcie, 1 - wygrana, -1 - przegrana
	int time = 0;

	int** p = NULL; //plansza
	int** clicked = NULL;
	unsigned short* bombs_list = NULL;
	unsigned char x, y;
	unsigned short bombs;
	short bombs_remain;

	float mouse_x = 0, mouse_y = 0;

	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();

	display = al_create_display(1000,650);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	game_timer = al_create_timer(1.0);
	baner = al_load_bitmap("img\\baner.bmp");


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(game_timer));
	al_register_event_source(queue, al_get_mouse_event_source());

	al_set_window_title(display, "Gra Saper");
	al_start_timer(timer);


	allegro_display_menu();


	// GRAFICZNE WYŚWIETLANIE
	while (running == 1)
	{

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) running = 0;
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_x = event.mouse.x;
			mouse_y = event.mouse.y;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (mode == 0) // WYBIERANIE TRUDNOŚCI
			{
				if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 200 && mouse_y <= 280) mode = 1;
				else if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 300 && mouse_y <= 380) mode = 2;
				else if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 400 && mouse_y <= 480) mode = 3;

				if (mode != 0)
				{
					lvl_select(&x, &y, &bombs, &mode, &margin_x, &margin_y);
					
					p = create_board(x, y);
					clicked = create_board(x, y);
					bombs_list = create_bombs_list(bombs);

					clear_board(p, x, y);
					clear_board(clicked, x, y);

					bombs_draw(p, x, y, bombs, bombs_list);
					set_field_numbers(p, x, y, bombs, bombs_list); //Tu są także wypisywanie koordynaty bomb
					bombs_remain = bombs;
					printf("\n\n--------------------------------\n\n");
					wypisz(p, x, y);
					printf("\n--------------------------------\n\n");

					al_start_timer(game_timer);
					al_clear_to_color(al_map_rgb(0, 0, 0), 0, 0);
				}
			}
			else if(mode == 1 || mode == 2 || mode == 3)
			{
				game_state = handle_field_click(p, clicked, bombs, x, y, margin_x, margin_y, mouse_x, mouse_y, &event, &bombs_remain, &clicks_made);
				printf("bombs: %hi\n", bombs_remain);
				
			}
		}


		if(event.type == ALLEGRO_EVENT_TIMER)
		{
			if(mode != 0)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0), 0, 0);
				al_draw_bitmap(baner, 0, 0, 0);
				allegro_draw_fields(p, clicked, x, y, margin_x, margin_y);

				if(event.timer.source == game_timer && mode != 10)
					time = event.timer.count;
				allegro_display_stats(bombs_remain, time);

				if(game_state == 1)
				{
					mode = 10;
					allegro_display_win();
					
				}
				else if (game_state == -1)
					mode = 10;

				al_flip_display();
			}
		}
	}


	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_timer(game_timer);
	al_destroy_bitmap(baner);


	free(bombs_list);
	free(p);

	return 0;
}
