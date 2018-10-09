//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include <time.h>
#include <Atari.h>
#include <Baller.h>

using namespace Ballerburg;

using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Store;

extern WriteableBitmapEx* handle;
extern Sound^ _sound;
extern Audio m_audio;
PsgDeviceChannel* _psgDevice;

extern byte* pixelsDran;
extern ressource_dran res_dran;
extern ressource_burg res_burg;
extern ressource_eintrag res_eintrag;
extern ressource_status res_status;
extern ressource_sieger res_sieger;
extern ressource_schuss res_schuss;
extern ressource_computer res_computer;
extern ressource_option res_option;
extern ressource res_font;
extern ressource res_font_disabled;
extern ressource res_gewonnen;
extern ressource res_verloren;
extern ressource res_infotree;
extern ressource res_regel1;
extern ressource res_regel2;
extern ressource res_regel3;
extern ressource_form res_form_alert_3;
extern ressource_form res_form_alert_4;
extern ressource_form res_form_alert_5;
extern ressource_form res_form_alert_schade;
extern ressource_form res_form_alert_0;
extern ressource_menu res_menu;
extern ressource_menu_desk res_menu_desk;
extern ressource_menu_spiel res_menu_spiel;
extern ressource_menu_modus res_menu_modus;
extern ressource_menu_regeln res_menu_regeln;
extern ressource_menu_ergebnisse res_menu_ergebnisse;
extern ressource_namen res_namen;



DirectXPage::DirectXPage() /*:
	m_renderNeeded(true),
	m_lastPointValid(false)*/
{
		InitializeComponent();
	//Application::Current->DebugSettings->EnableFrameRateCounter = true;
	Loaded += ref new RoutedEventHandler(this, &DirectXPage::OnMainPageLoaded);

	_frequency = 200;
	_counter = 0;
}


/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void DirectXPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}

void DirectXPage::OnMainPageLoaded(Object^ sender, RoutedEventArgs^ args)
{
	m_audio.Initialize();
	m_audio.CreateResources();
	filter = new BiquadFilterChannel();
	m_audio.SetChannel(filter);

	MasterChannel* master3 = new MasterChannel();
	_psgDevice = new PsgDeviceChannel();
	_psgDevice->SetMode(PsgDeviceChannel::MODE_SIGNED);
	_psgDevice->SetDevice(PsgDeviceChannel::DEVICE_AY_3_8910);
	master3->AddChannel(_psgDevice);

	filter->SetChannel(master3);

	if (!m_audio.m_isAudioStarted)
    {
        m_audio.Start();
    }

	LoadResx("BURG.png", &res_burg).then([this]() {
	LoadResx("EINTRAGweiss.png", &res_eintrag).then([this]() {
	LoadResx("COMPUTER.png", &res_computer).then([this]() {
	LoadResx("OPTION.png", &res_option).then([this]() {
	LoadResx("INFOTREE.png", &res_infotree).then([this]() {
	LoadResx("REGEL1.png", &res_regel1).then([this]() {
	LoadResx("REGEL2.png", &res_regel2).then([this]() {
	LoadResx("REGEL3.png", &res_regel3).then([this]() {
	LoadResx("form_alert_3.png", &res_form_alert_3).then([this]() {
	LoadResx("form_alert_4.png", &res_form_alert_4).then([this]() {
	LoadResx("form_alert_5.png", &res_form_alert_5).then([this]() {
	LoadResx("form_alert_schade.png", &res_form_alert_schade).then([this]() {
	LoadResx("form_alert_0.png", &res_form_alert_0).then([this]() {
	LoadResx("Menu.png", &res_menu).then([this]() {
	LoadResx("Menu_Desk.png", &res_menu_desk).then([this]() {
	LoadResx("Menu_Spiel.png", &res_menu_spiel).then([this]() {
	LoadResx("Menu_Modus.png", &res_menu_modus).then([this]() {
	LoadResx("Menu_Regeln.png", &res_menu_regeln).then([this]() {
	LoadResx("Menu_Ergebnisse.png", &res_menu_ergebnisse).then([this]() {
	LoadResx("font.png", &res_font).then([this]() {
	LoadResx("font_disabled.png", &res_font_disabled).then([this]() {
	LoadResx("GEWONNEN.png", &res_gewonnen).then([this]() {
	LoadResx("VERLOREN.png", &res_verloren).then([this]() {
	LoadResx("STATUS.png", &res_status).then([this]() {
	LoadResx("SIEGER.png", &res_sieger).then([this]() {
	LoadResx("SCHUSS.png", &res_schuss).then([this]() {
	LoadResx("NAMENweiss.png", &res_namen).then([this]() {
	}).then([this]() {
		LoadResx("DRAN.png", &res_dran).then([this]() {

			handle = new WriteableBitmapEx(_width, _height);
		
			_sound = ref new Sound();
			image->Source = handle->Bitmap;

			srand(static_cast<unsigned int>(time(nullptr)));
			//_sound->Start();
			//baller(1);
			//m_audio.PlaySoundEffect(Baller0Event);
			main1();
			CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnCompositionTargetRendering);
		});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
}

task<void> DirectXPage::LoadResx(Platform::String^ file, ressource* res)
{
	Uri^ uri = ref new Uri("ms-appx:///Assets/resx/" + file);
	RandomAccessStreamReference^ streamRef = RandomAccessStreamReference::CreateFromUri(uri);
	
	return task<IRandomAccessStreamWithContentType^> (streamRef->OpenReadAsync()).
		then([res](task<IRandomAccessStreamWithContentType^> thisTask)
	{
		IRandomAccessStreamWithContentType^ fileStream = thisTask.get();
		return BitmapDecoder::CreateAsync(fileStream);
	}).
		then([res](task<BitmapDecoder^> thisTask)
	{
		BitmapDecoder^ decoder = thisTask.get();
		return decoder->GetFrameAsync(0);
	}).
		then([res](task<BitmapFrame^> thisTask)
	{
		BitmapFrame^ frame = thisTask.get();   


		res->w = frame->PixelWidth;
		res->h = frame->PixelHeight;
		// Save some information as fields
		//width = frame->PixelWidth;
		//height = frame->PixelHeight;
		//xCenter = width / 2;
		//yCenter = height / 2;

		return frame->GetPixelDataAsync();
	}).
		then([this, res](task<PixelDataProvider^> thisTask)
	{

		PixelDataProvider^ pixelProvider = thisTask.get();
		srcPixels = pixelProvider->DetachPixelData();

		res->pixels = new byte[res->w*res->h];
		//pixelsDran = new byte[width*height];
		for(int y = 0;y<res->h;y++)
		{
			for(int x = 0;x<res->w;x++)
			{
				int index1 = y * res->w + x;
				int index2 = 4 * index1;
				res->pixels[index1] = srcPixels[index2];
			}
		}


		//schuss(1);
		//main1();
		//CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnCompositionTargetRendering);
	});
}

extern void (*render_callback)();
extern bool _wait;
//short tempp = 0;
//int ii = 0;
//short kk = 0;
bool menu_enabled = true;
bool blocked_by_menu = false;
//unsigned int _ticks = 0;
uint64 _tickCount;
void DirectXPage::OnCompositionTargetRendering(Object^ sender, Object^ args)
{
	m_audio.Render();
	if(render_callback != NULL)
	{
		menu_enabled = false;
		render_callback();
	}
	handle->Render();
	if(_wait && !blocked_by_menu) {
		//_ticks++;
		//if(_ticks >= 120)
		if(::GetTickCount64() - _tickCount >= 2000)
		{
			//_ticks = 0;
			_wait = 0;
			ein_zug();
		}
	}
}

extern short mx;
extern short my;
extern void (*callback)();
extern void (*mouse_move_callback)();
extern byte* _form_dial_screen;
ressource* actualSubmenu;
extern short m_buf[8];

extern char nsp1[22];
extern char nsp2[22];
extern char t_na[6][8];

enm_feedback namen_feedback = fbNone;
enm_feedback eintrag_feedback = fbNone;

void RestoreSubmenu()
{
	handle->RestoreRectangle(actualSubmenu->x,0,actualSubmenu->x+actualSubmenu->w,actualSubmenu->h,_form_dial_screen);
}

void Ballerburg::DirectXPage::image_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	auto pos = e->GetPosition(image);

	int x = pos.X / image->ActualWidth * 640.0;
	int y = pos.Y / image->ActualHeight * 400.0;
	mx = x;
	my = y;

	if(actualSubmenu != NULL) 
	{		
		if(x >= actualSubmenu->x && x <= actualSubmenu->x + actualSubmenu->w && y >= 20 && y < actualSubmenu->h)
		{
			int pos = (y - 19) / 16;
			if(actualSubmenu == &res_menu_desk)
			{
				if(pos == 0) m_buf[4] = INFO;
				RestoreSubmenu();
				event1();
				actualSubmenu = NULL;
				return;
			}
			if(actualSubmenu == &res_menu_spiel)
			{
				if(pos == 0) m_buf[4] = SPI1;
				else if(pos == 1)
				{
					m_buf[4] = SPI2;
					Name1->Visibility = Windows::UI::Xaml::Visibility::Visible;
					Name2->Visibility = Windows::UI::Xaml::Visibility::Visible;
				}
				else if(pos == 2) m_buf[4] = SPI4;

				RestoreSubmenu();
				event1();
				if(Name1->Visibility == Windows::UI::Xaml::Visibility::Visible)
				{
					int len;
					for(len = 0;len<20;len++) {
						if(res_namen.Name1[len] == '\0') break;
					}
					wchar_t *W1 = new wchar_t[len+1];
					int i;
					for(i = 0;i<len;i++) {
						if(res_namen.Name1[i] == '\0') break;
						W1[i] = ConvertChar(res_namen.Name1[i]);
					}
					while(i<len) {
						W1[i++] = ' ';
					}
					W1[len] = '\0';
					Name1->Text = ref new String(W1);

					for(len = 0;len<20;len++) {
						if(res_namen.Name2[len] == '\0') break;
					}
					wchar_t *W2 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_namen.Name2[i] == '\0') break;
						W2[i] = ConvertChar(res_namen.Name2[i]);
					}
					while(i<len) {
						W2[i++] = ' ';
					}
					W2[len] = '\0';
					Name2->Text = ref new String(W2);
				}
				actualSubmenu = NULL;
				return;
			}
			if(actualSubmenu == &res_menu_modus)
			{
				if(pos == 0) m_buf[4] = MOD1;
				else if(pos == 1) m_buf[4] = MOD2;
				else if(pos == 2) m_buf[4] = MOD3;
				else if(pos == 3) m_buf[4] = MOD4;
				else if(pos == 4) return;
				else if(pos == 5) m_buf[4] = MOD5;

				if(pos < 5) blocked_by_menu = false;

				RestoreSubmenu();
				event1();
				actualSubmenu = NULL;
				return;
			}
			if(actualSubmenu == &res_menu_regeln)
			{
				if(pos == 0) m_buf[4] = REG;
				RestoreSubmenu();
				event1();
				actualSubmenu = NULL;
				return;
			}
			if(actualSubmenu == &res_menu_ergebnisse)
			{
				if(pos == 0) 
				{
					m_buf[4] = ERG1;
					ei_n1->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ei_n2->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ei_n3->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ei_n4->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ei_n5->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ei_n6->Visibility = Windows::UI::Xaml::Visibility::Visible;
				}
				else if(pos == 1) m_buf[4] = ERG2;
				else if(pos == 2) m_buf[4] = ERG3;
				else if(pos == 3) return;
				else if(pos == 4) m_buf[4] = ERG4;

				RestoreSubmenu();
				event1();
				if(ei_n1->Visibility == Windows::UI::Xaml::Visibility::Visible)
				{
					int len;
					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n1[len] == '\0') break;
					}
					wchar_t *W1 = new wchar_t[len+1];
					int i;
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n1[i] == '\0') break;
						W1[i] = ConvertChar(res_eintrag.ei_n1[i]);
					}
					while(i<len) {
						W1[i++] = ' ';
					}
					W1[len] = '\0';
					ei_n1->Text = ref new String(W1);

					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n2[len] == '\0') break;
					}
					wchar_t *W2 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n2[i] == '\0') break;
						W2[i] = ConvertChar(res_eintrag.ei_n2[i]);
					}
					while(i<len) {
						W2[i++] = ' ';
					}
					W2[len] = '\0';
					ei_n2->Text = ref new String(W2);

					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n3[len] == '\0') break;
					}
					wchar_t *W3 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n3[i] == '\0') break;
						W3[i] = ConvertChar(res_eintrag.ei_n3[i]);
					}
					while(i<len) {
						W3[i++] = ' ';
					}
					W3[len] = '\0';
					ei_n3->Text = ref new String(W3);

					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n4[len] == '\0') break;
					}
					wchar_t *W4 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n4[i] == '\0') break;
						W4[i] = ConvertChar(res_eintrag.ei_n4[i]);
					}
					while(i<len) {
						W4[i++] = ' ';
					}
					W4[len] = '\0';
					ei_n4->Text = ref new String(W4);

					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n5[len] == '\0') break;
					}
					wchar_t *W5 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n5[i] == '\0') break;
						W5[i] = ConvertChar(res_eintrag.ei_n5[i]);
					}
					while(i<len) {
						W5[i++] = ' ';
					}
					W5[len] = '\0';
					ei_n5->Text = ref new String(W5);

					for(len = 0;len<8;len++) {
						if(res_eintrag.ei_n6[len] == '\0') break;
					}
					wchar_t *W6 = new wchar_t[len+1];
					for(i = 0;i<len;i++) {
						if(res_eintrag.ei_n6[i] == '\0') break;
						W6[i] = ConvertChar(res_eintrag.ei_n6[i]);
					}
					while(i<len) {
						W6[i++] = ' ';
					}
					W6[len] = '\0';
					ei_n6->Text = ref new String(W6);
				}
				actualSubmenu = NULL;
				return;
			}
		}
		else if(y>=20)
		{
			RestoreSubmenu();
			actualSubmenu = NULL;
			blocked_by_menu = false;
		}		
	}

	if(callback != NULL) {
		callback();

		if(namen_feedback != fbNone)
		{
			Name1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			Name2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			if(namen_feedback == fbOK) {
				const wchar_t *W1 = Name1->Text->Data();
				int Size = wcslen( W1 );
				int i;
				for(i = 0;i<Size;i++) {
					nsp1[i] = (char)W1[i];
				}
				nsp1[i] = '\0';
				const wchar_t *W2 = Name2->Text->Data();
				Size = wcslen( W2 );
				for(i = 0;i<Size;i++) {
					nsp2[i] = (char)W2[i];
				}
				nsp2[i] = '\0';

				namen2();
			}
			namen_feedback = fbNone;
		}
		else if(eintrag_feedback != fbNone)
		{
			ei_n1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			ei_n2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			ei_n3->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			ei_n4->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			ei_n5->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			ei_n6->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			if(eintrag_feedback == fbOK) {
				const wchar_t *W1 = ei_n1->Text->Data();
				int Size = wcslen( W1 );
				int i;
				for(i = 0;i<Size;i++) {
					t_na[0][i] = (char)W1[i];
				}
				t_na[0][i] = '\0';
				
				const wchar_t *W2 = ei_n2->Text->Data();
				Size = wcslen( W2 );
				for(i = 0;i<Size;i++) {
					t_na[1][i] = (char)W2[i];
				}
				t_na[1][i] = '\0';

				const wchar_t *W3 = ei_n3->Text->Data();
				Size = wcslen( W3 );
				for(i = 0;i<Size;i++) {
					t_na[2][i] = (char)W3[i];
				}
				t_na[2][i] = '\0';

				const wchar_t *W4 = ei_n4->Text->Data();
				Size = wcslen( W4 );
				for(i = 0;i<Size;i++) {
					t_na[3][i] = (char)W4[i];
				}
				t_na[3][i] = '\0';

				const wchar_t *W5 = ei_n5->Text->Data();
				Size = wcslen( W5 );
				for(i = 0;i<Size;i++) {
					t_na[4][i] = (char)W5[i];
				}
				t_na[4][i] = '\0';

				const wchar_t *W6 = ei_n6->Text->Data();
				Size = wcslen( W6 );
				for(i = 0;i<Size;i++) {
					t_na[5][i] = (char)W6[i];
				}
				t_na[5][i] = '\0';

				eintrag2();
			}
			eintrag_feedback = fbNone;
		}

		if(callback == &ein_zug1)
		{
			blocked_by_menu = false;
		}
	}
}

wchar_t Ballerburg::DirectXPage:: ConvertChar(char ch)
{
	if(ch==-28) return 228;
	if(ch==-10) return 246;
	if(ch==-4) return 252;
	return ch;
}

int mouse_x, mouse_y;
int prev_pos = -1;
void Ballerburg::DirectXPage::image_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	auto pos = e->GetCurrentPoint(image)->Position;
	int x = pos.X / image->ActualWidth * 640.0;
	int y = pos.Y / image->ActualHeight * 400.0;

	if(mouse_move_callback != NULL)
	{
		mx = x;
		my = y;
		mouse_move_callback();
		return;
	}
	if(!menu_enabled) return;

	if(x == mouse_x && y ==  mouse_y) return;
	checkmenu(x,y);
	mouse_x = x;
	mouse_y = y;
	
}

void Menu_InOut(int pos)
{
	if(actualSubmenu == &res_menu_desk)
	{
		if(pos > 0) return;
		handle->InvertRectangle(res_menu_desk.x+1, 19+pos*16, res_menu_desk.x+res_menu_desk.w-1,19+pos*16+16);
		return;
	}
	if(actualSubmenu == &res_menu_spiel)
	{
		if(pos > 2) return;
		handle->InvertRectangle(res_menu_spiel.x+1, 19+pos*16, res_menu_spiel.x+res_menu_spiel.w-1,19+pos*16+16);
		return;
	}
	if(actualSubmenu == &res_menu_modus)
	{
		if(pos == 4 || pos > 5) return;
		handle->InvertRectangle(res_menu_modus.x+1, 19+pos*16, res_menu_modus.x+res_menu_modus.w-1,19+pos*16+16);
		return;
	}
	if(actualSubmenu == &res_menu_regeln)
	{
		if(pos > 0) return;
		handle->InvertRectangle(res_menu_regeln.x+1, 19+pos*16, res_menu_regeln.x+res_menu_regeln.w-1,19+pos*16+16);
		return;
	}
	if(actualSubmenu == &res_menu_ergebnisse)
	{
		if(pos == 3 || pos > 4) return;
		handle->InvertRectangle(res_menu_ergebnisse.x+1, 19+pos*16, res_menu_ergebnisse.x+res_menu_ergebnisse.w-1,19+pos*16+16);
		return;
	}
}

byte* _invertBuffer = new byte[178*16];
void Ballerburg::DirectXPage::checkmenu(short x, short y)
{
	if(y<20)
	{
		blocked_by_menu = true;
		if(x>=res_menu.desk_x && x<=res_menu.desk_x + res_menu.desk_w)
		{
			drawSubmenu(&res_menu_desk);
			return;
		}
		if(x>=res_menu.spiel_x && x<=res_menu.spiel_x + res_menu.spiel_w)
		{
			drawSubmenu(&res_menu_spiel);
			return;
		}
		if(x>=res_menu.modus_x && x<=res_menu.modus_x + res_menu.modus_w)
		{
			drawSubmenu(&res_menu_modus, res_menu.check);
			return;
		}
		if(x>=res_menu.regeln_x && x<=res_menu.regeln_x + res_menu.regeln_w)
		{
			drawSubmenu(&res_menu_regeln);
			return;
		}
		if(x>=res_menu.ergebnisse_x && x<=res_menu.ergebnisse_x + res_menu.ergebnisse_w)
		{
			drawSubmenu(&res_menu_ergebnisse);
			return;
		}
	}
	else
	{
		if(actualSubmenu != NULL && y<actualSubmenu->h) 
		{
			int pos = (y - 19) / 16;

			if(pos != prev_pos) 
			{
				if(prev_pos != -1)
				{
					Menu_InOut(prev_pos);
				}
				Menu_InOut(pos);
				prev_pos = pos;
			}
		}
	}
}

void Ballerburg::DirectXPage::drawSubmenu(ressource* r)
{
	drawSubmenu(r, 0);
}

void Ballerburg::DirectXPage::drawSubmenu(ressource* r, short check)
{
	prev_pos = -1;
	if(actualSubmenu != NULL) 
	{
		handle->RestoreRectangle(actualSubmenu->x,0,actualSubmenu->x+actualSubmenu->w,actualSubmenu->h,_form_dial_screen);
	}
	actualSubmenu = r;
	_form_dial_screen = new byte[r->w*r->h];
	handle->SaveRectangle(r->x,0,r->x+r->w,r->h,_form_dial_screen);
	int o = 4 * (0 * 640 + r->x);
	for(int y = 0;y<r->h;y++) {
		for(int x = 0;x<r->w;x++)
		{
			int index1 = 4 * (y * 640 + x);
			int index2 = y * r->w + x;
			handle->pixels[index1+o] = r->pixels[index2];
			handle->pixels[index1+o+1] = r->pixels[index2];
			handle->pixels[index1+o+2] = r->pixels[index2];
			handle->pixels[index1+o+3] = 255;
		}
	}
	if(check > 0)
	{
		int icon[13] = {1,1,3,3,6,6,140,140,216,88,112,48,32};
		int x = r->x + 3;
		int y = 21 + (check - 25) * 16;
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 8; j++) {
				if (!((icon[i] >> (7-j)) & 1)) continue;
				int index = (x + j + (y + i) * handle->_width) * 4;
				handle->pixels[index] = 0;
				handle->pixels[index+1] = 0;
				handle->pixels[index+2] = 0;
				handle->pixels[index+3] = 255;
			}
		}
	}
}

extern short bt;
void Ballerburg::DirectXPage::image_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	bt = 1;
}

void Ballerburg::DirectXPage::image_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	bt = 0;
}

void Ballerburg::DirectXPage::TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	TextBox^ tb = safe_cast<TextBox^>(sender);
	int start = tb->SelectionStart;
	String^ orig = tb->Text;
	//auto t = orig->Begin();

	std::wstring ws( orig->Data() );
	wchar_t *w1 = new wchar_t[tb->MaxLength+1];
	int j = 0;
	bool changed = false;
	for(int i = 0;i<ws.length();i++)
	{
		if( ws[i] == 'a' || ws[i] == 'b' || ws[i] == 'c' || ws[i] == 'd' || ws[i] == 'e' || ws[i] == 'f' || ws[i] == 'g' || ws[i] == 'h' || ws[i] == 'i' || ws[i] == 'j' || ws[i] == 'k' ||
			ws[i] == 'l' || ws[i] == 'm' || ws[i] == 'n' || ws[i] == 'o' || ws[i] == 'p' || ws[i] == 'q' || ws[i] == 'r' || ws[i] == 's' || ws[i] == 't' || ws[i] == 'u' || ws[i] == 'v' ||
			ws[i] == 'w' || ws[i] == 'x' || ws[i] == 'y' || ws[i] == 'z' ||
			ws[i] == 'A' || ws[i] == 'B' || ws[i] == 'V' || ws[i] == 'D' || ws[i] == 'E' || ws[i] == 'F' || ws[i] == 'G' || ws[i] == 'H' || ws[i] == 'I' || ws[i] == 'J' || ws[i] == 'K' ||
			ws[i] == 'L' || ws[i] == 'M' || ws[i] == 'N' || ws[i] == 'O' || ws[i] == 'P' || ws[i] == 'Q' || ws[i] == 'R' || ws[i] == 'S' || ws[i] == 'T' || ws[i] == 'U' || ws[i] == 'V' ||
			ws[i] == 'W' || ws[i] == 'X' || ws[i] == 'Y' || ws[i] == 'Z' ||
			ws[i] == '1' || ws[i] == '2' || ws[i] == '3' || ws[i] == '4' || ws[i] == '5' || ws[i] == '6' || ws[i] == '7' || ws[i] == '8' || ws[i] == '9' || ws[i] == '0' || 
			ws[i] == '.' || ws[i] == '-' || ws[i] == '_' || ws[i] == 228 /* ä*/ || ws[i] == 246 /* ö */ || ws[i] == 252 /* ü */
			)
		{
			w1[j++] = ws[i];
		} else {
			changed = true;
			continue;
		}
	}
	w1[j] = '\0';
	
	//tb->Text = ref new String(w1);
	//tb->Text = ref new String( ws.c_str() );
	if(changed && start > 0) {
		tb->SelectionStart = start - 1;
	}

}

