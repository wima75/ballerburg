#pragma once

#include <pch.h>

#define P57       57.296
#define G         0.02    /* Fallbeschleunigung ( g/500 ) */


int comp();
void z_kn();
void z_ka();
void z_ft();
void z_ge();
void z_pk();
void schuss( short k );
void Flugschleife();
void Treffer();
void expls( short x,short y,short w,short h,short d );
void expls2();
int kugel(short x, short y);
void baller(char r);
void bild();
void burg(short nn);
void init_ka(short k,short xr);
void drw_all();
void drw_gpk( char w );
void draw( short x,short y,short *a );
void markt();
void markt2();
void markt3();
void zahl( short nr, short wert );
void fturm();
void anbau();
void anbau2();
void koenig();
void koenig2();
bool drin( short xk,short yk,short w,short h,short r,short x,short y );



struct s_markt2
{
	short k;
};

struct s_flugschleife
{
	double x,y,ox,oy,vx,vy;
	short v,c;
};