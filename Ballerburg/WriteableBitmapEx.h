#pragma once
#include <pch.h>

using namespace Windows::UI::Xaml::Media::Imaging;

#define SizeOfArgb 4

 class WriteableBitmapEx  {
public:
	WriteableBitmapEx(int width, int height);

	void DrawLineBresenham(int x1, int y1, int x2, int y2, byte color);
	void DrawLineBrush(int x1, int y1, int x2, int y2, byte color, int brush);
	void FillPolygon(short* points, int count, byte color);
	void FillPolygonBrush(short* points, int count, byte color, int* brush);
	void DrawPolyline(short* points, int count, byte color);
	void FillRectangle(int x1, int y1, int x2, int y2, byte color);
	void FillRectangleBrush(int x1, int y1, int x2, int y2, int* brush);
	void SaveRectangle(int x1, int y1, int x2, int y2, byte* saveScreen);
	void RestoreRectangle(int x1, int y1, int x2, int y2, byte* saveScreen);
	void InvertRectangle(int x1, int y1, int x2, int y2);

	void CalcRectangle(int x1, int y1, int x2, int y2, int &startYPlusX1, int &endOffset, int &yh);

	void Render();
	WriteableBitmap^ Bitmap;
	byte* pixels;
	int _width;
	int _height;
private:
};