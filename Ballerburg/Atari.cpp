#include <pch.h>
#include <Atari.h>
#include <Baller.h>

using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;

int _vsl_type;
int _vsl_pattern;
byte _textColor;
byte _lineColor;
byte _fillColor;
byte _vsf_color;
int _vswr_mode;
int _vsf_style;
int _vsf_interior;

extern WriteableBitmapEx* handle;
ressource_schuss res_schuss;
ressource_dran res_dran;
ressource_burg res_burg;
ressource_eintrag res_eintrag;
ressource_computer res_computer;
ressource_option res_option;
ressource_status res_status;
ressource_sieger res_sieger;
ressource res_font;
ressource res_font_disabled;
ressource res_gewonnen;
ressource res_verloren;
ressource res_infotree;
ressource res_regel1;
ressource res_regel2;
ressource res_regel3;
ressource_form res_form_alert_3;
ressource_form res_form_alert_4;
ressource_form res_form_alert_5;
ressource_form res_form_alert_schade;
ressource_form res_form_alert_0;
ressource_menu res_menu;
ressource_menu_desk res_menu_desk;
ressource_menu_spiel res_menu_spiel;
ressource_menu_modus res_menu_modus;
ressource_menu_regeln res_menu_regeln;
ressource_menu_ergebnisse res_menu_ergebnisse;
ressource_namen res_namen;

byte* _form_dial_screen;
ressource_form* res_form;
ressource* res_actual_font = &res_font;

extern Sound^ _sound;
extern Audio m_audio;
extern bool menu_enabled;


void v_pline(WriteableBitmapEx* handle, int count, short *pxy)
{
	if(count == 2) {
		if(_vsl_type == 7) {
			handle->DrawLineBrush(pxy[0],pxy[1], pxy[2], pxy[3], 0, _vsl_pattern);
		} else {
			handle->DrawLineBresenham(pxy[0],pxy[1], pxy[2], pxy[3], 0);
		}
	} else {
		handle->DrawPolyline(pxy, count*2, 0);
	}
}

void v_circle(WriteableBitmapEx* handle, short x, short y, short radius)
{
	if(radius == 5)
	{
		int data[11] = { 32, 248, 508, 1022, 1022, 2047, 1022, 1022, 508, 248, 32};
		x -= 6;
        y -= 6;
		for (int i = 0; i < 11; i++) {
            for (int j = 10; j >= 0; j--) {
                if (!((data[i] >> j) & 1)) continue;
                int index = (x + j + (y + i) * handle->_width) * 4;
                handle->pixels[index] = ((data[i] >> j) & 1) * 255;
                handle->pixels[index + 1] = handle->pixels[index];
                handle->pixels[index + 2] = handle->pixels[index];
                handle->pixels[index + 3] = 255;
            }
        }
		return;
	}
	if (radius == 15) {
		unsigned int data[31] = {((1 << 2) - 1) << 16, ((1 << 7) - 1) << 13, ((1 << 13) - 1) << 10, ((1 << 17) - 1) << 8, ((1 << 19) - 1) << 7, ((1 << 21) - 1) << 6, ((1 << 23) - 1) << 5, ((1 << 25) - 1) << 4, ((1 << 25) - 1) << 4, ((1 << 27) - 1) << 3, ((1 << 27) - 1) << 3, ((1 << 27) - 1) << 3, ((1 << 29) - 1) << 2, ((1 << 29) - 1) << 2, ((1 << 31) - 1) << 1, ((1 << 31) - 1) << 1, ((1 << 30) - 1) << 1, ((1 << 29) - 1) << 2, ((1 << 29) - 1) << 2, ((1 << 27) - 1) << 3, ((1 << 27) - 1) << 3, ((1 << 27) - 1) << 3, ((1 << 25) - 1) << 4, ((1 << 25) - 1) << 4, ((1 << 23) - 1) << 5, ((1 << 21) - 1) << 6, ((1 << 19) - 1) << 7, ((1 << 17) - 1) << 8, ((1 << 13) - 1) << 10, ((1 << 7) - 1) << 13, ((1 << 3) - 1) << 15};
        x -= 16;
        y -= 16;
        for (int i = 0; i < 31; i++) {
            for (int j = 31; j >= 0; j--) {
                if (!((data[i] >> j) & 1)) continue;
                int index = (x + j + (y + i) * handle->_width) * 4;
                handle->pixels[index] = 0;
                handle->pixels[index + 1] = handle->pixels[index];
                handle->pixels[index + 2] = handle->pixels[index];
                handle->pixels[index + 3] = 255;
            }
        }
        return;
    }
	if(radius == 17) {
		unsigned int or[17] = {15<<13,127<<10,255<<9,1023<<7,2047<<6,4095<<5,8191<<4,16383<<3,16383<<3,32767<<2,32767<<2,65535<<1,65535<<1,65535<<1,65535<<1,131071,131071};
		unsigned int ol[17] = {15,63,255,1023,2047,4095,8191,16383,16383,32767,32767,65535,65535,65535,65535,131071,131071};
		x -= 17;
		y -= 17;
		for (int i = 0; i < 17; i++) {
			for (int j = 17; j >= 0; j--) {
				if (!((or[i] >> j) & 1)) continue;
				int index = (x + j + (y + i) * handle->_width) * 4;
				handle->pixels[index] = 255;
				handle->pixels[index + 1] = handle->pixels[index];
				handle->pixels[index + 2] = handle->pixels[index];
				handle->pixels[index + 3] = 255;
			}
			for (int j = 17; j >= 0; j--) {
				if (!((ol[i] >> j) & 1)) continue;
				int index = (x + j + 17 + (y + i) * handle->_width) * 4;
				handle->pixels[index] = 255;
				handle->pixels[index + 1] = handle->pixels[index];
				handle->pixels[index + 2] = handle->pixels[index];
				handle->pixels[index + 3] = 255;
			}
		}
		for (int i = 0; i < 17; i++) {
			for (int j = 17; j >= 0; j--) {
				if (!((or[i] >> j) & 1)) continue;
				int index = (x + j + (y + (33-i)) * handle->_width) * 4;
				handle->pixels[index] = 255;
				handle->pixels[index + 1] = handle->pixels[index];
				handle->pixels[index + 2] = handle->pixels[index];
				handle->pixels[index + 3] = 255;
			}
			for (int j = 17; j >= 0; j--) {
				if (!((ol[i] >> j) & 1)) continue;
				int index = (x + j + 17 + (y + (33-i)) * handle->_width) * 4;
				handle->pixels[index] = 255;
				handle->pixels[index + 1] = handle->pixels[index];
				handle->pixels[index + 2] = handle->pixels[index];
				handle->pixels[index + 3] = 255;
			}
		}		
		return;
	}
	throw ref new Platform::FailureException("v_circle " + radius + " not implemented");
}

void vst_color(WriteableBitmapEx* handle, int color) 
{
	_textColor = color == 1 ? 0 : 255;
}
void vsl_color(WriteableBitmapEx* handle, int color) 
{
	_lineColor = color == 1 ? 0 : 255;
}
void vsf_color(WriteableBitmapEx* handle, int color) 
{
	_fillColor = color == 1 ? 0 : 255;
	_vsf_color = color == 1 ? 0 : 255;
}
void vswr_mode(WriteableBitmapEx* handle, int mode) {
    // 1: replace: alle Pixel werden durch die neue Ausgabe ersetzt
    // 2: transparent: alle gesetzten Pixel werden geschrieben, alle nicht gesetzten Pixel bleiben in der vorher gesetzten Farbe
    // 3: xor: die auszugebenden Pixel werden mit den vorher vorhandenen Pixel durch XOR verknüpft
    // 4: reverse transparent: alle nicht gesetzten Pixel werden geschrieben, alle gesetzten Pixel bleiben in der vorherigen Farbe (Umkehrung zu Modus 2)
    _vswr_mode = mode;
	_vsf_interior = 0;
}

int brush_2_4[8] = {85, 0, 85, 0, 85, 0, 85, 0};
int brush_2[8] = {170, 0, 170, 0, 170, 0, 170, 0};
int brush_9_4[8] = {247, 247, 0, 127, 127, 127, 0, 247};
int brush_9[8] =  {8, 8, 255, 128, 128, 128, 255, 8};
int brush_11_4[8] =  {255, 255, 191, 95, 255, 255, 253, 250};
int brush_11[8] = {0, 0, 64, 160, 0, 0, 2, 5};

int* GetBrush() {
    if (_vsf_style == 2) {
        if (_vswr_mode == 4) {
			return brush_2_4;
        } else {
			return brush_2;
        }
    }
    if (_vsf_style == 9) {
        if (_vswr_mode == 4) {
			return brush_9_4;
        } else {
			return brush_9;
        }
    }
    if (_vsf_style == 11) {
        if (_vswr_mode == 4) {
			return brush_11_4;
        } else {
			return brush_11;
        }
    }
	throw ref new Platform::FailureException(_vsf_style + " style not implemented");
}

void v_fillarea(WriteableBitmapEx* handle, int count, short *pxy) {
	pxy[count*2] = pxy[0];
	pxy[count*2+1] = pxy[1];
	if (_vsf_interior == 2) {
		int* brush1 = GetBrush();
        handle->FillPolygonBrush(pxy, count*2+2, 0, brush1);
    } else {
        handle->FillPolygon(pxy, count*2+2, 0);
		handle->DrawPolyline(pxy, count*2+2, 0);
    }
}

void v_bar(WriteableBitmapEx* handle, short* pxy)
{
	short* pxyp = new short[10];
	pxyp[0] = pxy[0];
	pxyp[1] = pxy[1];
	pxyp[2] = pxy[2];
	pxyp[3] = pxy[1];
	pxyp[4] = pxy[2];
	pxyp[5] = pxy[3];
	pxyp[6] = pxy[0];
	pxyp[7] = pxy[3];
	pxyp[8] = pxy[0];
	pxyp[9] = pxy[1];
	if (_vsf_interior == 2 && _fillColor == 0) {
		int* brush1 = GetBrush();
		// TODO: wieso + 1?
		//handle->FillRectangleBrush(pxy[0], pxy[1]+1, pxy[2], pxy[3], brush1);
		//handle->FillRectangleBrush(pxy[0], pxy[1], pxy[2], pxy[3], brush1);
		handle->FillPolygonBrush(pxyp, 10, 0, brush1);
		//handle->DrawPolyline(pxyp, 10, _fillColor);
	} else if(_vsf_interior == 0) {
		if(_vsf_style == 9 && _fillColor == 0) {
			int* brush1 = GetBrush();
			//handle->FillRectangleBrush(pxy[0], pxy[1], pxy[2], pxy[3], brush1);
			handle->FillPolygonBrush(pxyp, 10, 0, brush1);
			//handle->DrawPolyline(pxyp, 10, _fillColor);
		} else {
			handle->FillRectangle(pxy[0], pxy[1], pxy[2], pxy[3], 255);
			handle->DrawPolyline(pxyp, 10, _fillColor);
		}
	} else {
		handle->FillRectangle(pxy[0], pxy[1], pxy[2], pxy[3], _fillColor);
	}
}

void vsf_interior(WriteableBitmapEx* handle, int interior) {
    // 0: hollow (aushöhlen)
    // 1: solid
    // 2: pattern
    // 3: hatch
    // 4: user
    _vsf_interior = interior;
}

void vsf_style(WriteableBitmapEx* handle, int style) {
	_vsf_style = style;
}

long Random()
{
	return rand() % 16777216;
	//return 3;
}

// vsl_type() defines the style of line used in line-drawing functions and perimeter drawing
void vsl_type(WriteableBitmapEx* handle, int type) {
    _vsl_type = type;
}

void vsl_udsty(WriteableBitmapEx* handle, int pattern) {
	_vsl_pattern = pattern;
}

void v_get_pixel(WriteableBitmapEx* handle, short x, short y, short* a, short* b)
{
	int index = 4 * (y * handle->_width + x);
	*a = handle->pixels[index] == 255 ? 0 : 1;
}

void graf_mouse(int mode, int formptr) 
{
	// TODO
}

int _vst_height = 13;
void v_gtext(WriteableBitmapEx* handle, int x, int y, char* str)
{
	if(_vst_height == 4) {
		ressource* res;
		if(x == 60) {
			res = &res_verloren;
		} else {
			res = &res_gewonnen;
        }
		y -= 4;
		int o = 4 * (y * 640 + x);
		for(int y = 0;y<res->h;y++) {
			for(int x = 0;x<res->w;x++)
			{
				int index1 = 4 * (y * 640 + x);
				int index2 = y * res->w + x;
				handle->pixels[index1+o] = res->pixels[index2];
				handle->pixels[index1+o+1] = res->pixels[index2];
				handle->pixels[index1+o+2] = res->pixels[index2];
				handle->pixels[index1+o+3] = 255;
			}
		}
	} else {
		int length = 0;
		char ch = str[length++];
		while(ch != NULL) {
			ch = str[length++];
		}
		_drawChars(str, length-1, length-1, 0, x, y-13, 16);
	}
}

void vst_height(WriteableBitmapEx* handle, int height, short* wchar, short* hchar, short* wcell, short* hcell)
{
	_vst_height = height;
}

void graf_mkstate(short* mx, short* my, short* mb, short* ks)
{
	// TODO
}

void form_alert(int default1, char* alertstr)
{
	// TODO
	menu_enabled = false;
	int i = 0;
	int pipes = 0;
	char ch = alertstr[i++];
	while(ch != NULL) {
		if(ch == '|')
		{
			pipes++;
		}
		ch = alertstr[i++];
	}
	res_form = NULL;
	if(pipes == 0)
	{
		res_form = &res_form_alert_0;
		res_form->bx = 85;
		res_form->by = 35;
		res_form->bw = 64;
		res_form->bh = 16;
	} else if(pipes == 1) {
		res_form = &res_form_alert_schade;
		res_form->bx = 88;
		res_form->by = 48;
		res_form->bw = 62;
		res_form->bh = 22;
	} else if(pipes == 2) {
		res_form = &res_form_alert_3;
		res_form->bx = 8;
		res_form->by = 64;
		res_form->bw = 246;
		res_form->bh = 22;
	} else if(pipes == 3) {
		res_form = &res_form_alert_4;
		res_form->bx = 8;
		res_form->by = 80;
		res_form->bw = 246;
		res_form->bh = 22;
	} else if(pipes == 4) {
		res_form = &res_form_alert_5;
		res_form->bx = 8;
		res_form->by = 96;
		res_form->bw = 246;
		res_form->bh = 22;
	}

	if(res_form!=NULL)
	{
		int ox = (640-res_form->w)/2;
		int oy = (400-res_form->h)/2;
		int o = 4 * (oy * 640 + ox);

		res_form->x = ox;
		res_form->y = oy;
		_form_dial_screen = new byte[res_form->w*res_form->h];
		handle->SaveRectangle(ox, oy, ox+res_form->w, oy+res_form->h, _form_dial_screen);

		for(int y = 0;y<res_form->h;y++) {
			for(int x = 0;x<res_form->w;x++)
			{
				int index1 = 4 * (y * 640 + x);
				int index2 = y * res_form->w + x;
				handle->pixels[index1+o] = res_form->pixels[index2];
				handle->pixels[index1+o+1] = res_form->pixels[index2];
				handle->pixels[index1+o+2] = res_form->pixels[index2];
				handle->pixels[index1+o+3] = 255;
			}
		}

		i = 4;
		char ch = alertstr[i++];
		int zeile = 0;
		int x = ox + 19;
		int y = oy + 6;
		while(ch != NULL)
		{
			if(ch == '|')
			{
				x = ox + 19;
				y += 16;
			}
			else if(ch == ']')
			{
				break;
			}
			else
			{
				short w = 8;
				if(alertstr[i] == '|') w = 7;
				_drawChar(ch,0,x,y+zeile*16,w,16);
				x+=8;
			}
			ch = alertstr[i++];
		}
	}
}

#define char_numbers 83 //26*3+5
char chars[char_numbers] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '_', '?', '!', '>', '<', 'ä', 'ö', 'ü', 'Ä', 'Ö', 'Ü', ':', '.','\'', ',',
	'%','-','{','(',')'};

void objc_draw(int tree, int obj, int depth, int bx,int by,int bw,int bh)
{
	ressource* r = (ressource *)tree;
	bool full = false;
	if(bw == 640 && bh == 400)
	{
		full = true;
	}
	if(full)
	{
		int o = 4 * (r->y * bw + r->x);
		for(int y = 0;y<r->h;y++) {
			for(int x = 0;x<r->w;x++)
			{
				int index1 = 4 * (y * bw + x);
				int index2 = y * r->w + x;
				handle->pixels[index1+o] = r->pixels[index2];
				handle->pixels[index1+o+1] = r->pixels[index2];
				handle->pixels[index1+o+2] = r->pixels[index2];
				handle->pixels[index1+o+3] = 255;
			}
		}
	}
	if(r->tree == DRAN)
	{
		ressource_dran* r1 = (ressource_dran*)tree;
		char* name = (char*) r1->dnam;
		_drawChars(name,20,0,0,r1->x + 16,r1->y + 7,16);
		char* wind = (char*) r1->dwin;
		_drawChars(wind,6,6,0,r1->x + 15, r1->y + 67, 16);
	}
	if(r->tree == SCHUSS) 
	{
		ressource_schuss* r1 = (ressource_schuss *)tree;
		if(!full)
		{
			color(0);
			short xy[4];
			xy[0]=bx; xy[1]=by; xy[2]=bx+bw-2; xy[3]=by+bh-2;
			v_bar( handle,xy );
		}
		_drawChars(r1->wink, 3, 5,0, 324, 112, 16);
		_drawChars(r1->pulv, 3, 5,0, 324, 141, 16);
	}
	if(r->tree == COMPUTER)
	{
		ressource_computer* r1 = (ressource_computer *)tree;
		short c = 0;
		if(r1->cn1_0 == 1) c = 0;
		else if(r1->cn1_1 == 1) c = 1;
		else if(r1->cn1_2 == 1) c = 2;
		else if(r1->cn1_3 == 1) c = 3;
		else if(r1->cn1_4 == 1) c = 4;
		else if(r1->cn1_5 == 1) c = 5;
		else if(r1->cn1_6 == 1) c = 6;
		handle->InvertRectangle(r1->x+16, r1->y + 41 + c * 18, r1->x + 118, r1->y + 57 + c * 18);

		c = 0;
		if(r1->cn2_0 == 1) c = 0;
		else if(r1->cn2_1 == 1) c = 1;
		else if(r1->cn2_2 == 1) c = 2;
		else if(r1->cn2_3 == 1) c = 3;
		else if(r1->cn2_4 == 1) c = 4;
		else if(r1->cn2_5 == 1) c = 5;
		else if(r1->cn2_6 == 1) c = 6;
		handle->InvertRectangle(r1->x+138, r1->y + 41 + c * 18, r1->x + 240, r1->y + 57 + c * 18);

		c = 0;
		if(r1->cts1_0 == 1) c = 0;
		else if(r1->cts1_1 == 1) c = 1;
		else if(r1->cts1_2 == 1) c = 2;
		else if(r1->cts1_3 == 1) c = 3;
		handle->InvertRectangle(r1->x+16+c*26, r1->y + 175, r1->x + 40+c*26, r1->y + 191);

		c = 0;
		if(r1->cts2_0 == 1) c = 0;
		else if(r1->cts2_1 == 1) c = 1;
		else if(r1->cts2_2 == 1) c = 2;
		else if(r1->cts2_3 == 1) c = 3;
		handle->InvertRectangle(r1->x+138+c*26, r1->y + 175, r1->x + 162+c*26, r1->y + 191);
	}
	if(r->tree == OPTION)
	{
		ressource_option* r1 = (ressource_option *)tree;
		short c = 0;
		if(r1->a_erl == 1) c = 0;
		else if(r1->a_ver == 1) c = 1;
		handle->InvertRectangle(r1->x+131+c*88, r1->y + 51, r1->x + 211+c*96, r1->y + 67);

		c = -1;
		if(r1->max_1 == 1) c = 0;
		else if(r1->max_2 == 1) c = 1;
		else if(r1->max_xx == 1) c = 2;
		else if(r1->max_ne == 1) c = 3;
		if(c != -1) {
			if(c == 3) {
				handle->InvertRectangle(r1->x+219, r1->y + 133, r1->x + 315, r1->y + 149);
			} else {
				handle->InvertRectangle(r1->x+27+c*64, r1->y + 133, r1->x + 83+c*64, r1->y + 149);
			}
		}

		if(r1->a_kap == 1) {
			handle->InvertRectangle(r1->x+27, r1->y + 187, r1->x +315, r1->y + 203);
		}
	}
	if(r->tree == STATUS)
	{
		ressource_status* r1 = (ressource_status *)tree;
		_drawChars(r1->sh1, 16, 0,0, r1->x + 24, r1->y + 32, 16);
		_drawChars(r1->sh2, 16, 0,0, r1->x + 208, r1->y + 32, 16);
		_drawChars(r1->sh3, 16, 0,0, r1->x + 24, r1->y + 48, 16);
		_drawChars(r1->sh4, 16, 0,0, r1->x + 208, r1->y + 48, 16);
		_drawChars(r1->sh5, 16, 0,0, r1->x + 24, r1->y + 64, 16);
		_drawChars(r1->sh6, 16, 0,0, r1->x + 208, r1->y + 64, 15);
		_drawChars(r1->sh7, 16, 0,0, r1->x + 24, r1->y + 80, 16);

		_drawChars(r1->sh8, 12, 12,0,r1->x + 208, r1->y + 80, 16);
		_drawChars(r1->sh8+13, 3, 0,1, r1->x + 208 + 13 * 8, r1->y + 80, 16);
		
		res_actual_font = r1->sm1_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm1, 16, 0,0, r1->x + 24, r1->y + 128, 16);
		
		res_actual_font = r1->sm2_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm2, 16, 0,0, r1->x + 208, r1->y + 128, 16);
		
		res_actual_font = r1->sm3_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm3, 16, 0,0, r1->x + 24, r1->y + 144, 16);
		
		res_actual_font = r1->sm4_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm4, 16, 0,0, r1->x + 208, r1->y + 144, 16);
		
		res_actual_font = r1->sm5_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm5, 16, 0,0, r1->x + 24, r1->y + 160, 16);
		
		res_actual_font = r1->sm6_flags == 8 ? &res_font_disabled : &res_font;
		_drawChars(r1->sm6, 16, 0,0, r1->x + 208, r1->y + 160, 16);
		
		res_actual_font = &res_font;

	}
	if(r->tree == SIEGER)
	{
		ressource_sieger* r1 = (ressource_sieger*)tree;
		char* sg1 = (char*) r1->sg1;
		char* sg2 = (char*) r1->sg2;
		char* sg3 = (char*) r1->sg3;
		clr(r1->x + 65,r1->y + 69,403,21);
		clr(r1->x + 65,r1->y + 100,403,41);
		_drawChars(sg1,50,50,0,r1->x + 65,r1->y + 71,16);
		_drawChars(sg2,50,0,0,r1->x + 65,r1->y + 104,16);
		_drawChars(sg3,50,0,0,r1->x + 65,r1->y + 121,16);
	}
	if(r->tree == NAMEN)
	{
		ressource_namen* r1 = (ressource_namen*)tree;
		//_drawChars(r1->Name1, 20, 0, 0, r1->x+129,r1->y+52, 16);
		//_drawChars(r1->Name1, 20, 0, 0, r1->x+129,r1->y+52, 16);
	}
	if(r->tree == BURG)
	{
		ressource_burg* r1 = (ressource_burg*)tree;
		char* bsp1 = (char*) r1->bsp1;
		clr(r1->x + 19,r1->y + 35,249,17);
		clr(r1->x + 275,r1->y + 35,249,17);
		_drawChars(bsp1,22,22,0,r1->x + 55,r1->y + 35,16);
		char* bsp2 = (char*) r1->bsp2;
		_drawChars(bsp2,22,22,0,r1->x + 311,r1->y + 35,16);
	}
	if(r->tree == EINTRAG)
	{
		ressource_eintrag* r1 = (ressource_eintrag *)tree;

		/*_drawChars(r1->ei_n1, 7, 0,0, r1->x + 79, r1->y + 50, 16);
		_drawChars(r1->ei_n2, 7, 0,0, r1->x + 207, r1->y + 50, 16);
		_drawChars(r1->ei_n3, 7, 0,0, r1->x + 79, r1->y + 82, 16);
		_drawChars(r1->ei_n4, 7, 0,0, r1->x + 207, r1->y + 82, 16);
		_drawChars(r1->ei_n5, 7, 0,0, r1->x + 79, r1->y + 114, 16);
		_drawChars(r1->ei_n6, 7, 0,0, r1->x + 207, r1->y + 114, 16);*/

		short c = 0;
		if(r1->er_loe == 1) c = 0;
		else if(r1->er_ble == 1) c = 1;
		handle->InvertRectangle(r1->x+43, r1->y + 166 + c*25, r1->x + 275, r1->y + 182 + c*25);
	}

	//short x = *(short *)(tree+obj*24+16);
}

void _drawChars(char* ch, int length, int length2, short leftOffset, short x, short y, short h)
{
	
	if(length2 > 0) {

		int length3 = 0;
		for(int i = 0;i<length;i++)
		{
			if(ch[i] == '\0') break;
			length3++;
		}

		int offset = (length2*8 - length3*8) / 2;

		for(int i = 0;i<length3;i++)
		{
			int w = 8;
			if(i==length3-1) w = 7;
			_drawChar(ch[i],leftOffset, x+offset+i * 8, y, w, h);
		}
	} else {
		for(int i = 0;i<length;i++)
		{
			int w = 8;
			if(i==length-1) w = 7;
			if(ch[i] == '\0') break;
			if(ch[i] == NULL) {
				_drawChar(' ',leftOffset, x+i * 8, y, w, h);
			} else {
				_drawChar(ch[i],leftOffset, x+i * 8, y, w, h);
			}
		}
	}
}

void _drawChar(char ch, short leftOffset, short x, short y, short w, short h) 
{
	int o = 4 * (y * 640 + x);
	if(ch == '\0') {
		return;
	}
	int pos = _findCharPos(ch);

	for(int y2 = 0;y2<h;y2++)
	{
		for(int x2 = 0+leftOffset;x2<w;x2++)
		{
			int index1 = 4 * (y2 * 640 + x2);
			int index2 = pos * 8 + x2 + y2 * res_actual_font->w;
			handle->pixels[index1+o] = res_actual_font->pixels[index2];
			handle->pixels[index1+o+1] = res_actual_font->pixels[index2];
			handle->pixels[index1+o+2] = res_actual_font->pixels[index2];
			handle->pixels[index1+o+3] = 255;
		}
	}
}

int _findCharPos(char ch)
{
	for(int i = 0;i<char_numbers;i++)
	{
		if(ch == 3) return _findCharPos('>');
		if(ch == 4) return _findCharPos('<');
		if(ch == -127) return _findCharPos('ü');
		if(chars[i] == ch)
		{
			return i;
		}
	}
	throw ref new Platform::FailureException("char not found");
}

void form_dial(short mode, short x1, short y1, short w1, short h1, short x2, short y2, short w2, short h2)
{
	if(mode == 1)
	{
		menu_enabled = false;
		_form_dial_screen = new byte[w2*h2];
		handle->SaveRectangle(x2, y2, x2+w2, y2+h2, _form_dial_screen);
		return;
	}
	if(mode == 2)
	{
		menu_enabled = true;
		handle->RestoreRectangle(x2, y2, x2+w2, y2+h2, _form_dial_screen);
		return;
	}
}

void form_center(int tree, short* x, short* y, short* w, short* h)
{
	ressource* r = (ressource *)tree;

	if(r->tree == SCHUSS) {
		r->x = (640 - r->w) / 2;
		r->y = (400 - r->h) / 2 + 9;
		*w = r->w;
		*h = r->h;
		*x = r->x;
		*y = r->y;
	} else {
		r->x = (640 - r->w) / 2;
		r->y = (400 - r->h) / 2 + 9;
		*w = r->w;
		*h = r->h;
		*x = r->x;
		*y = r->y;
	}
}

void rsrc_gaddr(int type, int index, void* addr)
{
	if(index == MENUE) {
		ressource_menu **r;
		r = (ressource_menu **)addr;
		res_menu.tree = MENUE;
		res_menu.desk_x = 17;
		res_menu.desk_w = 46;
		res_menu.spiel_x = 65;
		res_menu.spiel_w = 54;
		res_menu.modus_x = 121;
		res_menu.modus_w = 54;
		res_menu.regeln_x = 177;
		res_menu.regeln_w = 62;
		res_menu.ergebnisse_w = 94;
		res_menu.ergebnisse_x = 241;
		res_menu.check = 25;
		*r = &res_menu;

		res_menu_desk.x = res_menu.desk_x-2;
		res_menu_spiel.x = res_menu.spiel_x-2;
		res_menu_modus.x = res_menu.modus_x-2;
		res_menu_regeln.x = res_menu.regeln_x-2;
		res_menu_ergebnisse.x = res_menu.ergebnisse_x-2;
		return;
	}
	if(index == INFOTREE)
	{
		ressource **r;
		r = (ressource **)addr;
		res_infotree.tree = INFOTREE;
		*r = &res_infotree;
		return;
	}
	if(index == DRAN) {
		ressource_dran **r;
		r = (ressource_dran **)addr;
		res_dran.tree = DRAN;
		res_dran.x = 5; 
		res_dran.y = 25;
		res_dran.dnam = 111; // TODO
		res_dran.i2 = 222; // TODO
		res_dran.dwin = new char[6];
		res_dran.dwin[1] = ' ';
		res_dran.dwin[4] = ' ';
		
		res_dran.dwbx_x = 80;
		res_dran.dwbx_y = 45;
		res_dran.dwbx_w = 104;
		res_dran.dwbx_h = 48;
		res_dran.w = 191;
		res_dran.h = 126;

		res_dran.dok_x = 7;
		res_dran.dok_y = 101;
		res_dran.dok_ftw = 160;
		res_dran.dok_fth = 16;
		*r = &res_dran;
		return;
	}
	if(index == SCHUSS) {
		ressource_schuss **r;
		r = (ressource_schuss **)addr;
		res_schuss.tree = SCHUSS;
		res_schuss.wink = new char[3];
		res_schuss.pulv = new char[2];
		res_schuss.wink[0] = 'A';
		res_schuss.wink[1] = 'B';
		res_schuss.wink[2] = 'C';
		res_schuss.pulv[0] = 'D';
		res_schuss.pulv[1] = 'E';
		res_schuss.x = 0; 
		res_schuss.y = 0;
		res_schuss.w = 315;
		res_schuss.h = 218;

		*r = &res_schuss;
		return;
	}
	if(index == BURG) {
		ressource_burg** r;
		r = (ressource_burg **)addr;
		res_burg.tree = BURG;
		res_burg.bsp1;
		res_burg.bsp2;
		res_burg.bk1_w = 248;
		res_burg.bk1_h = 128;
		*r = &res_burg;
		return;
	}
	if(index == COMPUTER)
	{
		ressource_computer** r;
		r = (ressource_computer **)addr;
		res_computer.tree = COMPUTER;
		*r = &res_computer;
		return;
	}
	if(index == OPTION)
	{
		ressource_option** r;
		r = (ressource_option **)addr;
		res_option.tree = OPTION;
		char* temp = new char[3];
		temp[0] = '1';
		temp[1] = '0';
		temp[2] = '0';

		res_option.max_xx1 = new int[1];
		res_option.max_xx1[0] = (int)&(*temp);
		*r = &res_option;
		return;
	}
	if(index == STATUS) {
		ressource_status **r;
		r = (ressource_status **)addr;
		res_status.tree = STATUS;
		res_status.sh1 = new char[16];
		res_status.sh2 = new char[16];
		res_status.sh3 = new char[16];
		res_status.sh4 = new char[16];
		res_status.sh5 = new char[16];
		res_status.sh6 = new char[16];
		res_status.sh7 = new char[16];
		res_status.sh8 = new char[16];
		res_status.sm1 = new char[16];
		res_status.sm2 = new char[16];
		res_status.sm3 = new char[16];
		res_status.sm4 = new char[16];
		res_status.sm5 = new char[16];
		res_status.sm6 = new char[16];
		char sh1[16] = {'G','e','l','d',' ',' ',' ',' ',' ',' ',' ','1','0','0','0','0'};	_fillCharArray(res_status.sh1, sh1, 16);
		char sh2[16] = {'F','ö','r','d','e','r','t','ü','r','m','e',' ',' ',' ','1','0'};	_fillCharArray(res_status.sh2, sh2, 16);
		char sh3[16] = {'G','e','s','c','h','ü','t','z','e',' ',' ',' ',' ',' ',' ','5'};	_fillCharArray(res_status.sh3, sh3, 16);
		char sh4[16] = {'W','i','n','f','a','h','n','e',' ',' ',' ',' ',' ',' ',' ','1'};	_fillCharArray(res_status.sh4, sh4, 16);
		char sh5[16] = {'P','u','l','v','e','r',' ',' ',' ',' ',' ',' ',' ','1','0','0'};	_fillCharArray(res_status.sh5, sh5, 16);
		char sh6[16] = {'K','u','g','e','l','n',' ',' ',' ',' ',' ',' ',' ',' ','1','5'};	_fillCharArray(res_status.sh6, sh6, 16);
		char sh7[16] = {'V','o','l','k',' ',' ',' ',' ',' ',' ',' ',' ',' ','2','0','0'};	_fillCharArray(res_status.sh7, sh7, 16);
		char sh8[16] = {'S','t','e','u','e','r','n',' ','i','n','%',' ',' ','1','0','0'};	_fillCharArray(res_status.sh8, sh8, 16);
		char sm1[16] = {'A','n','b','a','u','e','n',' ',' ',' ',' ',' ',' ','1','3','0'};	_fillCharArray(res_status.sm1, sm1, 16);
		char sm2[16] = {'F','ö','r','d','e','r','t','u','r','m',' ',' ',' ','8','7','6'};	_fillCharArray(res_status.sm2, sm2, 16);
		char sm3[16] = {'G','e','s','c','h','ü','t','z',' ',' ',' ',' ',' ','1','8','0'};	_fillCharArray(res_status.sm3, sm3, 16);
		char sm4[16] = {'W','i','n','d','f','a','h','n','e',' ',' ',' ',' ','1','5','8'};	_fillCharArray(res_status.sm4, sm4, 16);
		char sm5[16] = {'3','0',' ','P','u','l','v','e','r',' ',' ',' ',' ',' ','9','0'};	_fillCharArray(res_status.sm5, sm5, 16);
		char sm6[16] = {'2',' ','K','u','g','e','l','n',' ',' ',' ',' ',' ',' ','3','0'};	_fillCharArray(res_status.sm6, sm6, 16);
		*r = &res_status;
		return;
	}
	if(index == SIEGER)
	{
		ressource **r;
		r = (ressource **)addr;
		res_sieger.tree = SIEGER;
		*r = &res_sieger;
		return;
	}
	if(index == NAMEN) 
	{
		ressource **r;
		r = (ressource **)addr;
		res_namen.tree = NAMEN;
		res_namen.Name1 = new char[20];
		for(int i = 0;i<20;i++) {
			res_namen.Name1[i] = ' ';
		}
		res_namen.Name2 = new char[20];
		for(int i = 0;i<20;i++) {
			res_namen.Name2[i] = ' ';
		}
		*r = &res_namen;
		return;
	}
	if(index == REGEL1)
	{
		ressource **r;
		r = (ressource **)addr;
		res_regel1.tree = REGEL1;
		*r = &res_regel1;
		return;
	}
	if(index == REGEL2)
	{
		ressource **r;
		r = (ressource **)addr;
		res_regel2.tree = REGEL2;
		*r = &res_regel2;
		return;
	}
	if(index == REGEL3)
	{
		ressource **r;
		r = (ressource **)addr;
		res_regel3.tree = REGEL3;
		*r = &res_regel3;
		return;
	}
	if(index == EINTRAG)
	{
		ressource_eintrag** r;
		r = (ressource_eintrag **)addr;
		res_eintrag.tree = EINTRAG;
		res_eintrag.ei_n1 = new char[8];
		res_eintrag.ei_n2 = new char[8];
		res_eintrag.ei_n3 = new char[8];
		res_eintrag.ei_n4 = new char[8];
		res_eintrag.ei_n5 = new char[8];
		res_eintrag.ei_n6 = new char[8];
		for(int i = 0;i<8;i++) res_eintrag.ei_n1[i] = ' ';
		for(int i = 0;i<8;i++) res_eintrag.ei_n2[i] = ' ';
		for(int i = 0;i<8;i++) res_eintrag.ei_n3[i] = ' ';
		for(int i = 0;i<8;i++) res_eintrag.ei_n4[i] = ' ';
		for(int i = 0;i<8;i++) res_eintrag.ei_n5[i] = ' ';
		for(int i = 0;i<8;i++) res_eintrag.ei_n6[i] = ' ';
		*r = &res_eintrag;
		return;
	}
	throw ref new Platform::FailureException("rsrc_gaddr: unknown index " + index);
}

void _fillCharArray(char* dest, char* src, int count)
{
	for(int i = 0;i<count;i++) 
	{
		dest[i] = src[i];
	}
}

void objc_offset(int tree, int obj, short* ox, short* oy) 
{
	ressource * r = (ressource*)tree;
	if(r->tree == DRAN) {
		ressource_dran* r1 = (ressource_dran *)tree;
		if(obj == DOK) {
			*ox = r1->dok_x + r1->x;
			*oy = r1->dok_y + r1->y;
			return;
		}
		if(obj == DWBX) {
			*ox = r1->dwbx_x + r1->x;
			*oy = r1->dwbx_y + r1->y;
			return;
		}
	}
	else if(r->tree == SCHUSS)
	{
		ressource_schuss* r1 = (ressource_schuss *)tree;
		if(obj == WINK)
		{
			*ox = 324;
			*oy = 112;
			return;
		}
		if(obj == PULV)
		{
			*ox = 324;
			*oy = 141;
			return;
		}
		if(obj == KAST) 
		{
			*ox = 179;
			*oy = 167;
			return;
		}
	}
	else if(r->tree == BURG)
	{
		ressource_burg* r1 = (ressource_burg*)tree;
		if(obj == BK1)
		{
			*ox = 69;
			*oy = 140;
			return;
		}
		if(obj == BK2)
		{
			*ox = 325;
			*oy = 140;
			return;
		}
	}
	//*ox = *(short *)(tree+obj*24+16);
	//*oy = *(short *)(tree+obj*24+18);
}

extern short mx, my;
short form_do(int tree, int editobj)
{
	ressource * r = (ressource*)tree;
    short x = mx - r->x;
    short y = my - r->y;
	if(r->tree == SCHUSS) {
        if (drin_(x, y, 111, 11, 26, 18)) {
            return WL2;
        }
        if (drin_(x, y, 139, 11, 20, 18)) {
            return WL1;
        }
        if (drin_(x, y, 205, 11, 20, 18)) {
            return WR1;
        }
        if (drin_(x, y, 227, 11, 26, 18)) {
            return WR2;
        }

        if (drin_(x, y, 111, 40, 26, 18)) {
            return PL2;
        }
        if (drin_(x, y, 139, 40, 20, 18)) {
            return PL1;
        }
        if (drin_(x, y, 205, 40, 20, 18)) {
            return PR1;
        }
        if (drin_(x, y, 227, 40, 26, 18)) {
            return PR2;
        }

        if (drin_(x, y, 235, 160, 68, 20)) {
            return SOK;
        }
        if (drin_(x, y, 235, 187, 68, 20)) {
            return SAB;
        }
		return -1;
	}
	if(r->tree == STATUS) {
		ressource_status* r2 = (ressource_status*)tree;
		if (r2->sm1_flags!=8 && drin_(x, y, 24, 128, 129, 16)) {
			return SM1;
		}
		if (r2->sm2_flags!=8 &&drin_(x, y, 207, 128, 129, 16)) {
			return SM2;
		}
		if (r2->sm3_flags!=8 &&drin_(x, y, 24, 144, 129, 16)) {
			return SM3;
		}
		if (r2->sm4_flags!=8 &&drin_(x, y, 207, 144, 129, 16)) {
			return SM4;
		}
		if (r2->sm5_flags!=8 &&drin_(x, y, 24, 160, 129, 16)) {
			return SM5;
		}
		if (r2->sm6_flags!=8 &&drin_(x, y, 207, 160, 129, 16)) {
			return SM6;
		}
		if (drin_(x, y, 303, 79, 10, 18)) {
			return SHK;
		}
		if (drin_(x, y, 335, 79, 10, 18)) {
			return SHG;
		}
		if (drin_(x, y, 5, 189, 350, 21)) {
			return FERTIG;
		}
		return -1;
	}
	if(r->tree == BURG) {
		if (drin_(x, y, 17, 193, 124, 20)) {
			return BL1;
		}
		if (drin_(x, y, 145, 193, 124, 20)) {
			return BR1;
		}
		if (drin_(x, y, 273, 193, 124, 20)) {
			return BL2;
		}
		if (drin_(x, y, 401, 193, 124, 20)) {
			return BR2;
		}
		if (drin_(x, y, 384, 224, 70, 22)) {
			return BOK;
		}
		if (drin_(x, y, 457, 224, 68, 20)) {
			return BAB;
		}
		return -1;
	}
	if(r->tree == COMPUTER) {
		if(drin_(x, y,16,41, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_0 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,59, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_1 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,77, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_2 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,95, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_3 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,113, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_4 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,131, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_5 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,16,149, 102,16)) {
			res_computer.cn1_0 = res_computer.cn1_1 = res_computer.cn1_2 = res_computer.cn1_3 = res_computer.cn1_4 = res_computer.cn1_5 = res_computer.cn1_6 = 0;
			res_computer.cn1_6 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,138,41, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_0 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,59, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_1 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,77, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_2 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,95, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_3 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,113, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_4 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,131, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_5 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,138,149, 102,16)) {
			res_computer.cn2_0 = res_computer.cn2_1 = res_computer.cn2_2 = res_computer.cn2_3 = res_computer.cn2_4 = res_computer.cn2_5 = res_computer.cn2_6 = 0;
			res_computer.cn2_6 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,16,175, 24,16)) {
			res_computer.cts1_0 = res_computer.cts1_1 = res_computer.cts1_2 = res_computer.cts1_0 = 3;
			res_computer.cts1_0 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,42,175, 24,16)) {
			res_computer.cts1_0 = res_computer.cts1_1 = res_computer.cts1_2 = res_computer.cts1_0 = 3;
			res_computer.cts1_1 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,68,175, 24,16)) {
			res_computer.cts1_0 = res_computer.cts1_1 = res_computer.cts1_2 = res_computer.cts1_0 = 3;
			res_computer.cts1_2 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,94,175, 24,16)) {
			res_computer.cts1_0 = res_computer.cts1_1 = res_computer.cts1_2 = res_computer.cts1_0 = 3;
			res_computer.cts1_3 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,138,175, 24,16)) {
			res_computer.cts2_0 = res_computer.cts2_1 = res_computer.cts2_2 = res_computer.cts2_0 = 3;
			res_computer.cts2_0 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,164,175, 24,16)) {
			res_computer.cts2_0 = res_computer.cts2_1 = res_computer.cts2_2 = res_computer.cts2_0 = 3;
			res_computer.cts2_1 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,190,175, 24,16)) {
			res_computer.cts2_0 = res_computer.cts2_1 = res_computer.cts2_2 = res_computer.cts2_0 = 3;
			res_computer.cts2_2 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,216,175, 24,16)) {
			res_computer.cts2_0 = res_computer.cts2_1 = res_computer.cts2_2 = res_computer.cts2_0 = 3;
			res_computer.cts2_3 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,16,208, 224,16)) {
			return 0;
		}
		return -1;
	}
	if(r->tree == OPTION) {
		if(drin_(x, y,131,51, 80,16)) {
			res_option.a_erl = 1; res_option.a_ver = 0;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,219,51, 80,16)) {
			res_option.a_erl = 0; res_option.a_ver = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,27,133, 56,16)) {
			res_option.max_1 = res_option.max_2 = res_option.max_xx = res_option.max_ne = 0;
			res_option.max_1 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,91,133, 56,16)) {
			res_option.max_1 = res_option.max_2 = res_option.max_xx = res_option.max_ne = 0;
			res_option.max_2 = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,155,133, 56,16)) {
			res_option.max_1 = res_option.max_2 = res_option.max_xx = res_option.max_ne = 0;
			res_option.max_xx = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,219,133, 96,16)) {
			res_option.max_1 = res_option.max_2 = res_option.max_xx = res_option.max_ne = 0;
			res_option.max_ne = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,27,187, 288,16)) {
			res_option.a_kap ^= 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}

		if(drin_(x, y,179,227, 64,16)) {
			return OP_OK;
		}

		if(drin_(x, y,259,227, 64,16)) {
			return 0;
		}
		return -1;
	}
	if(r->tree == NAMEN) {
		if(drin_(x, y,171,115, 64,16)) {
			return NOK;
		}
		if(drin_(x, y,251,115, 64,16)) {
			return 0;
		}
		return -1;
	}
	if(r->tree == INFOTREE) {
		if(drin_(x, y,424,304, 166,23)) {
			return 0;
		}
		return -1;
	}
	if(r->tree == REGEL1) {
		if(drin_(x, y, 225, 305, 116, 20)) {
			return R12;
		}
		if(drin_(x, y, 353, 305, 116, 20)) {
			return R13;
		}
		if(drin_(x, y, 481, 305, 116, 20)) {
			return R1F;
		}
		return -1;
	}
	if(r->tree == REGEL2) {
		if(drin_(x, y, 225, 305, 116, 20)) {
			return R23;
		}
		if(drin_(x, y, 353, 305, 116, 20)) {
			return R21;
		}
		if(drin_(x, y, 481, 305, 116, 20)) {
			return R2F;
		}
		return -1;
	}
	if(r->tree == REGEL3) {
		if(drin_(x, y, 225, 305, 116, 20)) {
			return R31;
		}
		if(drin_(x, y, 353, 305, 116, 20)) {
			return R32;
		}
		if(drin_(x, y, 481, 305, 116, 20)) {
			return R3F;
		}
		return -1;
	}
	if(r->tree == EINTRAG)
	{
		if(drin_(x, y,43,166, 232,16)) {
			res_eintrag.er_loe = 1; res_eintrag.er_ble = 0;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y,43,191, 232,16)) {
			res_eintrag.er_loe = 0; res_eintrag.er_ble = 1;
			objc_draw(tree,0,5,0,0,640,400);
			return -1;
		}
		if(drin_(x, y, 147,227,64,16)) {
			return ER_OK;
		}
		if(drin_(x, y, 227,227,64,16)) {
			return 0;
		}
		return -1;
	}

	throw ref new Platform::FailureException("form_do not implemented");
}

bool drin_(short x, short y, short x1, short y1, short w, short h) {
    return (x >= x1 && x <= x1 + w && y >= y1 && y <= y1 + h);
}

void menu_bar(int tree, short mode)
{
	ressource_menu* r = (ressource_menu *)tree;
	
	for(int y = 0;y<20;y++) {
		for(int x = 0;x<r->w;x++)
		{
			int index1 = 4 * (y * 640 + x);
			int index2 = y * r->w + x;
			handle->pixels[index1] = r->pixels[index2];
			handle->pixels[index1+1] = r->pixels[index2];
			handle->pixels[index1+2] = r->pixels[index2];
			handle->pixels[index1+3] = 255;
		}
	}
}

void Dosound(char* cmdlist)
{
	/*for(int i = 0;i<20;i+=2)
	{
		int address = (unsigned char)cmdlist[i];
		int value = (unsigned char)cmdlist[i+1];
		if(address == 255) continue;
		if(address == 8) value -= 6;
		_sound->Write(address, value);
	}*/
	if(cmdlist[5] == 0) m_audio.PlaySoundEffect(Baller0Event);
	else if(cmdlist[5] == 1) m_audio.PlaySoundEffect(Baller1Event);
	else if(cmdlist[5] == 2) m_audio.PlaySoundEffect(Baller2Event);
	else if(cmdlist[5] == 3) m_audio.PlaySoundEffect(Baller3Event);
	else if(cmdlist[5] == 4) m_audio.PlaySoundEffect(Baller4Event);
	else if(cmdlist[5] == 5) m_audio.PlaySoundEffect(Baller5Event);
	else if(cmdlist[5] == 6) m_audio.PlaySoundEffect(Baller6Event);
	else if(cmdlist[5] == 7) m_audio.PlaySoundEffect(Baller7Event);
	else if(cmdlist[5] == 8) m_audio.PlaySoundEffect(Baller8Event);
	else if(cmdlist[5] == 9) m_audio.PlaySoundEffect(Baller9Event);
	else if(cmdlist[5] == 10) m_audio.PlaySoundEffect(Baller10Event);
	else if(cmdlist[5] == 11) m_audio.PlaySoundEffect(Baller11Event);
	else if(cmdlist[5] == 12) m_audio.PlaySoundEffect(Baller12Event);
	else if(cmdlist[5] == 13) m_audio.PlaySoundEffect(Baller13Event);
	else if(cmdlist[5] == 14) m_audio.PlaySoundEffect(Baller14Event);
	else if(cmdlist[5] == 15) m_audio.PlaySoundEffect(Baller15Event);
	else if(cmdlist[5] == 16) m_audio.PlaySoundEffect(Baller16Event);
	else if(cmdlist[5] == 17) m_audio.PlaySoundEffect(Baller17Event);
	else if(cmdlist[5] == 18) m_audio.PlaySoundEffect(Baller18Event);
	else if(cmdlist[5] == 19) m_audio.PlaySoundEffect(Baller19Event);
	else if(cmdlist[5] == 20) m_audio.PlaySoundEffect(Baller20Event);
	else if(cmdlist[5] == 21) m_audio.PlaySoundEffect(Baller21Event);
	else if(cmdlist[5] == 22) m_audio.PlaySoundEffect(Baller22Event);
	else if(cmdlist[5] == 23) m_audio.PlaySoundEffect(Baller23Event);
	else if(cmdlist[5] == 24) m_audio.PlaySoundEffect(Baller24Event);
	else if(cmdlist[5] == 25) m_audio.PlaySoundEffect(Baller25Event);
	else if(cmdlist[5] == 26) m_audio.PlaySoundEffect(Baller26Event);
	else if(cmdlist[5] == 27) m_audio.PlaySoundEffect(Baller27Event);
	else if(cmdlist[5] == 28) m_audio.PlaySoundEffect(Baller28Event);
	else if(cmdlist[5] == 29) m_audio.PlaySoundEffect(Baller29Event);
	else if(cmdlist[5] == 30) m_audio.PlaySoundEffect(Baller30Event);
	else if(cmdlist[5] == 31) m_audio.PlaySoundEffect(Baller31Event);
}

extern PsgDeviceChannel* _psgDevice;
void Giaccess(int data, int register1)
{
	//_sound->Write(register1 & 127,data);
	_psgDevice->WriteRegister(register1 & 127,data);
}

void bing() 
{
	m_audio.PlaySoundEffect(BingEvent);
}

void menu_icheck(int tree, short obj, short check)
{
	ressource_menu* r = (ressource_menu *)tree;
	if(check == 0) r->check = 0;
	else r->check = obj;
}

void m_musik()
{
	m_audio.PlaySoundEffect(EndeEvent);
}
