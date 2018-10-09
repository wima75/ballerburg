#pragma once

#include <pch.h>
#include <WriteableBitmapEx.h>


struct ressource
{
	// size: 24
	byte* pixels;
	short tree;
	short dummy1[5];
	short x; // 16
	short y;
	short w;
	short h;
};

struct ressource_dran : ressource
{
	short dummy2[18]; // 24
	int dnam; // 60
	int i2;
	short dummy3[20]; // 68
	char* dwin; // 108
	short dummy4[24]; //112
	
	short dwbx_x;
	short dwbx_y;
	short dwbx_w; // 164
	short dwbx_h;
	
	short dummy5[8]; // 168
	
	short dok_x; // 184
	short dok_y;
	short dok_ftw; // 188
	short dok_fth;
};

struct ressource_schuss : ressource 
{
	short dummy2[66];
	char* wink; // 156
	short dummy3[70]; // 160
	char* pulv; // 300
};

struct ressource_burg : ressource
{
	short dummy2[6];
	int bsp1; // 36
	short dummy3[10]; // 40
	int bsp2; // 60
	short dummy4[26]; // 64
	short bk1_w; // 116
	short bk1_h;
};

struct ressource_eintrag : ressource
{
	short dummy2[54]; // 24
	char* ei_n1; // 132
	short dummy3[10]; // 136
	char* ei_n2; // 156
	short dummy4[10];
	char* ei_n3;
	short dummy5[10];
	char* ei_n4;
	short dummy6[10];
	char* ei_n5;
	short dummy7[10];
	char* ei_n6; // 252
	short dummy8[21]; // 256
	short er_loe; // 298
	short dummy9[11]; // 300
	short er_ble; // 322
};

struct ressource_computer : ressource
{
	short dummy2[29];
	short cn1_0; // 82
	short dummy3[11];
	short cn1_1; // 106
	short dummy4[11];
	short cn1_2; // 130
	short dummy5[11];
	short cn1_3; // 154
	short dummy6[11];
	short cn1_4; // 178
	short dummy7[11];
	short cn1_5; // 202
	short dummy8[11];
	short cn1_6; // 226

	short dummy9[23];
	short cn2_0; // 274
	short dummy10[11];
	short cn2_1; // 298
	short dummy11[11];
	short cn2_2; // 322
	short dummy12[11];
	short cn2_3; // 346
	short dummy13[11];
	short cn2_4; // 370
	short dummy14[11];
	short cn2_5; // 394
	short dummy15[11];
	short cn2_6; // 418

	short dummy16[23];
	short cts1_0; // 466
	short dummy17[11];
	short cts1_1; // 490
	short dummy18[11];
	short cts1_2; // 514
	short dummy19[11];
	short cts1_3; // 538

	short dummy20[23];
	short cts2_0; // 586
	short dummy21[11];
	short cts2_1; // 610
	short dummy22[11];
	short cts2_2; // 634
	short dummy23[11];
	short cts2_3; // 658
};

struct ressource_option : ressource
{
	short dummy2[29]; // 24
	short max_1; // 82
	short dummy3[11]; // 84
	short max_2; // 106
	short dummy4[11];
	short max_ne;
	short dummy5[11];
	short max_xx; // 154
	int* max_xx1;
	short dummy6[33];
	short a_erl; // 226
	short dummy7[11];
	short a_ver; // 250
	short dummy8[59];
	short a_kap; // 370
};

struct ressource_status : ressource 
{
	short dummy2[30]; // 24
	char* sh1; // 84
	short dummy3[10];
	char* sh2;
	short dummy4[10];
	char* sh3;
	short dummy5[10];
	char* sh4;
	short dummy6[10];
	char* sh5;
	short dummy7[10];
	char* sh6;
	short dummy8[10];
	char* sh7;
	short dummy10[10];
	char* sh8; // 252
	short dummy11[69]; // 256
	short sm1_flags;
	char* sm1; // 396
	short dummy12[9]; // 400
	short sm2_flags;
	char* sm2; // 420
	short dummy13[9];
	short sm3_flags;
	char* sm3;
	short dummy14[9];
	short sm4_flags;
	char* sm4;
	short dummy15[9];
	short sm5_flags;
	char* sm5;
	short dummy16[9];
	short sm6_flags;
	char* sm6;


};

struct ressource_sieger : ressource
{
	short dummy2[66]; // 24
	char* sg1; // 156
	short dummy3[22]; // 160
	char* sg2; // 204
	short dummy4[10]; // 208
	char* sg3; // 228
};

struct ressource_form : ressource
{
	short bx, by,bw,bh;
};

struct ressource_menu : ressource
{
	short desk_x, desk_w;
	short spiel_x,spiel_w;
	short modus_x,modus_w;
	short regeln_x, regeln_w;
	short ergebnisse_x, ergebnisse_w;
	short check;
};

struct ressource_menu_desk : ressource
{
};

struct ressource_menu_spiel : ressource
{
};

struct ressource_menu_modus : ressource
{
};

struct ressource_menu_regeln : ressource
{
};

struct ressource_menu_ergebnisse : ressource
{
};

struct ressource_namen : ressource
{
	short dummy2[18]; // 24
	char* Name1; // 60
	short dummy3[10]; // 64
	char* Name2; // 84
};

void v_pline(WriteableBitmapEx* g, int count, short *pxy);

long Random();

void vst_color(WriteableBitmapEx* handle, int color);
void vsl_color(WriteableBitmapEx* handle, int color);
void vsf_color(WriteableBitmapEx* handle, int color);
void vswr_mode(WriteableBitmapEx* handle, int mode);
void vsl_type(WriteableBitmapEx* handle, int type);
void vsf_style(WriteableBitmapEx* handle, int style);
void vsl_udsty(WriteableBitmapEx* handle, int pattern);
void vsf_interior(WriteableBitmapEx* handle, int interior);
void v_fillarea(WriteableBitmapEx* handle, int count, short *pxy);
void v_bar(WriteableBitmapEx* handle, short* pxy);
void v_circle(WriteableBitmapEx* handle, short x, short y, short radius);

void vst_height(WriteableBitmapEx* handle, int height, short* wchar, short* hchar, short* wcell, short* hcell);
void v_gtext(WriteableBitmapEx* handle, int x, int y, char* str);

void graf_mouse(int mode, int formptr);

void objc_offset(int tree, int obj, short* ox, short* oy);
void objc_draw(int tree, int obj, int depth, int bx,int by,int bw,int bh);
void Dosound(char* cmdlist);
void graf_mkstate(short* mx, short* my, short* mb, short* ks);
void form_alert(int default1, char* alertstr);

void rsrc_gaddr(int type, int index, void* addr);
void form_center(int tree, short* x, short* y, short* w, short* h);
short form_do(int tree, int editobj);
void form_dial(short mode, short x1, short y1, short w1, short h1, short x2, short y2, short w2, short h2);
void v_get_pixel(WriteableBitmapEx* handle, short x, short y, short* a, short* b);
bool drin_(short x, short y, short x1, short y1, short w, short h);
void _drawChars(char* ch, int length, int length2, short leftOffset, short x, short y, short h);
void _drawChar(char ch,short leftOffset, short x, short y, short w, short h);
int _findCharPos(char ch);
void _fillCharArray(char* dest, char* src, int count);
void bing();
void menu_icheck(int tree, short obj, short check);

void menu_bar(int tree, short mode);
void Giaccess(int data, int register1);
void m_musik();

enum enm_feedback
{
	fbNone,
	fbAbbrechen,
	fbOK
};
