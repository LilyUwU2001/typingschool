#include <allegro.h>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <iostream>
#include <math.h>
#include <conio.h>

bool litery_wl[10];
char litery[10];
float litery_x[10];
float litery_y[10];
char litery_znak[10];
bool litery_na_ziemi[10];
bool litery_zn[10];

bool pocisk_wl[10];
float pocisk_x[10];
float pocisk_y[10];
int pocisk_litera[10];

int ufo_x = 16;
int ufo_kierunek = 1;

int random = 0;
int predkosc = 1;
int licznik_zbitych = 0;
int punkty = 0;
int klawisz = 0;
int liczba_na_ziemi = 0;

BITMAP * bufor = NULL;
BITMAP * ludek = NULL;

BITMAP * ziemia = NULL;
BITMAP * tlo = NULL;
BITMAP * dzialo = NULL;
BITMAP * pocisk = NULL;
BITMAP * spadochroniarz = NULL;
BITMAP * spadochroniarz_ziemia = NULL;
BITMAP * ufo = NULL;
BITMAP * gwiazdy = NULL;
BITMAP * okno = NULL;

inline void init()
{
	allegro_init();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0);

	install_timer();
	install_keyboard();
	install_mouse();
	install_timer();
}

inline void deinit()
{
	clear_keybuf();
	allegro_exit();
}

void dzwiek_strzalu()
{
    //printf("\a");
}

void sprawdzaj_koniecgry()
{
    for (int i=0; i<10; i++) {
        if (litery_na_ziemi[i] == 1) {
            liczba_na_ziemi += 1;
        }
    }
    if (liczba_na_ziemi < 5) {
        liczba_na_ziemi = 0;
    }
}

void update_pociski()
{
    for (int i=0; i<10; i++) {
        if (pocisk_wl[i] == 1) {
            if (pocisk_wl[i] == 1 && litery_wl[pocisk_litera[i]] && pocisk_x[i] < litery_x[pocisk_litera[i]]+8) {
                pocisk_x[i] += 8;
            }
            if (pocisk_wl[i] == 1 && litery_wl[pocisk_litera[i]] && pocisk_x[i] > litery_x[pocisk_litera[i]]+8) {
                pocisk_x[i] -= 8;
            }
            if (pocisk_wl[i] == 1 && litery_wl[pocisk_litera[i]] && pocisk_y[i] < litery_y[pocisk_litera[i]]+8) {
                pocisk_y[i] += 8;
            }
            if (pocisk_wl[i] == 1 && litery_wl[pocisk_litera[i]] && pocisk_y[i] > litery_y[pocisk_litera[i]]+8) {
                pocisk_y[i] -= 8;
            }
            if (pocisk_wl[i] == 1 && pocisk_x[i] > litery_x[pocisk_litera[i]] && pocisk_x[i] < litery_x[pocisk_litera[i]]+24) {
                if (pocisk_wl[i] == 1 && pocisk_y[i] > litery_y[pocisk_litera[i]] && pocisk_y[i] < litery_y[pocisk_litera[i]]+8) {
                    punkty += predkosc * 1;
                    licznik_zbitych += 1;
                    litery_zn[pocisk_litera[i]] = 0;
                    litery_wl[pocisk_litera[i]] = 0;
                    litery_na_ziemi[pocisk_litera[i]] = 0;
                    pocisk_wl[i] = 0;
                }
            }
        }
    }
}

void spawnuj_pocisk(int litera)
{
    for (int i=0; i<10; i++) {
        if (pocisk_wl[i] == 0 && litery_zn[litera] == 0) {
            pocisk_wl[i] = 1;
            pocisk_x[i] = 124;
            pocisk_y[i] = 168;
            pocisk_litera[i] = litera;
            litery_zn[pocisk_litera[i]] = 1;
            dzwiek_strzalu();
        }
    }
}

void sprawdzaj_klawiature()
{
    for (int i=0; i<10; i++) {
        if (litery_wl[i] == 1) {
            if (keypressed()) {
                klawisz = readkey();
            }
            if ((klawisz & 0xff) == litery_znak[i]) {
                spawnuj_pocisk(i);
                klawisz = 0;
            }
        }
    }
}

void update_spadochroniarzy()
{
    for (int i=0; i<10; i++) {
        if (litery_wl[i] == 1 && litery_na_ziemi[i] == 0) {
            litery_y[i] += (predkosc+1)/2;
        }
        if (litery_y[i] > 168) {
            litery_na_ziemi[i] = 1;
        }
    }
}

void spawnuj_spadochroniarza(int x)
{
    bool zespawnowano = false;

    if (liczba_na_ziemi >= 5) {
        zespawnowano = true;
    }

    for (int i=0; i<10; i++) {
        if (litery_wl[i] == 0 && zespawnowano == false) {
            litery_wl[i] = 1;
            litery_x[i] = ufo_x+16;
            litery_y[i] = 48;
            litery_znak[i] = 33 + rand()%92;
            zespawnowano = true;
        }
    }
}

void update_ufo()
{
    if (ufo_kierunek == 0) {
        ufo_x -= 4;
        if (ufo_x < 16) {
            ufo_kierunek = 1;
        }
    }
    if (ufo_kierunek == 1) {
        ufo_x += 4;
        if (ufo_x > 192) {
            ufo_kierunek = 0;
        }
    }
    random = rand()%20;
    if (random == 0) {
        spawnuj_spadochroniarza(ufo_x);
        random = -1;
    }
}

void update_gry()
{
    update_ufo();
    update_spadochroniarzy();
    update_pociski();
    sprawdzaj_klawiature();
    sprawdzaj_koniecgry();
}

void rysuj_info()
{
    textprintf(bufor, font, 8, 8, makecol(255,255,255), "SCORE: %i", punkty);
    textprintf(bufor, font, 168, 8, makecol(255,255,255), " LEVEL: %i", predkosc);
    if (licznik_zbitych > (predkosc * 10)) {
        predkosc++;
        licznik_zbitych = 0;
    }
    if (liczba_na_ziemi >= 5) {
        textprintf_centre(bufor, font, 128, 96, makecol(255,255,255), "GAME OVER");
    }
}

void rysuj_spadochroniarzy()
{
    for (int i=0; i<10; i++) {
        if (litery_wl[i]) {
            if (litery_na_ziemi[i] == 0) {
                masked_blit( spadochroniarz, bufor, 0, 0, litery_x[i], litery_y[i], 256, 48);
                textprintf_centre_ex(bufor, font, litery_x[i]+12, litery_y[i]+2, makecol(255,255,255), makecol(224,0,64), "%c", litery_znak[i]);
            }
            if (litery_na_ziemi[i] == 1) {
                masked_blit( spadochroniarz_ziemia, bufor, 0, 0, litery_x[i]+8, litery_y[i]+23, 256, 48);
            }
        }
    }
}

void rysuj_pociski()
{
    for (int i=0; i<10; i++) {
        if (pocisk_wl[i]) {
            masked_blit( pocisk, bufor, 0, 0, pocisk_x[i], pocisk_y[i], 256, 48);
        }
    }
}

void rysuj_tlo()
{
    clear_to_color( bufor, makecol( 0, 0, 0 ) );
    masked_blit( gwiazdy, bufor, 0, 0, 0, 0, 256, 128);
    masked_blit( gwiazdy, bufor, 0, 0, 0, 96, 256, 128);
    masked_blit( ziemia, bufor, 0, 0, 0, 200, 256, 48);
    masked_blit( tlo, bufor, 0, 0, 0, 104, 256, 96);
}

void rysuj_ufo()
{
    masked_blit( ufo, bufor, 0, 0, ufo_x, 24, 256, 48);
}

void rysowanie_gry()
{
    rysuj_tlo();
    rysuj_ufo();
    rysuj_spadochroniarzy();
    rysuj_pociski();
    rysuj_info();
    //masked_stretch_blit( okno, screen, 0, 0, 320, 240, 0, 0, 640, 480);
    stretch_blit( bufor, screen, 0, 0, 256, 224, 64, 16, 512, 448 );
}

int main()
{
    srand( time( NULL ) );
	init();

	bufor = create_bitmap( 256, 224 );
    if( !bufor )
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie mogê utworzyæ bufora !" );
        allegro_exit();
        return 0;
    }

    ziemia = load_bmp( "ziemia.bmp", default_palette );
    tlo = load_bmp( "tlo.bmp", default_palette );
    dzialo = load_bmp( "dzialo.bmp", default_palette );
    pocisk = load_bmp( "pocisk.bmp", default_palette );
    spadochroniarz = load_bmp( "spadochroniarz.bmp", default_palette );
    spadochroniarz_ziemia = load_bmp( "spadochroniarz_ziemia.bmp", default_palette );
    ufo = load_bmp( "ufo.bmp", default_palette );
    gwiazdy = load_bmp( "gwiazdy.bmp", default_palette );
    okno = load_bmp( "okno.bmp", default_palette );

	while (!key[KEY_ESC])
	{
	    rest(100);
	    rysowanie_gry();
	    update_gry();
		/* put your code here */
	}

	deinit();
	return 0;
}
END_OF_MAIN()
