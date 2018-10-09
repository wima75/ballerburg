#pragma once

#include <pch.h>

/*struct RenderObj {
        double x;
        double y;
        double ox;
        double oy;
        double vx;
        double vy;
        int v;
        int c;
        int a;
        int j;
};*/

void main1();

void event1();
void event2(bool bur_obj_result);
void tabelle();
void z_txt(short a);
void neues();
void ein_zug();
void rechnen();
void ende();
void m_wait();
void werdran(char c);
void fahne();
Concurrency::task<void> t_load();
Concurrency::task<void> load();
Concurrency::task<void> file_exists();
Concurrency::task<void> t_save();
void burgen_laden();
char zeichen();
int rdzahl();
short sch_obj(short k);
void bur_obj();
void bur_obj2();
void bur_obj3(short i);
void obj_do(int adr);
void obj_do2();
void gem_init();
void color(short a);
int loc(short x, short y);
void line( short x1, short y1, short x2, short y2 );
void box(short x, short y, short x2, short y2, short c);
void clr( short x, short y, short w, short h );
void cls();
void regeln();
void optionen();
void computer_gewaehlt();
void eintrag();
void eintrag2();
void tabelle2();
void namen();
void namen2();

/*struct s_ein_zug1
{
	short i,fl,a;
};*/

void ein_zug1();
void ein_zug2(short fl, short i);
void ein_zug3();
void ein_zug4();

/*struct s_ein_zug2
{
	short i;
};*/

struct s_sch_obj1
{
	short i;
	short wi;
	short pv;
	short xw;
	short yw;
	short xp;
	short yp;
	short xk;
	short yk;
	char *aw;
	char *ap;
	char fl;
	short k;
};

void sch_obj1();
void sch_obj2();
