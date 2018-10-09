#include <pch.h>
#include <Baller.h>
#include <Baller1.h>
#include <Baller2.h>
#include <Sound.h>

#define Min(a,b)  ((a)<(b)?(a):(b))
#define Max(a,b)  ((a)>(b)?(a):(b))
#define maus()    graf_mkstate(&mx,&my,&bt,&dum)
#define hide()    graf_mouse(256,0)
#define show()    graf_mouse(257,0)
//#define menu(a)   menu_bar(a_men,a)
//#define sav_scr() hide();movmem(scr,buf,32000);show()
//#define lod_scr() hide();movmem(buf,scr,32000);show()
#define sav_scr() hide();handle->SaveRectangle(0, 0, 640, 400, _buf_scr);show();
#define lod_scr() hide();handle->RestoreRectangle(0, 0, 640, 400, _buf_scr);show();
#define PI        3.1416
#define P57       57.296
#define G         0.02    /* Fallbeschleunigung ( g/500 ) */
//#define bing()    printf("\007");
#define fn()      f=1-2*(n&1);

void (*mouse_move_callback)();

extern double vvx,vvy;
extern WriteableBitmapEx* handle;
extern short  mx,my,bt,dum,m_buf[8], xy[100], 
      bur[2],bx[2],by[2], ge[2],pu[2],ku[2],vo[2],st[2],kn[2],
      wx[2],wy[2],ws,wc,
      *bg, zug,n,oldn, p[6],
      fx,fy,fw,fh,
      ftx,fty,ftw,fth,
      f_h, *burgen[20],b_anz;
extern int   buf, scr/*, bur_ad*/, l_nam,r_nam,
      a_men,a_inf,a_sch,a_brg,a_nam,a_dra,a_sta,a_sie;
extern byte* _buf_scr;
extern char  f, mod, wnd, end, an_erl,
      cw[2],cx[2],
      nsp1[],nsp2[];
extern struct { short x,y,w,p; } ka[2][10];
extern struct { short x,y; } ft[2][5];

extern void (*callback)();
extern byte* _form_dial_screen;
extern ressource_form* res_form;
extern bool menu_enabled;

/***************** Grafikdaten fÅ¸r Trohn, Kanone... **************************/
short trohn[]={ -2,2,2, 8,3,5,12,8,12,12,16,12,17,14,15,
   15,15,17,17,17,16,21,11,24,11,21,3,-9, -4, 3,0,5,2,24,2,26,0,-9,
   19,4,18,9,-9, 10,4,11,9,-9, 14,4,15,4,-9, 8,10,11,11,-9, 21,10,18,11,-9,
   14,17,15,17,-9, 13,19,13,19,-9, 16,19,16,19,-9, 12,22,12,21,15,21,14,22,
   17,21,17,22,-9, 6,13,6,17,5,16,5,17,7,17,7,16,-9, -1 },
      kanon[]={ -2,1,1, 1,0,3,3,5,0,3,3,3,11,2,11,2,9,4,9,4,11,3,11,3,8,0,5,
   3,8,6,5,3,8,3,3,-9, 11,0,13,2,15,0,16,0,17,1,17,2,15,4,19,8,16,11,8,3,-9,
   -1 },
      sack[]={ -4, 1,1,1,4,2,5,2,0,3,0,3,8,2,8,4,8,3,8,3,6,4,6,4,0,5,0,
   5,6,6,5,6,1,-9, -1 },
      fass[]={ -4, 2,0,6,0,8,3,8,5,6,8,2,8,0,5,0,3,2,0,3,0,3,3,2,4,3,5,3,8,
   5,8,5,5,6,4,5,3,5,1,-9, -1 },
      kuge[]={ -2,1,1, 1,0,0,1,0,3,1,4,3,4,4,3,4,1,3,0,-9,-1 },
      turm[]={ -4, 23,30,27,0,3,10,24,20,6,30,23,30,5,20,26,10,2,0,6,30,-9,
   8,0,8,31,9,34,11,36,14,37,15,37,18,36,20,34,21,31,21,29,20,26,18,24,15,23,
   14,23,11,24,9,26,8,29,-9, 14,23,15,37,-9, 10,25,19,35,-9, 10,35,19,25,-9,
   21,30,21,0,-9, -1 };
/************************ Der Computer agiert: *******************************/
int zx,zy;  /* Koordinaten des Zieles */
short *bh;


//RenderObj _renderObj;
//extern Sound^ _sound;

int comp()  /* F¸Åhrt einen Zug des Computers durch */
{
   char wd;
   int i,t;
   register double x2=0,x,y,vx2,vx,vy=0,wi;

   st[n]=16;
   for( i=0;i<10;i++ ) if( ka[n][i].x>-1 ) break;
   if( pu[n]<20 && ge[n]>=p[4] ) { pu[n]+=30; ge[n]-=p[4]; }  /* Pulv. kaufen*/
   if( !ku[n] && ge[n]>=p[5] )   { ku[n]+=2;  ge[n]-=p[5]; }  /* Kug.  kaufen*/
   if( i>9 && ge[n]>=p[2] ) { init_ka(i=0,639*n); ge[n]-=p[2]; } /* Ka. kauf.*/
   if( (ft[n][0].x<0 || ft[n][1].x<0 || ft[n][2].x<0 ) && ge[n]>=p[1] &&
       cw[n]>2 )
      { ge[n]-=p[1]; fturm(); } /* Fîrderturm kaufen */
   drw_all();

   if( i>9 || pu[n]<20 || !ku[n] ) return(-1);

        /* Jetzt kommen die Berechnungen f¸r den Schuss: */
   bh=burgen[bur[!n]];
   do i=Random()%10; while( ka[n][i].x==-1 );

   zx=639*!n-f*(Random()%bh[0]); zy=by[!n];
   t=Random()%100;
   if( cw[n] ) z_kn();
   switch( cw[n] )  /* Die verschiedenen Strategien */
   {
      case 1: if( t<30 ) z_ge(); if( t>60 ) z_pk(); break;
      case 2: z_ka(); if( t<90 ) z_ge(); break;
      case 3: if( t<50 )z_ka(); else if( t<70 )z_ge(); else if( t<90 )z_pk();
              if( !(Random()%3) ) z_ft();
              break;
      case 4: z_ka(); break;
      case 5: z_ka(); if( t<90 ) z_ft();
   }

   y=ka[n][i].y-10-zy;    t=49-cx[n]*16;   /* Berechnen der Distanz */
   x=ka[n][i].x+9+8*f-zx -t/2+Random()%t; if( x<0 ) x=-x;

   wd=n? -wnd:wnd;

   x2=-1;
   for( vx=.5; vx<7 && ( x2<x || vy/3.5>vx ); )  /* Zeitberechnung bei */
   {                              /* verschiedenen X-Geschwindigkeiten */
      vx+=.4;
      t=x2=0; vx2=vx;
      while( vx2>0 && x2<x )
         { x2+=vx2; vx2+=(wd/2-vx2)/5000; t++; }
      vy=y/t+.5*G*t;
   }

   if( n ) vx=-vx;
   vvx=vx; vvy=vy;  /* Werte fÅr Schuss Åbergeben */

   if( vx<0 ) vx=-vx; 
   wi=atan2(vy,vx);
   ka[n][i].w=wi*P57;
   ka[n][i].p=(int)(4.0*vx/cos(wi)-1.6);

   return(i);
}

/********************* Routinen zur Zielerkennung: ***************************/
/* Die Routinen berechnen die Zielkoordinaten fÅr den Kînig, Kanonen, Geld...*/
void z_kn()
{
   zx=!n*639-f*(bh[21]+15);
   zy=by[!n]-bh[22]-5;
}

void z_ka()
{
   short i;

   for( i=0;i<10;i++ ) if( ka[!n][i].x>-1 ) break;
   if( i<10 )
   {
      do i=Random()%10; while( ka[!n][i].x==-1 );
      zx=ka[!n][i].x+10; zy=ka[!n][i].y;
   }
}
void z_ft()
{
   short i;

   for( i=0;i<5;i++ ) if( ft[!n][i].x>-1 ) break;
   if( i<5 )
      { zx=ft[!n][i].x-15*f; zy=ft[!n][i].y-10; }
}
void z_ge()
{
   if( ge[!n]>100 )
      { zx=!n*639-f*(bh[25]+bh[31]/2); zy=by[!n]-bh[26]; }
}
void z_pk()
{
   short i;

   if( ku[!n] || pu[!n]>19 )
   {
      i=Random()&2;
      zx=!n*639-f*(bh[27+i]+bh[33+i]/2); zy=by[!n]-bh[28+i];
   }
}

extern void (*render_callback)();
s_flugschleife _flugschleife;
/********************************* Ein Schuss ********************************/
void schuss( short k ) {
	double x,y,ox,oy,vx,vy;
	short v,c,a, j;

	ox = oy = 0;
	hide();
	pu[n]-=ka[n][k].p; ku[n]--;
	drw_gpk(1); drw_gpk(2);

	x=ka[n][k].x+9+8*f; y=ka[n][k].y-10; c=2;
	vx=(.4+0.25*ka[n][k].p)*cos( ka[n][k].w/P57 )*f;
	vy=(.4+0.25*ka[n][k].p)*sin( ka[n][k].w/P57 );

	if( mod&(2-n) ) { vx=vvx; vy=vvy; }

	color(1); vswr_mode( handle,3 );

	baller(0);

	kugel( (int)x,(int)y ); v=1;

	_flugschleife.x = x;
	_flugschleife.y = y;
	_flugschleife.ox = ox;
	_flugschleife.oy = oy;
	_flugschleife.vx = vx;
	_flugschleife.vy = vy;
	_flugschleife.v = v;
	_flugschleife.c = c;
	render_callback = &Flugschleife;

	/*
	int wnd = 0;
	while( x>3 && x<637 && y<396 &&  ( v || c ) )  // Flugschleife 
   {
      ox=x; oy=y; x+=vx; y-=vy;
      vy-=G; vx+=(wnd/2-vx)/5000;
      //kugel( (int)ox,(int)oy );
      //if( kugel( (int)x,(int)y ) )
      //   v=loc((int)x,(int)y)& loc((int)x-1,(int)y+1)& loc((int)x+1,(int)y+2);
      a=1000+2*y; if( a<30 ) a=30;
	  float f = 111861.12f / a;
	  //_sound->Test(f);
      //Giaccess( 10,137 );    Giaccess( 244,135 );
      //Giaccess( a&255,130 ); Giaccess( a>>8,131 );

      if( c ) c--;

   }*/
	//_sound->Test2();
}


enum Goto
{
	None,
    Kanone,
	Foerderturm,
	Koenig,
	Koenig2,
	GPK
};
struct s_Treffer
{
	short v,c,a,j;
	double x,y,ox, oy,vx,vy;
	Goto _nextGoto;
	Goto _goto;
};
s_Treffer _treffer;
void Flugschleife()
{
	double x,y,ox,oy,vx,vy;
	short v,c,a,j;
	
	x = _flugschleife.x;
	y = _flugschleife.y;
	ox = _flugschleife.ox;
	oy = _flugschleife.oy;
	vx = _flugschleife.vx;
	vy = _flugschleife.vy;
	v = _flugschleife.v;
	c = _flugschleife.c;

	if (x > 3 && x < 637 && y < 396 && (v > 0 || c > 0))
	{
		ox=x; oy=y; x+=vx; y-=vy;
		vy-=G; vx+=(wnd/2-vx)/5000;
		kugel( (int)ox,(int)oy );
		if( kugel( (int)x,(int)y ) )
			v=loc((int)x,(int)y)& loc((int)x-1,(int)y+1)& loc((int)x+1,(int)y+2);
			a=1000+2*y; if( a<30 ) a=30;
			//_sound->Test3(f);
		//Debug.WriteLine(f);
		////f = 440+900f;
			//_sound->Test(f);
		//_sound->GenerateSquare(f);
		//while (_instance.PendingBufferCount < 3)
		//{
		//	_instance.SubmitBuffer(_xnaBuffer);
		//}
			 //_sound->Write(a);
		Giaccess( 10,137 );    Giaccess( 244,135 );
		Giaccess( a&255,130 ); Giaccess( a>>8,131 );
		if (c > 0) c--;

		_flugschleife.x = x;
		_flugschleife.y = y;
		_flugschleife.ox = ox;
		_flugschleife.oy = oy;
		_flugschleife.vx = vx;
		_flugschleife.vy = vy;
		_flugschleife.v = v;
		_flugschleife.c = c;

		//g.Dispose();
		//RenderObj.SetValues(x, y, ox, oy, vx, vy, v, c, a, j);
		return;
	}
	//			_sound->Stop();
	render_callback = NULL;

	kugel( (int)x,(int)y );
	vswr_mode( handle,1 ); color( 0 );
	oldn=n;
	a = 0;
	j = 0;
	_treffer.c = c;
	_treffer.v = v;
	_treffer.ox = ox;
	_treffer.oy = oy;
	_treffer.a = a;
	_treffer.j = j;
	_treffer.vx = vx;
	_treffer.vy = vy;
	Treffer();
}

void Treffer()
{
	short v,c,a,j;
	double x,y,ox, oy,vx,vy;
	v = _treffer.v;
	c = _treffer.c;
	ox = _treffer.ox;
	oy = _treffer.oy;
	a = _treffer.a;
	x = _treffer.x;
	y = _treffer.y;
	j = _treffer.j;
	vx = _treffer.vx;
	vy = _treffer.vy;

	if(_treffer._goto == Kanone) goto gKanone;
	else if(_treffer._goto == Foerderturm) goto gFoerderturm;
	else if(_treffer._goto == Koenig) goto gKoenig;
	else if(_treffer._goto == Koenig2) goto gKoenig2;
	else if(_treffer._goto == GPK) goto gGPK;


   if( !v ) for( c=0;c<4;c++ )      /* Einschlag der Kugel */
   {
      if( ox>6 && ox<634 ) v_circle( handle, (int)ox,(int)oy, 5 );

      baller(22+c*3);

      for( n=0;n<2;n++ )    /* Treffer ? */
      {
         bg=burgen[bur[n]];
         if( Random()&1 && (n? x>639-bg[0] : x<bg[0]) && vo[n] ) vo[n]--;
         fn();
         for( j=0;j<10;j++ )
            if( ka[n][j].x<ox+2 && ka[n][j].x+22>ox && ka[n][j].x>-1 &&
                ka[n][j].y-16<oy && ka[n][j].y>oy )
            {
				// Kanone getroffen
               clr( a=ka[n][j].x,v=ka[n][j].y-12,20,13 );
			   _treffer._nextGoto = Kanone;
			   ka[n][j].x=-1;
               expls( a+10,v+6,13,8,50 );
			   _treffer.v=v;_treffer.c=c;_treffer.ox=ox;_treffer.oy=oy;_treffer.a=a;_treffer.x=x;_treffer.y=y;_treffer.j=j;_treffer.vx=vx;_treffer.vy=vy;
			   return;
			   gKanone:
			   color(0);			  
            }
         for( j=0;j<5;j++ )
            if( ft[n][j].x>-1 && drin( a=639*n+f*ft[n][j].x,v=by[n]-ft[n][j].y,
                30,37,2,(int)ox,(int)oy ) )
            {
               if( drin( a,v,30,37,-2,(int)ox,(int)oy ) )
               {
				   // Fˆrderturm getroffen
				   _treffer._nextGoto = Foerderturm;
                  expls((a=ft[n][j].x-29*n)+15,(v=ft[n][j].y-40)+20,15,20,120);
				  _treffer.v=v;_treffer.c=c;_treffer.ox=ox;_treffer.oy=oy;_treffer.a=a;_treffer.x=x;_treffer.y=y;_treffer.j=j;_treffer.vx=vx;_treffer.vy=vy;
				  return;
				  gFoerderturm:
                  clr( a,v,30,40 ); ft[n][j].x=-1;
               }
               c=4;
            }
         if( ox>wx[n]-11 && ox<wx[n]+11 && oy>wy[n]-16 && oy<wy[n] && wx[n]>-1)
            { clr( wx[n]-10,wy[n]-15,20,15 ); wx[n]=-1; }
         if( drin( bg[21],bg[22],30,25,0,(int)ox,(int)oy ) )
         {
			 // Kˆnig getroffen
            end=n+17;
			_treffer._nextGoto = Koenig;
            expls( a=639*n+f*(bg[21]+15),v=by[n]-bg[22]-12,17,17,40 );
			_treffer.v=v;_treffer.c=c;_treffer.ox=ox;_treffer.oy=oy;_treffer.a=a;_treffer.x=x;_treffer.y=y;_treffer.j=j;_treffer.vx=vx;_treffer.vy=vy;
			return;
			gKoenig:
            v_circle( handle, a,v,17 );
			_treffer._nextGoto = Koenig2;
            expls( a,v,17,17,200 );
			_treffer.v=v;_treffer.c=c;_treffer.ox=ox;_treffer.oy=oy;_treffer.a=a;_treffer.x=x;_treffer.y=y;_treffer.j=j;_treffer.vx=vx;_treffer.vy=vy;
			return;
			gKoenig2:
			c=4;
         }
         for( j=0;j<6;j+=2 )
            if( drin( bg[25+j],bg[26+j],a=bg[31+j],v=bg[32+j],3,
                (int)ox,(int)oy ) ) break;
         switch( j )
         {
            case 0: ge[n]-=Min(200,ge[n]); break;
            case 2: pu[n]=0; break;
            case 4: ku[n]-=Min(2,ku[n]);
         }
         if( j<6 )
         {
			 // Geld, Kugeln, Pulver getroffen
			 _treffer._nextGoto = GPK;
            expls( 639*n+f*(bg[25+j]+a/2),by[n]-bg[26+j]-v/2,
                   a/2,v/2,60+100*(j==2) );
			_treffer.v=v;_treffer.c=c;_treffer.ox=ox;_treffer.oy=oy;_treffer.a=a;_treffer.x=x;_treffer.y=y;_treffer.j=j;_treffer.vx=vx;_treffer.vy=vy;
			return;
			gGPK:
            drw_gpk(j/2); c=4;
         }
      }
      ox+=vx; oy-=vy;
   }

   _treffer._goto = None;
   _treffer._nextGoto = None;
   color(0);
   n=oldn; fn();
   Giaccess( 0,137 );
   show();
   menu_enabled = true;
   ein_zug3();

}

struct s_expls {
	short d,x,y,w,h;
};
s_expls _expls;
extern short tempp;
/****************************** Explosion ************************************/
void expls( short x,short y,short w,short h,short d )
{
	short i,j;

	vswr_mode( handle,3 );
	for( i=0;i<32; ) { xy[i++]=x; xy[i++]=y; }
	_expls.d = d;
	_expls.x = x;
	_expls.y = y;
	_expls.w = w;
	_expls.h = h;

	render_callback = &expls2;
}

void expls2()
{
	short d,x,y,w,h;
	short i,j;
	d = _expls.d;
	x = _expls.x;
	y = _expls.y;
	w = _expls.w;
	h = _expls.h;

	for(int k = 0;k<2;k++)
	{
		if(d-->0)
		{
			_expls.d = d;
			v_pline( handle,2,xy );
			memmove(xy,xy+4,120);
			for( j=28;j<32; )
			{ xy[j++]=x-w+w*(Random()&511)/256; xy[j++]=y-h+h*(Random()&511)/256; }
			v_pline( handle,2,xy+28 );
			baller( d&31^31 );
		} else {
			render_callback = NULL;
			for( i=0;i<32;i+=4 ) v_pline( handle,2,xy+i );
			vswr_mode( handle,1 );
			_treffer._goto = _treffer._nextGoto;
			Treffer();
			break;
		}
	}
}

int kugel(short x, short y) {
	if( x<3 || x>637 || y<23 ) return(0);
	line( x-2,y-2,x+1,y-2 );
	line( x-3,y-1,x+2,y-1 );
	line( x-3,y  ,x+2,y   );
	line( x-3,y+1,x+2,y+1 );
	line( x-2,y+2,x+1,y+2 );
	return(1);
}

void baller(char r)
{
   static char s_bal[]={ 0,0,1,15,6,31,9,0,11,0,12,50,13,0,7,192,8,16,255,0 };
	//static char s_bal[]={ 0,0,  1,15,  6,31,  9,0,  11,0,  12,50,  13,10,  7,192,  8,16,  255,0 };

   s_bal[5]=r;

   char msgbuf[100];
   //sprintf_s(msgbuf, "%d\n", r);
	//OutputDebugStringA(msgbuf);
   Dosound( s_bal );
}

/**************************** Neues Bild zeichnen ****************************/
void bild()
{
   short y,x1,x2,v1,v2;

   hide();
   cls();

   by[0]=Random()%80+300&~3; by[1]=Random()%80+300&~3;

   y=400; x1=0; x2=2556; color( 1 ); v1=v2=2;
   vsl_type( handle,7 );
   while( x1<x2 && --y>20 )
   {
      vsl_udsty( handle,~((257<<(Random()&7))*(y&1)) );
      line( x1/4,y,x2/4,y );
      if( y==by[0] ) x1=*burgen[bur[0]]*4;
      if( y==by[1] ) x2=639-*burgen[bur[1]]<<2;
      if( x1 )
         { v1=v1+Random()%5-2; v1=Max(0,v1); v1=Min(7,v1); x1+=v1; }
      if( x2<2556 )
         { v2=v2+Random()%5-2; v2=Max(0,v2); v2=Min(7,v2); x2-=v2; }
   }
   vsl_type( handle,1 );
   memset(ka,-1,160);
   burg(0); burg(1);
   v_gtext(handle,276,395," Runde     ");
   show();
}

/***************************** Burg zeichnen *********************************/
void burg(short nn)
{
   short i,xr;

   oldn=n; n=nn; fn();
   xr=n&2? bx[n&=1]:639*n;
   bg=burgen[bur[n]];
   ge[n]=bg[37]; pu[n]=bg[38]; ku[n]=bg[39]; vo[n]=bg[40];

   color(1);
   vswr_mode( handle,4 ); draw( xr,by[n], &bg[45] );
   vswr_mode( handle,1 );
   clr( xr+f*bg[21]-n*30,by[n]-bg[22]-25,30,25 );
   color(1); draw( xr+f*bg[21],by[n]-bg[22], trohn );

   n=nn;
   for( i=0; i<10 && bg[i*2+1]>-1; i++ ) init_ka( i,xr );
   drw_all();
   n=oldn;
}

void init_ka(short k,short xr)    /* Kanone k setzen */
{
   short x,y;

   draw( x=xr+bg[1+k*2]*f, y=by[n&1]-bg[2+k*2], kanon );
   if( ~n&2 )
     { ka[n][k].x=x-20*n; ka[n][k].y=y; ka[n][k].w=45; ka[n][k].p=12; }
}

void drw_all()  /* Geld, Pulver und Kugeln zeichnen */
{
	drw_gpk(0); drw_gpk(1); drw_gpk(2);
}

void drw_gpk( char w )
{
	short i,z, x,y, xr,yr, *a,xp,yp, m=n&1;

	a = NULL;
	switch( w )
	{
	case 0: a=sack;  xp=7; yp=10;  i=(ge[m]+149)/150; break;
	case 1: a=fass;  xp=yp=9;      i=(pu[m]+29)/30;  break;
	case 2: a=kuge;  xp=yp=6;      i=ku[m];
	}
	bg=burgen[bur[m]]; w*=2;
	xr=(n&2? bx[m]:639*m)+f*bg[25+w]; yr=by[m]-bg[26+w];

	hide();
	clr( xr-bg[31+w]*m-!n,yr-bg[32+w],bg[31+w]+1,bg[32+w]+1 );
	color(1);

	for( y=z=0; i>0 && y<bg[32+w]; y+=yp )
		for( x=0; i>0 && x<bg[31+w]; x+=xp,z++,i-- )
			draw( xr+f*x,yr-y, a );
	show();
	if( i>0 )    /* Maximalbetrag ¸berschritten ? */
		switch( w )
	{
		case 0: ge[m]=z*150; break;
		case 2: pu[m]=z*30;  break;
		case 4: ku[m]=z;
	}
}

/***************************** Zeichenroutine ********************************/
void draw( short x,short y,short *a )
{
   short i,fil=1;

   hide();
   vsf_interior( handle,2 ); vsf_style( handle,9 );
   while( *a!=-1 )
   {
      if( *a==-2 )
         { vsf_interior( handle,*++a ); vsf_style( handle,*++a ); }
      else if( *a==-4 ) fil=!fil;
      else
      {
         i=0;
         while( *a>-1 )
            { xy[i++]=x+*a++*f; xy[i++]=y-*a++; }
         xy[i++]=xy[0]; xy[i++]=xy[1];
		 if( fil ) {v_fillarea( handle,i/2,xy );
		 }
		 else {v_pline( handle,i/2-1,xy );}
      }
      a++;
   }
   show();
}

s_markt2 _markt2;
/******************************** Markt **************************************/
void markt()
{
   form_center( a_sta,&fx,&fy,&fw,&fh );
   sav_scr();
   form_dial( 1,mx,my,30,20,fx,fy,fw,fh );
   markt2();
}

void markt2()
{
	short a,k,ko,t;

	for( a=0;a<6;a++ ) zahl( SM1+a, p[a] );

	for( t=k=0;k<5; ) t+=(ft[n][k++].x>-1);
	for( ko=k=0;k<10; ) ko+=(ka[n][k++].x>-1);
	for( k=0;k<10;k++ )
		if( bg[1+k*2]>-1 && ka[n][k].x==-1 )
		{
			hide(); for( a=1;a<10;a++ )
				if( loc(639*n+(bg[1+k*2]+5+a)*f,by[n]-bg[2+k*2]-a) ) break;
			show(); if( a>9 ) break;
		}
		zahl( SH1,ge[n] ); zahl( SH2,t );
		zahl( SH3,ko    ); zahl( SH4,wx[n]>-1 );
		zahl( SH5,pu[n] ); zahl( SH6,ku[n] );
		zahl( SH7,vo[n] ); zahl( SH8,st[n] ); 
		for( a=0;a<6;a++ )
			*(short *)(a_sta+24*(SM1+a)+10)=8* ( ge[n]<p[a] || !an_erl && !a ||
			a==1 && (bg[0]+t*30>265||t>4) || a==2 && k>9 || a==3 && wx[n]>-1 );

		objc_draw( a_sta,0,5,0,0,640,400 );
		_markt2.k = k;
		callback = &markt3;
}

void markt3()
{
	short a, k;
	k = _markt2.k;
	callback = NULL;
	// TODO
      a=form_do( a_sta,0 ); //if( a>=0 ) *(short *)(a_sta+24*a+10)=0;
      if( a==SHK ) st[n]-=2*(st[n]>0);
      else if( a==SHG ) st[n]+=2*(st[n]<100);
      else if( a!=FERTIG && a-SM1>-1 && a-SM1<6)
      {
         ge[n]-=p[a-SM1];
         if( a<SM5 )
         {
            lod_scr(); drw_all();
			if( a==SM1 ) {anbau(); return;}
            else if( a==SM2 ) fturm();
            else if( a==SM3 ) init_ka( k,639*n );
            else if( a==SM4 )
               { wx[n]=639*n+f*bg[23]; wy[n]=by[n]-bg[24]; werdran(1); }
            sav_scr();
         }
         else
           {pu[n]+=30*(a==SM5); ku[n]+=2*(a==SM6); drw_gpk(a-SM4); drw_gpk(0);}
      }
	  if(a!=FERTIG)
	  {
		  markt2();
		  return;
	  }
   //} while( false /*a!=FERTIG */);

   lod_scr(); drw_all();
   // TODO wenn hier der Dialog geschlossen wird, ¸berschreibt es den gespeicherten Screen.
   //form_dial( 2,mx,my,30,20,fx,fy,fw,fh );
   menu_enabled = true;
   callback = &ein_zug1;
}

void zahl( short nr, short wert ) /* 5-stellige Zahl, rechtsbÅndig, ohne f¸hrende Nullen */
{
   short i,a,b;
   char *adr;

   adr=*(char **)(a_sta+24*nr+12)+11;
   for( b=i=0,a=10000; i<5; i++,a/=10 )
     { *adr++=48+wert/a-16*(wert<a && i<4 && !b); b|=wert/a; wert%=a; }
}

/********************* Von Markt aufzurufende Routinen ***********************/
void fturm()  /* Fˆrderturm bauen */
{
   short x,y,yy,i,t;

   hide();
   for( t=0;t<5;t++ ) if( ft[n][t].x==-1 ) break;
   x=639*n+f*(bg[0]+20+30*t); y=380;
   do
   {
      while( loc(x,y) && loc(x+29*f,y--) );
      yy=y;
      for( i=0;i<40;i++,y-- ) if( loc(x,y) && loc(x+29*f,y) ) break;
   } while( i<40 );
   y=yy;
   clr( x-29*n,y-70,30,70 );
   color( 1 ); draw( ft[n][t].x=x,ft[n][t].y=y,turm );
   show();
}

struct s_anbau
{
	short s;
	byte* screen;
};
s_anbau _anbau;
void anbau()  /* Anbauen */
{
	short s;
	_anbau.screen = new byte[240*40];
	handle->SaveRectangle(200,360,440,400,_anbau.screen);

	color(1); vsf_interior( handle,2 ); vsf_style( handle,9 ); 
	v_gtext( handle,280,375," Anbauen: " );
	v_gtext( handle,220,395," Verbleibende Steine: 20 "); s=20;
	graf_mouse(6,0);

	_anbau.s = s;
	mouse_move_callback = &anbau2;
}

void anbau2()
{
	short s;
	char a[3];
	s = _anbau.s;
	//do
	//{
		maus();
		if( bt && (n? mx>624-bg[0] : mx<bg[0]+15 ) && my>155 )
		{
			hide();
			if( !( loc(mx,my) || loc(mx+1,my+1) || loc(mx-1,my-1) ) &&
				( loc(mx+3,my-1)||loc(mx+3,my+1)||loc(mx-3,my-1)||loc(mx-3,my+1)||
				loc(mx+1,my+2)||loc(mx-1,my+2)||loc(mx+1,my-2)||loc(mx-1,my-2) ))
			{
				vswr_mode( handle,4 );
				xy[0]=mx-2; xy[1]=my-1; xy[2]=mx+2; xy[3]=my+1;
				v_bar( handle,xy );
				vswr_mode( handle,1 );
				s--;
				a[0]=48+s/10; a[1]=48+s%10; a[2]=0; v_gtext( handle,396,395,a );
			}
			show();
		}
		

	//} while( s>0 && bt<2 );

	graf_mouse(0,0);
	_anbau.s = s;
	if(!(s > 0))
		{
			//for( s=360;s<400;s++ ) memmove( scr+s*80+26,buf+s*80+26,28 );
			//handle->RestoreRectangle(200, 360, 440, 399, _buf_scr+((360*640+200)));
			handle->RestoreRectangle(200,360,440,400,_anbau.screen);
			mouse_move_callback = NULL;
			sav_scr();
			markt2();
		}
}

/*************************** Audienz beim Kˆnig ******************************/
char kna[]="[0][Der Kˆnig meint:             |'",
     kne[]="'|][Dem¸tig zur Kenntnis genommen]",
     kn0[]="Naja...| Nun gut...| Weiter so...",
     kn1[]="Ich bin zufrieden| mit Ihren Leistungen!",
     kn2[]="Hervorragend,| Weiter so!",
     kn3[]="Vielleicht sollten Sie mal| die Steuern senken...",
     kn4[]="Wenn Sie so weiter machen| werde ich Sie entlassen!",
     kn5[]="Vielleicht mal 'nen| Fˆrderturm kaufen...",
     kn6[]="Sie sollten sich| gef‰lligst mehr M¸he| geben!",
     kn7[]="Sie brauchen nicht| jede Runde zu kommen.",
     kn8[]="Wissen Sie eigentlich,| dass Sie mich bereits| xxmal besucht haben?",
     kn9[]="Und Sie sind sich sicher,| dass Sie auch ohne eine| Windfahne zurecht kommen?",
    kn10[]="Schˆn, Sie zu sehen...",
    kn11[]="Was soll ich denn in| so einer fr¸hen Phase| schon sagen?",
    kn12[]="Sie sollten mehr Geld| verdienen, Fˆrdert¸rme bauen| und den Gegner besiegen.",
    kn13[]="Ich habe Ihnen nichts| neues zu sagen.",
    kn14[]="Find' ich nett, dass| Sie mich mal besuchen!";

void koenig()
{
	callback = NULL;
   char a[300],*s[20],k,t,j,i;
   static char *ltz[2];

   for( j=k=0;j<10;j++ ) k+=ka[n][j].x>-1;
   for( j=t=0;j<5;j++ )  t+=ft[n][j].x>-1;

   if( !(Random()%20) || kn[n]&16 || (kn[n]&15)>9 )
      form_alert(1,"[0][Der Kˆnig hat keine Lust,   |dich zu sprechen.][Schade]");
   else
   {
      kn8[46]=kn[n]>2559? 48+kn[n]/2560:32; kn8[47]=48+kn[n]%2560/256;

      j=Random(); i=2;
      s[0]=j&1? kn0:kn10; s[1]=j&2? kn12:kn6; if( j&4 ) s[i++]=kn14;

      if( ge[n]>p[1]&&t<3 ) s[i++]=kn5;
      if( st[n]>40&&vo[n]<bg[40]||st[n]>70 ) s[i++]=kn3;
      if( wx[n]<0 ) s[i++]=kn9;
      if( ge[n]>bg[37]&&vo[n]>bg[40]&&k>1 ) s[1]=t<2? kn1:kn2;
      if( t>2 ) s[i++]=kn2;
      if( k<1 && ge[n]<p[2] ) s[i++]=kn4;
      if( (kn[n]&15)>4 ) s[i++]=s[i++]=s[i++]=kn7;
      if( (kn[n]&15)>6 || !(Random()&7) && kn[n]>2048 ) s[i++]=s[i++]=kn8;
      if( zug<4 ) s[1]=s[2]=s[(i=4)-1]=kn11;
      do
         if( s[j=Random()%i]==ltz[n] ) s[i++]=s[i++]=kn13;
      while( s[j]==ltz[n] );

      //strcpy( a,kna ); strcat( a,ltz[n]=s[j] ); strcat( a,kne );
	  int c = 0;
	  int p = 0;
	  char ch = kna[c++];
	  while(ch != NULL) {
		  a[p++] = ch;
		  ch = kna[c++];
	  }
	  c = 0;
	  ch = s[j][c++];
	  while(ch != NULL) {
		  a[p++] = ch;
		  ch = s[j][c++];
	  }
	  c = 0;
	  ch = kne[c++];
	  while(ch != NULL) {
		  a[p++] = ch;
		  ch = kne[c++];
	  }
	  a[p++] = NULL;

      form_alert(1,a);
   }
   callback = &koenig2;
}

void koenig2()
{
	if (!drin_(mx, my, res_form->x+res_form->bx, res_form->y+res_form->by, res_form->bw, res_form->bh )) {
        return;
    }
	callback = &ein_zug1;
	menu_enabled = true;
	handle->RestoreRectangle(res_form->x, res_form->y, res_form->x+res_form->w, res_form->y+res_form->h, _form_dial_screen);
   kn[n]|=16; kn[n]+=256; if( ~kn[n]&15 ) kn[n]++;
}

bool drin( short xk,short yk,short w,short h,short r,short x,short y ) /* Test, ob Koord. innerhalb eines Rechteckes */
{
   if( n ) xk=639-xk-w;
   return( x>xk-r && x<xk+w+r && y<by[n]-yk+r && y>by[n]-yk-h-r );
}



