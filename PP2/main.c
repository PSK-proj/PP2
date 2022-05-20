/*	TRYBY
	- 9x9, 10 bomb
	- 16x16, 40 bomb
	- 30x16, 99 bomb
	- niestand. min: 9x9, 10bomb; max: 30x24, 667 bomb

	ZASADY
	- bomba = -1 wartoœæ pola
	- pole obok co najmniej 1 bomby = iloœæ bomb wokó³
	- pole bez bomb wokó³ = 0

	przegrana - punkty zliczone z ods³oniêtych pól zmniejszy³y siê (wybrano bombê, czyli -1)
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
		sizeof(int) * y * x //tab int * iloœæ wierszy
	);

	temp = (int*)&p[y]; // y - ostatni element tablicy wskaŸników
	for (int i = 0; i < y; i++)
	{
		p[i] = temp;
		temp += x; // x - wielkoœæ wiersza
	}

	return p;
}

void lvl_select(unsigned char* x, unsigned char* y, unsigned short* bombs, unsigned char* mode)
{
	switch (*mode)
	{
	case 1:
		*x = 9;
		*y = 9;
		*bombs = 10;
		break;
	case 2:
		*x = 16;
		*y = 16;
		*bombs = 40;
		break;
	case 3:
		*x = 30;
		*y = 16;
		*bombs = 99;
		break;
	default:
		*x = 9;
		*y = 9;
		*bombs = 10;
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
	al_draw_bitmap(mode1, 370, 160, 0);
	al_draw_bitmap(mode2, 370, 260, 0);
	al_draw_bitmap(mode3, 370, 360, 0);

	al_flip_display();

	al_destroy_bitmap(baner);
	al_destroy_bitmap(mode1);
	al_destroy_bitmap(mode2);
	al_destroy_bitmap(mode3);
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

	int** p = NULL; //plansza
	unsigned short* bombs_list = NULL; //tablica pozycji bomb
	unsigned char x, y;
	unsigned short bombs;

	float mouse_x = 0, mouse_y = 0;

	al_init();
	al_init_image_addon();
	al_install_mouse();

	display = al_create_display(1000,560);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	baner = al_load_bitmap("img\\baner.bmp");


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());


	al_start_timer(timer);

	allegro_display_menu();


	// GRAFICZNE WYŒWIETLANIE
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
			if (mode == 0) // WYBIERANIE TRUDNOŒCI
			{
				if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 160 && mouse_y <= 240) mode = 1;
				else if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 260 && mouse_y <= 340) mode = 2;
				else if(mouse_x >= 370 && mouse_x <= 630 && mouse_y >= 360 && mouse_y <= 440) mode = 3;

				if (mode != 0)
				{
					lvl_select(&x, &y, &bombs, &mode);
					p = create_board(x, y);
					bombs_list = create_bombs_list(bombs);
					clear_board(p, x, y);


					bombs_draw(p, x, y, bombs, bombs_list);
					set_field_numbers(p, x, y, bombs, bombs_list); //Tu s¹ tak¿e wypisywanie koordynaty bomb

					printf("\n\n--------------------------------\n\n");
					wypisz(p, x, y);
					printf("\n--------------------------------\n\n");

					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_bitmap(baner, 0, 0, 0);
				}
			}
			else // PEWNIE ODKRYWANIE PÓL
			{
				// dodaj klikniête pole do tablicy klikniêtych pól (zmieñ wartoœæ w komórce o klikniêtych koordynatach [tak jak lista bomb / a mo¿e i dwuwymiarowo] na inn¹ ni¿ nieklikniête)
			}
		}


		if(event.type == ALLEGRO_EVENT_TIMER)
		{
			if(mode != 0)
			{

				//funkjca rysuj¹ca na podstawie tablicy klikniêtych i tablicy 'p'
				al_flip_display();
			}
		}
	}



	// Sprz¹tanie pamiêci

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_bitmap(baner);


	free(bombs_list);
	free(p);

	return 0;
}
