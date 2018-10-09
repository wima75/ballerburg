#include <pch.h>
#include <Atari.h>
#include <Baller.h>
#include <Baller1.h>
#include <Baller2.h>
#include <Sound.h>
#include <iostream>
#include <fstream>

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Concurrency;

#define Min(a,b)  ((a)<(b)?(a):(b))
#define Max(a,b)  ((a)>(b)?(a):(b))
//#define maus()    graf_mkstate(&mx,&my,&bt,&dum)
//#define menu(a)   wind_update(3-a)
#define hide()    graf_mouse(256,0)
#define show()    graf_mouse(257,0)
//#define sav_scr() hide();movmem(scr,buf,32000);show()
//#define lod_scr() hide();movmem(buf,scr,32000);show()
#define sav_scr() hide();handle->SaveRectangle(0, 0, 640, 400, _buf_scr);show();
#define lod_scr() hide();handle->RestoreRectangle(0, 0, 640, 400, _buf_scr);show();
#define PI        3.1416
#define P57       57.296
#define G         0.02
//#define bing()    printf("\007");
#define fn()      f=1-2*(n&1);

bool _tab_exists;

extern byte* _form_dial_screen;
extern ressource_form* res_form;
extern bool menu_enabled;

double vvx,vvy;
WriteableBitmapEx* handle;
short mx,my,bt,dum,m_buf[8], xy[100], 
      bur[2],bx[2],by[2], ge[2],pu[2],ku[2],vo[2],st[2],kn[2],
      wx[2],wy[2],ws,wc,
      *bg, zug,n,oldn, p[6],  t_gew[6][10], max_rund,
      fx,fy,fw,fh,
      ftx,fty,ftw,fth,
      *burgen[20],b_anz;
HANDLE f_h;
int   buf, scr, l_nam,r_nam,  a_opt, a_ein,
      a_men,a_inf,a_sch,a_brg,a_nam,a_dra,a_sta,a_sie,a_com,a_re1,a_re2,a_re3;
short * bur_ad;
char  f, mod, wnd, end, txt[4], an_erl, mxin, au_kap,
      cw[2]={2,2}, cx[2]={1,1}, cn[7][8]={ "Tˆlpel","Dummel","Brubbel",
      "Wusel","Brˆsel","Toffel","R¸Åpel" },
      t_na[6][8]={ "Tˆlpel","Dummel","Brubbel","Wusel","Brˆsel","Toffel" },
      nsp1[22]="Hugo",nsp2[22]="Emil";
	  //nsp1[22]="Tˆlpel",nsp2[22]="Wusel";
struct { short x,y,w,p; } ka[2][10];
struct { short x,y; } ft[2][5];

Sound^ _sound;
Audio           m_audio;
extern int _vsf_interior;
extern int _vsf_style;

byte* _buf_scr = new byte[640*400];

void (*callback)();
void (*render_callback)();
bool _wait = false;

void (*obj_do_callback)();
struct s_obj_do {
	int adr;
	short x, fl;
	int result;
};
s_obj_do _obj_do;

void main1()
{
	short i;

	//bur[0] = 3;bur[1] = 4;

	rsrc_gaddr( 0,MENUE, &a_men); rsrc_gaddr( 0,INFOTREE,&a_inf );
	rsrc_gaddr( 0,SCHUSS,&a_sch ); rsrc_gaddr( 0,BURG,&a_brg );
	rsrc_gaddr( 0,NAMEN,&a_nam  ); rsrc_gaddr( 0,DRAN,&a_dra );
	rsrc_gaddr( 0,STATUS,&a_sta ); rsrc_gaddr( 0,SIEGER,&a_sie );
	rsrc_gaddr( 0,COMPUTER,&a_com); rsrc_gaddr( 0,REGEL1,&a_re1 );
	rsrc_gaddr( 0,REGEL2,&a_re2 );  rsrc_gaddr( 0,REGEL3,&a_re3 );
	rsrc_gaddr( 0,OPTION,&a_opt ); rsrc_gaddr( 0,EINTRAG,&a_ein );

	*(short *)(a_com+CN1*24+58)=1;  *(short *)(a_com+CN2*24+58)=1;
	*(short *)(a_com+CTS1*24+34)=1; *(short *)(a_com+CTS2*24+34)=1;

	short *a;
	//int *xxx;
	//xxx = (int *)(a_dra+60);

	//a=(short *)(a_dra+16);

	// TODO (8000 reicht wohl);
	bur_ad = new short[42000];
	/*for(int i = 0;i<20;i++) {
		burgen[i] = new short[1000];
	}*/
	//bur_ad = (short *)malloc(42000);  /* Speicher f¸r Burgdaten und zweiten Schirm holen */
	//bur_ad=buf+32000;
		an_erl=1;
		max_rund=32767;
		mxin=3;

		au_kap=1; t_load().then([=](){
		burgen_laden();

		menu_bar(a_men,1);

		l_nam=(int)nsp1; r_nam=(int)nsp2; mod=0;
		neues();ein_zug();

		/*vsf_style(handle, 9);
		vswr_mode( handle,4 );
		vsf_interior(handle, 2);
		xy[0]=0; xy[1]=0; xy[2]=200; xy[3]=50;
		v_bar( handle,xy );*/
		/*xy[0]=0; xy[1]=0; xy[2]=200; xy[3]=0;xy[4]=200;xy[5]=50;xy[6]=0;xy[7]=50;;
		v_fillarea(handle, 4, xy);
		xy[0]=3; xy[1]=55; xy[2]=200; xy[3]=55;xy[4]=200;xy[5]=70;xy[6]=3;xy[7]=70;;
		v_fillarea(handle, 4, xy);*/
	});

	/*color(1);
    _vsf_interior = 2;
    _vsf_style = 2;
    xy[0] = 10;
    xy[1] = 10;
    xy[2] = 100;
    xy[3] = 10;
    xy[4] = 100;
    xy[5] = 50;
    xy[6] = 10;
    xy[7] = 50;
    i = 8;
    v_pline(handle, i / 2, xy);*/

}

struct s_regeln
{
	int c;
	short a;
};
s_regeln _regeln;
/* Verwaltung von Ereignissen: Messages, Maus oder Timer */ 
void event1()
{
	callback = NULL;

	short wh;
	// TODO
	if( m_buf[4]==INFO ) obj_do( a_inf );
	else if( m_buf[4]==SPI1 )     /* Neues Spiel */
	{
		//if( bur_obj() ) { neues(); werdran(1); }
		bur_obj();
	}
	else if( m_buf[4]==SPI2 )     /* Namen eingeben */
	{
         /*movmem( nsp1, **(int **)(a_nam+NSP1*24+12), 20 );
         movmem( nsp2, **(int **)(a_nam+NSP2*24+12), 20 );
         if( obj_do( a_nam )==NOK )
         {
            movmem( **(int **)(a_nam+NSP1*24+12), nsp1, 20 );
            movmem( **(int **)(a_nam+NSP2*24+12), nsp2, 20 );
            if( !end ) werdran(1);
         }*/
		*(char **)(a_nam+NSP1*24+12)=nsp1;
		*(char **)(a_nam+NSP2*24+12)=nsp2;
		obj_do_callback = &namen;
		obj_do(a_nam);
		return;
	}
	else if( m_buf[4]==SPI4 ) /* Optionen */
	{
		*(short*)(a_opt+A_ERL*24+10)=an_erl;
		*(short*)(a_opt+A_VER*24+10)=!an_erl;
		*(short*)(a_opt+MAX_1*24+10)=!mxin;
		*(short*)(a_opt+MAX_2*24+10)=(mxin==1);
		*(short*)(a_opt+MAX_XX*24+10)=(mxin==2);
		*(short*)(a_opt+MAX_NE*24+10)=(mxin==3);
		*(short*)(a_opt+A_KAP*24+10)=au_kap;
		obj_do_callback = &optionen;
		obj_do(a_opt);
		return;
	}
	//else if( m_buf[4]==SPI3 ) return( 1 );
	// TODO
	else if( m_buf[4]>=MOD1 && m_buf[4]<=MOD4 )
	{                          /* Spielmodus ( wer gegen wen ) */
		menu_icheck( a_men,MOD1+mod,0 );
		mod=m_buf[4]-MOD1;
		menu_icheck( a_men,m_buf[4],1 );
		//ein_zug1();
	}
	else if( m_buf[4]==MOD5 )
	{                       /* Computer ausw‰hlen */
		obj_do_callback = &computer_gewaehlt;
		obj_do( a_com );
		return;
	}
	else if( m_buf[4]==REG )     /* Spielregeln */
	{
		short a=0,b;
		int c;
		form_center( a_re1,&fx,&fy,&fw,&fh );
		form_center( a_re2,&fx,&fy,&fw,&fh );
		form_center( a_re3,&fx,&fy,&fw,&fh );
		sav_scr();
         form_dial( 1,190,20,30,20,fx,fy,fw,fh );

            objc_draw( c=a? (a<2? a_re2:a_re3):a_re1, 0,5,0,0,640,400 );
			_regeln.c = c;
			_regeln.a = a;
			callback = &regeln;
            return;
	}
	else if( m_buf[4]==ERG1 )
	{
		memmove(*(char **)(a_ein+EI_N1*24+12),t_na[0],8);
		memmove(*(char **)(a_ein+EI_N2*24+12),t_na[1],8);
		memmove(*(char **)(a_ein+EI_N3*24+12),t_na[2],8);
		memmove(*(char **)(a_ein+EI_N4*24+12),t_na[3],8);
		memmove(*(char **)(a_ein+EI_N5*24+12),t_na[4],8);
		memmove(*(char **)(a_ein+EI_N6*24+12),t_na[5],8);
		*(short*)(a_ein+24*ER_LOE+10)=0; *(short*)(a_ein+24*ER_BLE+10)=1;
		obj_do_callback = &eintrag;
		obj_do(a_ein);
		return;
	}
	else if( m_buf[4]==ERG2 )
	{
		//if( t_load() )
		//	form_alert(1,"[1][Kann BALLER.TAB nicht laden!][Abbruch]" );
		t_load();
	}
	else if( m_buf[4]==ERG3 ) t_save();
	else if( m_buf[4]==ERG4 )
	{
		sav_scr();
		form_dial( 1,260,20,30,20,48,52,548,308 );
		tabelle(); bt=0; callback = &tabelle2;
		return;
	}
	if( m_buf[4]>=MOD1 && m_buf[4]<=MOD5 )
      {
         if( mod<2 ) l_nam=(int)nsp1; else l_nam=(int)cn[cw[0]];
         if( mod&1 ) r_nam=(int)cn[cw[1]]; else r_nam=(int)nsp2;
         if( !end ) werdran(1);
		 ein_zug1();
		 callback = &ein_zug1;
      }
}

void optionen()
{
	int a;
	int adr;
	short x, fl;
	adr = _obj_do.adr;
	fl = _obj_do.fl;
	x = _obj_do.x;
	a = OP_OK;
		callback = &ein_zug1;
		_obj_do.result = a;
		*(short *)(adr+24*a+10)=0;
		lod_scr(); form_dial( 2,x,20,30,20,fx,fy,fw,fh );

		obj_do_callback = NULL;


	char *h;
	au_kap=*(short*)(a_opt+A_KAP*24+10);
	an_erl=*(short*)(a_opt+A_ERL*24+10);
	mxin=*(short*)(a_opt+MAX_2*24+10)+
		2**(short*)(a_opt+MAX_XX*24+10)+
		3**(short*)(a_opt+MAX_NE*24+10);
	h=(char *)(**(int **)(a_opt+MAX_XX*24+12));
	max_rund=h[0]-48;
	if(h[1])
	{ max_rund=max_rund*10+h[1]-48; if(h[2])
	max_rund=max_rund*10+h[2]-48; }
	if(!mxin) max_rund=20; if(mxin==1) max_rund=50;
	if(mxin==3) max_rund=32767;
}

extern enm_feedback namen_feedback;
void namen()
{
	obj_do_callback = NULL;
	if(_obj_do.result == NOK)
	{
		namen_feedback = fbOK;
	} else {
		namen_feedback = fbAbbrechen;
	}
}

void namen2()
{
	if( !end ) werdran(1);
}

void computer_gewaehlt()
{
	obj_do_callback = NULL;
	for( cw[0]=0; cw[0]<7; cw[0]++ )
		if( *(short *)(a_com+24*(CN1+cw[0])+10) ) break;
	for( cw[1]=0; cw[1]<7; cw[1]++ )
		if( *(short *)(a_com+24*(CN2+cw[1])+10) ) break;
	for( cx[0]=0; cx[0]<4; cx[0]++ )
		if( *(short *)(a_com+24*(CTS1+cx[0])+10) ) break;
	for( cx[1]=0; cx[1]<4; cx[1]++ )
		if( *(short *)(a_com+24*(CTS2+cx[1])+10) ) break;
}

extern enm_feedback eintrag_feedback;
void eintrag()
{
	obj_do_callback = NULL;
	if(_obj_do.result == ER_OK)
	{
		eintrag_feedback = fbOK;
	} else {
		eintrag_feedback = fbAbbrechen;
	}
}

void eintrag2() {
	short i,j;
	/*memmove(t_na[0],*(char **)(a_ein+EI_N1*24+12),8);
	memmove(t_na[1],*(char **)(a_ein+EI_N2*24+12),8);
	memmove(t_na[2],*(char **)(a_ein+EI_N3*24+12),8);
	memmove(t_na[3],*(char **)(a_ein+EI_N4*24+12),8);
	memmove(t_na[4],*(char **)(a_ein+EI_N5*24+12),8);
	memmove(t_na[5],*(char **)(a_ein+EI_N6*24+12),8);*/
	if(*(short*)(a_ein+24*ER_LOE+10))
		for(i=0;i<6;i++) for(j=0;j<10;j++) t_gew[i][j]=0;

}

void regeln()
{
	short b;
	int c = _regeln.c;
	short a = _regeln.a;
	b=form_do( c,0 );
	if(b < 0) return;

	*(short *)(c+24*b+10)=0;
	if( a==1 && b==R21 || a==2 && b==R31 ) { b=-1; a=0; }
	if( a==0 && b==R12 || a==2 && b==R32 ) { b=-1; a=1; }
	if( a==0 && b==R13 || a==1 && b==R23 ) { b=-1; a=2; }
	if(b<0) {
		objc_draw( c=a? (a<2? a_re2:a_re3):a_re1, 0,5,0,0,640,400 );
		_regeln.a = a;
		_regeln.c = c;
		return;
	}
	callback = &ein_zug1;
	lod_scr();
	form_dial( 2,190,20,30,20,fx,fy,fw,fh );
}

void event2(bool bur_obj_result)
{
	if(bur_obj_result) { neues(); werdran(1); }

	callback = &ein_zug1;
}

/**************************** Tabelle ****************************************/
void tabelle()
{
   short i,j;
   hide(); vsf_interior(handle,0);
   box(53,56,587,343,1); box(55,58,585,341,1); box(56,59,584,340,1);
   line(144,59,144,340); for(i=152;i<584;i+=72) line(i,59,i,340);
   line(56,84,584,84); for(i=92;i<240;i+=24) line(56,i,584,i);
   for(i=244;i<340;i+=24) line(56,i,584,i);
   for(i=0;i<6;i++) v_gtext(handle,160+i*72,78,t_na[i]);
   for(i=0;i<6;i++) v_gtext(handle,80,110+i*24,t_na[i]);
   vsf_interior(handle,2); vsf_style(handle,2);
   for(i=0;i<6;i++) for(j=0;j<10;j++)
   {
      z_txt(t_gew[i][j]);
      if(j==9 && !t_gew[i][6] ) { txt[0]=32; txt[1]='-'; txt[2]=0; }
      v_gtext(handle,176+i*72,110+j*24+8*(j>5),txt);
      if(i==j) box(152+i*72,92+j*24,224+i*72,116+j*24,1);
   }
   v_gtext(handle,64,262,"{  Spiele"); v_gtext(handle,64,286,"{gewonnen");
   v_gtext(handle,64,310,"{verloren"); v_gtext(handle,64,334,"Siege in%");
   line(56,59,144,84); vst_height(handle,4,&i,&i,&i,&i);
   v_gtext(handle,60,81,"VERLOREN"); v_gtext(handle,92,66,"GEWONNEN");
   vst_height(handle,13,&i,&i,&i,&i); show();
}

void tabelle2()
{
	callback = &ein_zug1;
	lod_scr();
	form_dial( 2,260,20,30,20,48,52,548,308 );
}

void z_txt(short a)
{
   txt[0]=a/100+48; txt[1]=a%100/10+48; txt[2]=a%10+48;
   if(a<100) { txt[0]=32; if(a<10) txt[1]=32; }
   for(a=0;a<3;a++) txt[a]=txt[a]==48? 79: txt[a]==49? 108: txt[a];
}

/******************** Initialisierung vor neuem Spiel ************************/
void neues()
{
   static short pr[6]={ 200,500,400,150,50,50 };   /* Preise zu Beginn */
   short j;

   wnd=Random()%60-30;
   st[0]=st[1]=20; kn[0]=kn[1]=0;
   for( j=0;j<6;j++ )
      p[j]=pr[j]*(95+Random()%11)/100;
   bild();
   for( n=0;n<2;n++ )
   {
      bg=burgen[bur[n]]; wx[n]=n? 639-bg[23]:bg[23]; wy[n]=by[n]-bg[24];
      for( f=0;f<5; ) ft[n][f++].x=-1;
   }
   zug=n=end=0; f=1;
}



//s_ein_zug1 _ein_zug1;

/************************* Durchf¸hren eines Zuges ***************************/
void ein_zug()
{
short i,fl,a;

   n=zug&1; fn(); kn[n]&=~16;
   wnd=wnd*9/10+Random()%12-6;

   werdran(1);
   // TODO

   bg=burgen[bur[n]];
   if( mod&(2-n) ) { hide(); i=comp(); show(); fl=1+(i<0); 
	ein_zug2(fl, i);
   }
   else {
	callback = &ein_zug1;
   }
}

//s_ein_zug2 _ein_zug2;

void ein_zug1() {
	short i,fl;
	fl=0;
	bg=burgen[bur[n]];
	i = -1;
	do
	{
		if( mod&(2-n) ) { hide(); i=comp(); show(); fl=1+(i<0); }
		else if( mx>ftx && mx<ftx+ftw && my>fty && my<fty+fth ) fl=2;
		else
		{
			 for( i=0;i<10;i++ )
				if( ka[n][i].x<=mx && ka[n][i].x+20>=mx && ka[n][i].x!=-1 &&
					ka[n][i].y>=my && ka[n][i].y-14<=my ) break;
			 if( i>9 )
			 {
				if( drin( bg[25],bg[26],bg[31],bg[32],0,mx,my ) ||
					drin( bg[27],bg[28],bg[33],bg[34],0,mx,my ) ||
					drin( bg[29],bg[30],bg[35],bg[36],0,mx,my ) ) markt();
				else if( drin( bg[21],bg[22],30,25,0,mx,my ) ) koenig();
				else bing();
			 }
			 else if( pu[n]<5 )
			  {
				  bing();
				  form_alert(0,"[0][Dein Pulver reicht nicht!][Abbruch]");
				callback = &ein_zug4;
				return;
			 }
			 else if( !ku[n] )
			  { 
				  bing();
				  form_alert(0,"[0][Du hast keine Kugeln mehr!][Abbruch]");
				  callback = &ein_zug4;
				  return;
			 }
			 else fl=sch_obj(i);
		}
	} while(false);
	if(!fl) return;
	callback = NULL;
	//_ein_zug2.i = i;
	ein_zug2(fl, i);
}

void ein_zug2(short fl, short i) {
	
	werdran(0);
	if( fl<2 ) {
		schuss(i);
	} else {
		ein_zug3();
	}
}

extern uint64 _tickCount;
void ein_zug3()
{
	short i,a;
	if( ~kn[n]&16 ) kn[n]&=~15;
	rechnen(); zug++;

	for( i=0;i<10;i++ ) if( ka[n][i].x>-1 ) break;
	n=zug&1; bg=burgen[bur[n]];
	for( a=0;a<10;a++ ) if( ka[n][a].x>-1 ) break;
	if( a==10 && i<10 && bg[40]>vo[n] && ge[n]<p[2]/3 && au_kap && 
		ft[n][0].x+ft[n][1].x+ft[n][2].x+ft[n][3].x+ft[n][4].x==-5 )
		end=n+33;

	if( !end && zug/2>=max_rund )
	{
		static int h[2];
		for(n=0;n<2;n++)
		{
			h[n]=ge[n]+pu[n]*p[4]/30+ku[n]*p[5]/2+(wx[n]>-1)*p[3]+vo[n]*4;
			for(i=0;i<5;i++) if( ft[n][i].x>-1 ) h[n]+=p[1];
			for(i=0;i<10;i++) if( ka[n][i].x>-1 ) h[n]+=p[2];
		}
		end=65+(h[1]<h[0]);
	}
	if( end )
	{
		ende();
		return;
		/*menu(1);
		do
		if( event() ) return(0);
		while( !bt || end );
		menu(0);*/
	}
   //return(1);
	if( mod&(2-n) ) {
		_tickCount = ::GetTickCount64();
		_wait = true;
	} else {
		ein_zug();
	}
}

void ein_zug4()
{
	if (!drin_(mx, my, res_form->x+res_form->bx, res_form->y+res_form->by, res_form->bw, res_form->bh )) {
        return;
    }
	callback = &ein_zug1;
	menu_enabled = true;
	handle->RestoreRectangle(res_form->x, res_form->y, res_form->x+res_form->w, res_form->y+res_form->h, _form_dial_screen);
}

/********** Berechnen von Bevˆlkerungszuwachs usw. nach jedem Zug ************/
void rechnen()
{
   short j;
   static short pmi[6]={ 98,347,302,102,30,29 },   /* Preisgrenzen */
                pma[6]={ 302,707,498,200,89,91 },
                psp[6]={ 10,50,50,20,10,10 };     /* max. Preisschwankung */

   j=st[n];
   ge[n]+=vo[n]*(j>65? 130-j:j)/(150-Random()%50);
   vo[n]=vo[n]*(95+Random()%11)/100+(21-j+Random()%9)*(8+Random()%5)/20;
   if( vo[n]<0 ) { vo[n]=0; end=n+49; }

   for( j=0;j<5;j++ ) ge[n]+=(40+Random()%31)*(ft[n][j].x>-1);
   for( j=0;j<6;j++ )
   {
      p[j]+=psp[j]*(Random()%99)/98-psp[j]/2;
      p[j]=Max(p[j],pmi[j]); p[j]=Min(p[j],pma[j]);
   }
   drw_gpk(0);
}

/******************************* Spielende ***********************************/
void ende()
{
	char s1[80],s2[80],s3[80],a,b,c;
	int sav_ssp;

	form_center( a_sie,&fx,&fy,&fw,&fh ); *(short *)(a_sie+18)=30;
	*(char **)(a_sie+24*SG1+12)=s1;
	*(char **)(a_sie+24*SG2+12)=s2;
	*(char **)(a_sie+24*SG3+12)=s3;
	strcpy_s( s1,"!! "); strcat_s( s1, end&2? (char*)l_nam:(char*)r_nam );
	strcat_s( s1," hat gewonnen !!" ); s2[0]=0;
	if(~end&64) strcpy_s(s2,"( ");   strcat_s( s2, end&2? (char*)r_nam:(char*)l_nam );
	if( (end&240)<48 )
	{ a=s2[strlen(s2)-1]; strcat_s( s2, a=='s' || a=='S'? "' ":"s " ); }
	switch( end&240 )
	{
	case 16: strcat_s( s2,"Kˆnig wurde getroffen," );
		strcpy_s( s3,"  daraufhin ergab sich dessen Volk. )" ); break;
	case 32: strcat_s( s2,"Kˆnig hat aufgrund der" );
		strcpy_s( s3,"  aussichtslosen Lage kapituliert. )" ); break;
	case 48: strcat_s( s2," hat kein Volk mehr. )" ); s3[0]=0; break;
	case 64: strcpy_s( s3,s2 );
		strcpy_s( s2,"( Die maximale Rundenzahl ist erreicht.");
		strcat_s( s3," befindet sich in der schlechteren Lage. )" );
	}
	objc_draw( a_sie,0,7,0,0,640,400 );

	for(a=0;a<6 && strncmp(t_na[a],(char*)l_nam,7);a++);
	for(b=0;b<6 && strncmp(t_na[b],(char*)r_nam,7);b++);
	if(a<6 && b<6 && a!=b)
	{
		if(~end&2) { c=a; a=b; b=c; }
		t_gew[a][b]++; t_gew[b][8]++;
		t_gew[a][9]=100*++t_gew[a][7]/++t_gew[a][6];
		t_gew[b][9]=100*t_gew[b][7]/++t_gew[b][6];
	}
	// TODO
	//hide(); sav_ssp=Super(0); m_musik(); Super(sav_ssp); show();  /* Musik... */
	//Giaccess( 0,138 ); Giaccess( 0,139 ); Giaccess( 0,140 );
	m_musik();
}

/* Die Routine m_wait() wird von m_musik() nach jedem 1/96 Takt aufgerufen.  */
/* In diesem Fall macht sie nichts anderes als die eigentliche Warteschleife */
/* aufzurufen. In eigenen Programmen kînnten Sie hier wÑhrend der Musik zu-  */
/* sÑtzliche Aktionen ablaufen lassen. */
void m_wait()
{
	// TODO
   //m_wloop();
}

/** Anzeige des Spielers, der am Zug ist, sowie Darstellung der Windfahnen ***/
void werdran(char c)
{
	short *a,i,x,y,w,h,c1,s1,c2,s2;
	char *ad;
	double wk,wl;

	z_txt(zug/2+1); v_gtext(handle,332,395,txt);

	a=(short *)(a_dra+16);
	if( c )
	{
		a[0]=5+(629-a[2])*n; a[1]=25;
		*(int *)(a_dra+DNAM*24+12)=n? r_nam:l_nam;
		ad=*(char **)(a_dra+DWIN*24+12);
		ad[0]=ad[5]=4+28*!wnd-(wnd>0);
		i=wnd<0? -wnd:wnd;
		ad[2]=48+i/10; ad[3]=48+i%10;
		if( wx[n]<0 ) { ad[0]=ad[5]=32; ad[2]=ad[3]='?'; }
		objc_offset( a_dra,DOK,&ftx,&fty );
		ftw=*(short *)(a_dra+DOK*24+20); fth=*(short *)(a_dra+DOK*24+22);
		objc_offset( a_dra,DWBX,&x,&y );
		w=*(short *)(a_dra+DWBX*24+20);  h=*(short *)(a_dra+DWBX*24+22);

		objc_draw( a_dra,0,4,0,0,640,400 );

		c=wnd>0? 1:-1;  wk=c*wnd/15.0; wl=wk*.82;
		if( wk>1.57 ) wk=1.57;  if( wl>1.57 ) wl=1.57;
		s1=c*20*sin(wk); c1=20*cos(wk); s2=c*20*sin(wl); c2=20*cos(wl);
		ws=s1/2.0; ws+=!ws; wc=c1/2.0;

		hide();
		if( wx[n]>-1 )
		{
			color(1); x+=w/2; line( x,y+h,x,y+5 ); line( x+1,y+h,x+1,y+5 );
			xy[0]=xy[2]=x+1; xy[1]=y+5; xy[3]=y+11;
			if( wk<.2 ) { xy[0]=x-1; xy[1]=xy[3]=y+5; xy[2]=x+2; }
			xy[4]=xy[2]+s1; xy[5]=xy[3]+c1;
			xy[8]=xy[0]+s1; xy[9]=xy[1]+c1;
			xy[10]=xy[0];   xy[11]=xy[1];
			xy[6]=(xy[4]+xy[8]>>1)+s2; xy[7]=(xy[5]+xy[9]>>1)+c2;
			v_pline( handle,6,xy );
		}
		fahne();
		show();
	}
	else
	{
		hide();
		// TODO
		clr( a[0],a[1],a[2]+1,a[3]+1 );
		show();
	}
}

/******************* Darstellung der beiden Windfahnen ***********************/
void fahne()
{
   char m=-1;

   while( ++m<2 ) if( wx[m]>-1 )
   {
      clr( wx[m]-10,wy[m]-15,20,15 );
      color(1);
      line( wx[m],wy[m],wx[m],wy[m]-15 );
      if( m==n )
      {
         line( wx[m],wy[m]-15,wx[m]+ws,wy[m]-13+wc );
         line( wx[m],wy[m]-11,wx[m]+ws,wy[m]-13+wc );
      }
   }
}

/********************** BALLER.TAB laden/speichern ***************************/
task<void> t_load()
{
	return file_exists().then([=]() {
		if(_tab_exists) {
			load();
		}
	});
}

task<void> file_exists()
{

	StorageFolder^ folder= ApplicationData::Current->LocalFolder;	
	task<StorageFile^> t([folder]()
	{
		return folder->CreateFileAsync("BALLER.TAB",CreationCollisionOption::FailIfExists);

	});
	auto opr = create_task(t);
	return opr.then([=](task<StorageFile^> t)
	{  
		try
		{
			StorageFile^ fol = t.get();
			_tab_exists = false;
		}
		catch(Platform::Exception^ ex)
		{
			_tab_exists = true;
		}

	}, task_continuation_context::use_current());
}

task<void> load()
{
   /*if( (f_h=Fopen( "BALLER.TAB",0 ))<0 ) return(1);
   Fread(f_h,1,&an_erl); Fread(f_h,1,&au_kap); Fread(f_h,1,&mxin);
   Fread(f_h,3,**(int **)(a_opt+MAX_XX*24+12) ); Fread(f_h,2,&max_rund);
   Fread(f_h,48,t_na); Fread(f_h,120,t_gew);
   Fclose(f_h); return(0);*/

	task<StorageFile^> getFileOperation(ApplicationData::Current->LocalFolder->GetFileAsync("BALLER.TAB"));
   return getFileOperation.then([=](StorageFile^ file)
   {
      return FileIO::ReadBufferAsync(file);
   }).then([=](concurrency::task<IBuffer^> previousOperation) {
      IBuffer^ buf = previousOperation.get();

	  try {
	  DataReader^ dr = Windows::Storage::Streams::DataReader::FromBuffer(buf);
	  an_erl = dr->ReadByte();
	  au_kap = dr->ReadByte();
	  mxin = dr->ReadByte();
	  char* tmp = new char[3];
	  tmp[0] = dr->ReadByte();
	  tmp[1] = dr->ReadByte();
	  tmp[2] = dr->ReadByte();
	  ((char*)(**(int **)(a_opt+6*24+12)))[0] = tmp[0];
	  ((char*)(**(int **)(a_opt+6*24+12)))[1] = tmp[1];
	  ((char*)(**(int **)(a_opt+6*24+12)))[2] = tmp[2];
	  max_rund = dr->ReadInt16();
	  for(int x = 0;x<6;x++) {
		   for(int y = 0;y<8;y++) {
			t_na[x][y] = dr->ReadByte();
		   }
	   }
	   for(int x = 0;x<6;x++) {
		   for(int y = 0;y<10;y++) {
			 t_gew[x][y] = dr->ReadByte();
		   }
	   }
	  } catch(Platform::Exception^ ex)
	  {
	  }
         
   });
}

task<void> t_save()
{
	/*if( (f_h=Fcreate( "BALLER.TAB",0 ))<0 ) return(1);
   Fwrite(f_h,1,&an_erl); Fwrite(f_h,1,&au_kap); Fwrite(f_h,1,&mxin);
   Fwrite(f_h,3,**(int **)(a_opt+MAX_XX*24+12) ); Fwrite(f_h,2,&max_rund);
   Fwrite(f_h,48,t_na); Fwrite(f_h,120,t_gew);
   Fclose(f_h); return(0);*/
	task<StorageFile^> getFileOperation(ApplicationData::Current->LocalFolder->CreateFileAsync("BALLER.TAB", CreationCollisionOption::ReplaceExisting));
   return getFileOperation.then([=](StorageFile^ file)
   {

	   DataWriter^ dw = ref new DataWriter();
	   dw->WriteByte(an_erl);
	   dw->WriteByte(au_kap);
	   dw->WriteByte(mxin);
	   char* tmp = (char *)(**(int **)(a_opt+MAX_XX*24+12));
	   dw->WriteByte(tmp[0]);
	   dw->WriteByte(tmp[1]);
	   dw->WriteByte(tmp[2]);
	   dw->WriteInt16(max_rund);
	   for(int x = 0;x<6;x++) {
		   for(int y = 0;y<8;y++) {
			dw->WriteByte(t_na[x][y]);
		   }
	   }
	   for(int x = 0;x<6;x++) {
		   for(int y = 0;y<10;y++) {
			 dw->WriteInt16(t_gew[x][y]);
		   }
	   }	   
	   return FileIO::WriteBufferAsync(file, dw->DetachBuffer());
   });
}

/************************* BALLER.DAT laden **********************************/
void burgen_laden()
{
   short *a,j;

   a=(short *)bur_ad;
   f_h = CreateFile2(L"BALLER.DAT", GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);
   b_anz=0;
   while( (j=rdzahl())!=-999 )
   {
      burgen[b_anz++]=a;
      *a++=j;
      for( j=0;j<40;j++ ) *a++=rdzahl();
      while( (*a++=rdzahl())!=-1 );
   }
   CloseHandle(f_h);
}

char zeichen()  /* liest ein char von der Datei */
{
   char a;
   DWORD read = -1;
   ReadFile(f_h, &a, 1, &read, NULL);
   return(a);
}

int rdzahl() /* liest eine Dezimalzahl von der Datei, Remarks werden Åberlesen */
{
   char a,sign=1,rem=0;    /* wird durch * getoggled, und zeigt damit an, */
                           /* ob man sich in einer Bemerkung befindet */
   int val=0;

   do
      if( (a=zeichen())=='*' ) rem=!rem;
   while( a!='-' && a<'0' || a>'9' || rem );

   if( a=='-' ) { sign=-1; a=zeichen(); }
   while( a>='0' && a<='9' )
   {
      val*=10; val+=a-'0';
      a=zeichen();
   }
   return( sign*val );
}

s_sch_obj1 _sch_obj1;

/************* Kanonenobjektbaum, Wahl von Winkel und Pulver *****************/
short sch_obj(short k)
{
	short i, wi,pv, xw,yw,xp,yp,xk,yk;
	double s,c;
	char *aw,*ap, fl=1;

	aw=*(char **)(a_sch+24*WINK+12);
	ap=*(char **)(a_sch+24*PULV+12);  *(ap+2)=0;

	sav_scr();

	form_center( a_sch,&fx,&fy,&fw,&fh );
	objc_offset( a_sch,WINK,&xw,&yw );
	objc_offset( a_sch,PULV,&xp,&yp );
	objc_offset( a_sch,KAST,&xk,&yk );  xk+=105+f*36; yk+=102;
	// TODO
	form_dial(1, ka[n][k].x+10*f,ka[n][k].y-10,20,14,fx,fy,fw,fh );
	wi=ka[n][k].w; pv=ka[n][k].p;

	vsf_interior( handle,1 );

	i = 0;
	_sch_obj1.k = k;
	_sch_obj1.i = i;
	_sch_obj1.wi = wi;
	_sch_obj1.pv = pv;
	_sch_obj1.xw = xw;
	_sch_obj1.yw = yw;
	_sch_obj1.xp = xp;
	_sch_obj1.yp = yp;
	_sch_obj1.xk = xk;
	_sch_obj1.yk = yk;
	_sch_obj1.fl = fl;
	_sch_obj1.aw = aw;
	_sch_obj1.ap = ap;
	sch_obj1();

	return 0;
}

void sch_obj1()
{
	static short fig[]={ 0,0,15,20,30,20,20,15,10,0,10,-30,18,-18,20,-5,24,-6,
		20,-25,10,-40,0,-45, -10,-40,-20,-25,-24,-6,-20,-5,-18,-18,-10,-30,-10,0,
		-20,15,-30,20,-15,20, -1,-1 }; /* Daten fÅr das MÑnnchen */
	short i, wi,pv, xw,yw,xp,yp,xk,yk;
	double s,c;
	char *aw,*ap, fl;
	i = _sch_obj1.i;
	wi = _sch_obj1.wi;
	pv = _sch_obj1.pv;
	xw = _sch_obj1.xw;
	yw = _sch_obj1.yw;
	xp = _sch_obj1.xp;
	yp = _sch_obj1.yp;
	xk = _sch_obj1.xk;
	yk = _sch_obj1.yk;
	fl = _sch_obj1.fl;
	aw = _sch_obj1.aw;
	ap = _sch_obj1.ap;

	if( pv>pu[n] ) pv=pu[n];
	*aw=48+wi/100; *(aw+1)=48+wi%100/10; *(aw+2)=48+wi%10;
	if(wi<100) { *aw=*(aw+1); *(aw+1)=*(aw+2); *(aw+2)=0; }
	*ap=48+pv/10; *(ap+1)=48+pv%10;
	if(pv<10)  { *ap=*(ap+1); *(ap+1)=0; }

	if( fl )
	{
		objc_draw( a_sch,0,4,0,0,640,400 );
		hide();
		color(1);
		v_circle( handle,xk-88*f,yk-60,15 );
		i=0;
		while( fig[i]!=-1 )
			{ xy[i]=xk-88*f+fig[i]; i++; xy[i]=yk-5+fig[i]; i++; }
		xy[i++]=xy[0]; xy[i++]=xy[1];
		v_fillarea( handle,i/2-1,xy );
		show();
	}
	objc_draw( a_sch,0,4,xw,yw,42,18 );
	objc_draw( a_sch,0,4,xp,yp,42,18 );

	if( i!=PL2 && i!=PR2 && i!=PL1 && i!=PR1 || fl )
	{
		hide();
		clr( xk-55,yk-76,110,90 );
		color( 1 );
		v_circle( handle,xk,yk,15 );

		s=sin(wi/P57); c=cos(wi/P57);
		fl=-f; if( wi>90 ) { fl=-fl; c=-c; }
		xy[0]=xk+fl*(c*14+s*14);  xy[1]=yk+s*14-c*14;
		xy[2]=xk+fl*(c*14+s*40);  xy[3]=yk+s*14-c*40;
		xy[4]=xk-fl*(c*55-s*40);  xy[5]=yk-s*55-c*40;
		xy[6]=xk-fl*(c*55-s*14);  xy[7]=yk-s*55-c*14;
		xy[8]=xy[0]; xy[9]=xy[1];
		v_fillarea( handle,4,xy );
		show();
		fl=0;
	}

	_sch_obj1.i = i;
	_sch_obj1.wi = wi;
	_sch_obj1.pv = pv;
	_sch_obj1.xw = xw;
	_sch_obj1.yw = yw;
	_sch_obj1.xp = xp;
	_sch_obj1.yp = yp;
	_sch_obj1.xk = xk;
	_sch_obj1.yk = yk;
	_sch_obj1.fl = fl;
	_sch_obj1.aw = aw;
	_sch_obj1.ap = ap;
	callback = &sch_obj2;
	// TODO
}

void sch_obj2()
{
	short i, wi, pv;
	short k;
	k = _sch_obj1.k;

	callback = NULL;
	i = _sch_obj1.i;
	wi = _sch_obj1.wi;
	pv = _sch_obj1.pv;
	// TODO
	i=form_do( a_sch,0 ); //if( i>=0 ) *(short *)(a_sch+24*i+10)=0;
	wi-=10*(i==WL2)-10*(i==WR2)+(i==WL1)-(i==WR1);
	if( wi<0 ) wi=0;  if( wi>180 ) wi=180;
	pv-= 3*(i==PL2)- 3*(i==PR2)+(i==PL1)-(i==PR1);
	if( pv<5 ) pv=5;  if( pv>20 ) pv=20;
	if(i!=SOK && i!=SAB)
	{
		_sch_obj1.i = i;
		_sch_obj1.wi = wi;
		_sch_obj1.pv = pv;
		sch_obj1();
		return;
	}

	lod_scr();
	// TODO
	form_dial(2, ka[n][k].x+10*f,ka[n][k].y-10,20,14,fx,fy,fw,fh );
	ka[n][k].w=wi; ka[n][k].p=pv;
	if(i==SOK) 
	{
		ein_zug2(i==SOK, _sch_obj1.k);
	} else {
		callback = &ein_zug1;
	}
	//return( i==SOK );
}

struct s_bur_obj {
	short y,w,h;
	short ob0,ob1,oy0,oy1;
	short ol[8];
};
s_bur_obj _bur_obj;
/********************* Neues Spiel: Auswahl der Burgen ***********************/
void bur_obj()
{
	// TODO
	short i,ob0,ob1,oy0,oy1, x,y,w,h, ol[8];

	oy0=by[0]; oy1=by[1]; ob0=bur[0]; ob1=bur[1];

   *(int *)(a_brg+24*BSP1+12)=l_nam;
   *(int *)(a_brg+24*BSP2+12)=r_nam;

   form_center( a_brg,&fx,&fy,&fw,&fh );

   w=*(short *)(a_brg+24*BK1+20);     h=*(short *)(a_brg+24*BK1+22);
   objc_offset( a_brg,BK1,&x,&y ); bx[0]=x;   by[0]=y+h;
   objc_offset( a_brg,BK2,&x,&y ); bx[1]=x+w; by[1]=y+h;

   sav_scr();
   form_dial( 1,70,20,30,20,fx,fy,fw,fh );
   objc_draw( a_brg,0,5,0,0,640,400 );
   ol[0]=ge[0]; ol[1]=ge[1];   ol[2]=pu[0]; ol[3]=pu[1]; /* Sichern der alten*/
   ol[4]=ku[0]; ol[5]=ku[1];   ol[6]=vo[0]; ol[7]=vo[1]; /* Werte */
   burg(2); burg(3);

   ge[0]=ge[1]=pu[0]=pu[1]=ku[0]=ku[1]=9999;
   _bur_obj.y = y;
   _bur_obj.w = w;
   _bur_obj.h = h;
   _bur_obj.ob0 = ob0;
   _bur_obj.ob1 = ob1;
   _bur_obj.oy0 = oy0;
   _bur_obj.oy1 = oy1;
   for(i=0;i<8;i++) {
	_bur_obj.ol[i] = ol[i];
   }
   callback = &bur_obj2;
}

void bur_obj2()
{
	short i,y,w,h;
	y = _bur_obj.y;
	w = _bur_obj.w;
	h = _bur_obj.h;
	i=form_do( a_brg,0 ); *(short *)(a_brg+24*i+10)=0;
	if( i==BL1 || i==BR1 )
	{
		bur[0]=(bur[0]+1-2*(i==BL1)+b_anz)%b_anz;
		clr( bx[0],y,w,h ); burg(2);
	}
	if( i==BL2 || i==BR2 )
	{
		bur[1]=(bur[1]+1-2*(i==BL2)+b_anz)%b_anz;
		clr( bx[1]-w,y,w,h ); burg(3);
	}
	if(i!=BOK && i!=BAB) return;

	bur_obj3(i);
}

void bur_obj3(short i)
{
	short ol[8];
	short ob0,ob1,oy0,oy1;

	for(int ii = 0;ii<8;ii++) {
		ol[ii] = _bur_obj.ol[ii];
	}
	ob0 = _bur_obj.ob0;
	ob1 = _bur_obj.ob1;
	oy0 = _bur_obj.oy0;
	oy1 = _bur_obj.oy1;

	ge[0]=ol[0]; ge[1]=ol[1];  pu[0]=ol[2]; pu[1]=ol[3];
	ku[0]=ol[4]; ku[1]=ol[5];  vo[0]=ol[6]; vo[1]=ol[7];

	lod_scr();
	form_dial( 2,70,20,30,20,fx,fy,fw,fh );

	fn(); by[0]=oy0; by[1]=oy1;
	if( i==BAB ) { bur[0]=ob0; bur[1]=ob1; }

	m_audio.StopSoundEffect(EndeEvent);
	event2(i==BOK);
}

/************** Darstellung und Verwaltung eines Objektbaumes ****************/
void obj_do(int adr)
{	
	int a; short x=20,fl=0;

	if(adr==a_nam) { fl=NSP1; x=80; }
	if(adr==a_opt) { fl=MAX_XX; x=80; }
	if(adr==a_com) x=150;
	if(adr==a_ein) { fl=EI_N1; x=260; }

	form_center( adr,&fx,&fy,&fw,&fh );
	sav_scr(); form_dial( 1,x,20,30,20,fx,fy,fw,fh );
	objc_draw( adr,0,5,0,0,640,400 );
	_obj_do.adr = adr;
	_obj_do.fl = fl;
	_obj_do.x = x;
	callback = &obj_do2;
}

void obj_do2()
{
	int a;
	int adr;
	short x, fl;
	adr = _obj_do.adr;
	fl = _obj_do.fl;
	x = _obj_do.x;
	a=form_do( adr,fl );
	if(a == -1) return;
		callback = &ein_zug1;
		_obj_do.result = a;
		*(short *)(adr+24*a+10)=0;
		lod_scr(); form_dial( 2,x,20,30,20,fx,fy,fw,fh );

		if(obj_do_callback != NULL) obj_do_callback();
}

/*********************** Elementare  Grafikbefehle ***************************/
void gem_init() /* ÷ffnen der Workstation... */
{
	// TODO
}

void color(short a)
{
   vst_color( handle,a );
   vsl_color( handle,a );
   vsf_color( handle,a );
}

int loc(short x, short y)   /* Ermittelt, ob Punkt gesetzt ist */
{
   short a,b;
   v_get_pixel(handle,x,y,&a,&b);
   return((int)a);
}

void line( short x1, short y1, short x2, short y2 ) /* Zeichnet eine Linie */
{
	xy[0]=x1; xy[1]=y1; xy[2]=x2; xy[3]=y2;
	v_pline( handle, 2, xy );
}

void box(short x, short y, short x2, short y2, short c)
{
	color(c);
	xy[0]=x; xy[1]=y; xy[2]=x2; xy[3]=y2;
	v_bar( handle,xy );
}

void clr( short x, short y, short w, short h )
{
	box(x,y,x+w-1,y+h-1,0);
}

void cls() /* Lˆscht den gesamten Bildschirm ausser der Men¸leiste */
{
	int *a,i;

	//a=(int *)(scr+32000);
	//for( i=0;i<7600;i++ ) *--a=0;
	int index;
	for(int y = 19;y<400;y++)
	{
		for(int x = 0;x<640;x++)
		{
			index = (y * 640 + x) << 2;
			handle->pixels[index] = 255; // blau
			handle->pixels[index+1] = 255; // gr¸n
			handle->pixels[index+2] = 255; // rot
			handle->pixels[index+3] = 0; // alpha

		}
	}
}
