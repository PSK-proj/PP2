/*	TRYBY
	- 9x9, 10 bomb
	- 16x16, 40 bomb
	- 30x16, 99 bomb
	- niestand. min: 9x9, 10bomb; max: 30x24, 667 bomb

	ZASADY
	- bomba = -1 wartość pola
	- pole obok co najmniej 1 bomby = ilość bomb wokół
	- pole bez bomb wok� = 0

	przegrana - punkty zliczone z odsłoniętych pól zmniejszyły się (wybrano bombę, czyli -1)

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

int main()
{
	system("CHCP 1250 >NUL");
	srand(time(NULL));

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_BITMAP* baner;


	unsigned char running = 1;
	unsigned char mode = 0;
	unsigned short margin_x = 0, margin_y = 0;

	int** p = NULL; //plansza
	int** clicked = NULL;
	unsigned short* bombs_list = NULL; //tablica pozycji bomb
	unsigned char x, y;
	unsigned short bombs;

	float mouse_x = 0, mouse_y = 0;

	al_init();
	al_init_image_addon();
	al_install_mouse();

	display = al_create_display(1000,650);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	baner = al_load_bitmap("img\\baner.bmp");


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());


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

					printf("\n\n--------------------------------\n\n");
					wypisz(p, x, y);
					printf("\n--------------------------------\n\n");


					al_clear_to_color(al_map_rgb(0, 0, 0), 0, 0);
					al_draw_bitmap(baner, 0, 0, 0);
				}
			}
			else // PEWNIE ODKRYWANIE PÓL
			{
				// dodaj kliknięte pole do tablicy klikniętych pól (zmień wartość w komórce o klikniętych koordynatach [tak jak lista bomb / a może i dwuwymiarowo] na inną niż niekliknięte)
			}
		}


		if(event.type == ALLEGRO_EVENT_TIMER)
		{
			if(mode != 0)
			{
				//funkjca rysująca na podstawie tablicy klikniętych i tablicy 'p'
				allegro_draw_fields(p, clicked, x, y, margin_x, margin_y);
				al_flip_display();
			}
		}
	}



	// Sprzątanie pamięci

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_bitmap(baner);


	free(bombs_list);
	free(p);

	return 0;
}
