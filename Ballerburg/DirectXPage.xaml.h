//
// BlankPage.xaml.h
// Declaration of the BlankPage.xaml class.
//

#pragma once

#include "DirectXPage.g.h"
#include "DirectXBase.h"
#include <WriteableBitmapEx.h>
#include <Atari.h>
#include <Baller1.h>
#include <Baller2.h>
#include <Sound.h>
#include <Audio.h>

namespace Ballerburg
{
	/// <summary>
	/// A DirectX page that can be used on its own.  Note that it may not be used within a Frame.
	/// </summary>
	//[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		

		/*void OnPreviousColorPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnNextColorPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);*/

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		void OnMainPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs ^ args);
		void OnCompositionTargetRendering(Platform::Object^ sender, Platform::Object^ args);

	private:
		/*void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnLogicalDpiChanged(Platform::Object^ sender);
		void OnOrientationChanged(Platform::Object^ sender);
		void OnDisplayContentsInvalidated(Platform::Object^ sender);
		void OnRendering(Object^ sender, Object^ args);

		Windows::Foundation::EventRegistrationToken m_eventToken;

		SimpleTextRenderer^ m_renderer;
		bool m_renderNeeded;

		Windows::Foundation::Point m_lastPoint;
		bool m_lastPointValid;

		BasicTimer^ m_timer;

		Synthesizer^ m_synth;*/

		BiquadFilterChannel* filter;
		
		

		Platform::Array<byte>^ srcPixels;

		Concurrency::task<void> LoadResx(Platform::String^ file, ressource* res);

		static const int _width = 640;
		static const int _height = 400;

		//int width;
		//int height;

		//Sound^ _sound;

		float _frequency;
		int _counter;

		void image_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		
		void drawSubmenu(ressource* r);
		void drawSubmenu(ressource* r, short check);
		void checkmenu(short x, short y);
		void image_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void image_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void image_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

		wchar_t ConvertChar(char ch);
		void btnJa_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void btnNein_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
