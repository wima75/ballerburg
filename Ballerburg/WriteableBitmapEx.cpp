#include <pch.h>
#include <WriteableBitmapEx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::Streams;


extern int _vswr_mode;

WriteableBitmapEx::WriteableBitmapEx(int width, int height)
{
	_width = width;
	_height = height;
	Bitmap = ref new WriteableBitmap(width, height);

	IBuffer^ buffer = Bitmap->PixelBuffer;

	ComPtr<IBufferByteAccess> pBufferByteAccess;
	ComPtr<IUnknown> pBuffer((IUnknown*)buffer);
	pBuffer.As(&pBufferByteAccess);

	pBufferByteAccess->Buffer(&pixels);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			int index = 4 * (y * width + x);
			pixels[index] = 255; // blau
			pixels[index+1] = 255; // grün
			pixels[index+2] = 255; // rot
			pixels[index+3] = 0; // alpha
		}
}

void WriteableBitmapEx::FillRectangle(int x1, int y1, int x2, int y2, byte color)
{
	int w = _width;
	int startYPlusX1;
	int endOffset;
	int yh;
	CalcRectangle(x1, y1, x2, y2, startYPlusX1, endOffset, yh);
	int index2;
	for (int x = startYPlusX1; x < endOffset; x++) {
	    index2 = x << 2;
	    for (int y = 0; y <yh; y ++) {
	        pixels[index2 + w * 4 * y] = color;
	        pixels[index2 + 1 + w * 4 * y] = color;
	        pixels[index2 + 2 + w * 4 * y] = color;
	        pixels[index2 + 3 + w * 4 * y] = 255;
	    }
	}
}

void WriteableBitmapEx::FillRectangleBrush(int x1, int y1, int x2, int y2, int* brush)
{
	int w = _width;
	int startYPlusX1;
	int endOffset;
	int yh;
	CalcRectangle(x1, y1, x2, y2, startYPlusX1, endOffset, yh);
	int index2;
	int x3 = 0;
	for (int x = startYPlusX1; x < endOffset; x++) {
		int y3 = 0;
	    index2 = x << 2;
	    for (int y = 0; y <yh; y ++) {
			int xx = 8 - ((x3) % 8);
			int mask = brush[y3 % 8];
			byte c = 255 * (((mask & (1 << (xx))) >> (xx)) ^ 1);
	        pixels[index2 + w * 4 * y] = c;
	        pixels[index2 + 1 + w * 4 * y] = c;
	        pixels[index2 + 2 + w * 4 * y] = c;
	        pixels[index2 + 3 + w * 4 * y] = 255;
			y3++;
	    }
		x3++;
	}
}

void WriteableBitmapEx::CalcRectangle(int x1, int y1, int x2, int y2, int &startYPlusX1, int &endOffset, int &yh)
{
	// Use refs for faster access (really important!) speeds up a lot!
	int w = _width;
	int h = _height;

	// Check boundaries
	if ((x1 < 0 && x2 < 0) || (y1 < 0 && y2 < 0)
		|| (x1 >= w && x2 >= w) || (y1 >= h && y2 >= h)) {
	    return;
	}

	// Clamp boundaries
	if (x1 < 0) { x1 = 0; }
	if (y1 < 0) { y1 = 0; }
	if (x2 < 0) { x2 = 0; }
	if (y2 < 0) { y2 = 0; }
	if (x1 >= w) { x1 = w - 1; }
	if (y1 >= h) { y1 = h - 1; }
	if (x2 >= w) { x2 = w - 1; }
	if (y2 >= h) { y2 = h - 1; }

	// Fill first line
	int startY = y1 * w;
	startYPlusX1 = startY + x1;
	endOffset = startY + x2;
	int index2;

	yh = abs(y2 - y1);

	//int len = (x2 - x1) * SizeOfArgb;
	//int srcOffsetBytes = startYPlusX1 * SizeOfArgb;
	//int offset2 = y2 * w + x1;
}

void WriteableBitmapEx::SaveRectangle(int x1, int y1, int x2, int y2, byte* saveScreen) 
{
	int w = _width;
	int startYPlusX1;
	int endOffset;
	int yh;
	CalcRectangle(x1, y1, x2, y2, startYPlusX1, endOffset, yh);
	int index2;
	int index1 = 0;
	for (int x = startYPlusX1; x < endOffset; x++) {
	    index2 = x << 2;
	    for (int y = 0; y <yh; y ++) {
			saveScreen[index1++] = pixels[index2 + w * 4 * y];
	    }
	}
}

void WriteableBitmapEx::RestoreRectangle(int x1, int y1, int x2, int y2, byte* saveScreen) 
{
	int w = _width;
	int startYPlusX1;
	int endOffset;
	int yh;
	CalcRectangle(x1, y1, x2, y2, startYPlusX1, endOffset, yh);
	int index2;
	int index1 = 0;
	for (int x = startYPlusX1; x < endOffset; x++) {
	    index2 = x << 2;
	    for (int y = 0; y <yh; y ++) {
	        pixels[index2 + w * 4 * y] = saveScreen[index1];
	        pixels[index2 + 1 + w * 4 * y] = saveScreen[index1];
	        pixels[index2 + 2 + w * 4 * y] = saveScreen[index1];
	        pixels[index2 + 3 + w * 4 * y] = 255;
			index1++;
	    }
	}
}

void WriteableBitmapEx::InvertRectangle(int x1, int y1, int x2, int y2) 
{
	int w = _width;
	int startYPlusX1;
	int endOffset;
	int yh;
	CalcRectangle(x1, y1, x2, y2, startYPlusX1, endOffset, yh);
	int index2;
	int index1 = 0;
	for (int x = startYPlusX1; x < endOffset; x++) {
	    index2 = x << 2;
	    for (int y = 0; y <yh; y ++) {
	        pixels[index2 + w * 4 * y] ^= 255;
	        pixels[index2 + 1 + w * 4 * y] ^= 255;
	        pixels[index2 + 2 + w * 4 * y] ^= 255;
	        pixels[index2 + 3 + w * 4 * y] = 255;
			index1++;
	    }
	}
}

void WriteableBitmapEx::DrawPolyline(short* points, int count, byte color)
{
	int x1 = points[0];
	int y1 = points[1];
	for (int i = 2; i < count; i += 2) {
		int x2 = points[i];
		int y2 = points[i + 1];

		DrawLineBresenham(x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}
}

void WriteableBitmapEx::FillPolygon(short* points, int count, byte color)
{
	FillPolygonBrush(points, count,  color, NULL);
}

void WriteableBitmapEx::FillPolygonBrush(short* points, int count, byte color, int* brush)
{
		// Use refs for faster access (really important!) speeds up a lot!
		int w = _width;
		int h = _height;
		int pn = count;
		int pnh = count >> 1;
		int* intersectionsX = new int[count];

				// Find y min and max (slightly faster than scanning from 0 to height)
		int yMin = h;
		int yMax = 0;
		for (int i = 1; i < pn; i += 2)
		{
		   int py = points[i];
		   if (py < yMin) yMin = py;
		   if (py > yMax) yMax = py;
		}
		if (yMin < 0) yMin = 0;
		if (yMax >= h) yMax = h - 1;

		// Scan line from min to max
		for (int y = yMin; y <= yMax; y++)
		{
		   // Initial point x, y
		   int vxi = points[0];
		   int vyi = points[1];

		   // Find all intersections
		   // Based on http://alienryderflex.com/polygon_fill/
		   int intersectionCount = 0;
		   for (int i = 2; i < pn; i += 2)
		   {
			  // Next point x, y
			  int vxj = points[i];
			  int vyj = points[i + 1];

			  // Is the scanline between the two points
			  if (vyi < y && vyj >= y
			   || vyj < y && vyi >= y)
			  {
				 // Compute the intersection of the scanline with the edge (line between two points)
				 intersectionsX[intersectionCount++] = ~~((vxi + (y - vyi) * (vxj - vxi)/ (vyj - vyi)));
			  }
			  vxi = vxj;
			  vyi = vyj;
		   }

		   // Sort the intersections from left to right using Insertion sort 
		   // It's faster than Array.Sort for this small data set
		   int t, j;
		   for (int i = 1; i < intersectionCount; i++)
		   {
			  t = intersectionsX[i];
			  j = i;
			  while (j > 0 && intersectionsX[j - 1] > t)
			  {
				 intersectionsX[j] = intersectionsX[j - 1];
				 j = j - 1;
			  }
			  intersectionsX[j] = t;
		   }

		   
		   if (brush == NULL) {
		       // Fill the pixels between the intersections
		       for (int i = 0; i < intersectionCount - 1; i += 2) {
		           int x0 = intersectionsX[i];
		           int x1 = intersectionsX[i + 1];

		           // Check boundary
		           if (x1 > 0 && x0 < w) {
		               if (x0 < 0) x0 = 0;
		               if (x1 >= w) x1 = w - 1;

		               // Fill the pixels
		               int index2;
		               for (int x = x0; x <= x1; x++) {
		                   index2 = (y * w + x) << 2;
		                   pixels[index2] = color;
		                   pixels[index2 + 1] = color;
		                   pixels[index2 + 2] = color;
		                   pixels[index2 + 3] = 255;
		               }
		           }
		       }
		   } else {
		       byte c;

		       // Fill the pixels between the intersections
		       for (int i = 0; i < intersectionCount - 1; i += 2) {
		           int x0 = intersectionsX[i];
		           int x1 = intersectionsX[i + 1];

		           // Check boundary
		           if (x1 > 0 && x0 < w) {
		               if (x0 < 0) x0 = 0;
		               if (x1 >= w) x1 = w - 1;

		               // Fill the pixels
		               int index2;
		               for (int x = x0; x <= x1; x++) {
		                   int xx = 7 - (x % 8);
		                   int mask = brush[y % 8];
		                   c = 255 * (((mask & (1 << (xx))) >> (xx)) ^ 1);
		                   index2 = (y * w + x) << 2;
		                   pixels[index2] = color | c;
		                   pixels[index2 + 1] = color | c;
		                   pixels[index2 + 2] = color | c;
		                   pixels[index2 + 3] = 255;
		               }
		           }
		       }
           }
		}

}


void WriteableBitmapEx::DrawLineBrush(int x1, int y1, int x2, int y2, byte color, int brush)
{
	int pixelWidth = _width;
	int pixelHeight = _height;

	int PRECISION_SHIFT = 8;

	if (x1 == x2 && y1 == y2)
	{
		int x1s = x1 << PRECISION_SHIFT;
		//int index = x1s + ((y1 * pixelWidth) << PRECISION_SHIFT);
		int index = x1 + (y1 * pixelWidth);
		//pixels[index >> PRECISION_SHIFT] = color;
		int index2 = index << 2;
		pixels[index2] = color;
		pixels[index2 + 1] = color;
		pixels[index2 + 2] = color;
		pixels[index2 + 3] = 255;
		return;
	}


	// Distance start and end point
	int dx = x2 - x1;
	int dy = y2 - y1;

	// Determine slope (absoulte value)
	int lenX, lenY;
	if (dy >= 0)
	{
		lenY = dy;
	}
	else
	{
		lenY = -dy;
	}

	if (dx >= 0)
	{
		lenX = dx;
	}
	else
	{
		lenX = -dx;
	}

	if (lenX > lenY)
	{ // x increases by +/- 1
		if (dx < 0)
		{
			int t = x1;
			x1 = x2;
			x2 = t;
			t = y1;
			y1 = y2;
			y2 = t;
		}

		// Init steps and start
		int incy = (dy << PRECISION_SHIFT) / dx;

		int y1s = y1 << PRECISION_SHIFT;
		int y2s = y2 << PRECISION_SHIFT;
		int hs = pixelHeight << PRECISION_SHIFT;

		if (y1 < y2)
		{
			if (y1 >= pixelHeight || y2 < 0)
			{
				return;
			}
			if (y1s < 0)
			{
				if (incy == 0)
				{
					return;
				}
				int oldy1s = y1s;
				// Find lowest y1s that is greater or equal than 0.
				y1s = incy - 1 + ((y1s + 1) % incy);
				x1 += (y1s - oldy1s) / incy;
			}
			if (y2s >= hs)
			{
				if (incy != 0)
				{
					// Find highest y2s that is less or equal than ws - 1.
					// y2s = y1s + n * incy. Find n.
					y2s = hs - 1 - (hs - 1 - y1s) % incy;
					x2 = x1 + (y2s - y1s) / incy;
				}
			}
		}
		else
		{
			if (y2 >= pixelHeight || y1 < 0)
			{
				return;
			}
			if (y1s >= hs)
			{
				if (incy == 0)
				{
					return;
				}
				int oldy1s = y1s;
				// Find highest y1s that is less or equal than ws - 1.
				// y1s = oldy1s + n * incy. Find n.
				y1s = hs - 1 + (incy - (hs - 1 - oldy1s) % incy);
				x1 += (y1s - oldy1s) / incy;
			}
			if (y2s < 0)
			{
				if (incy != 0)
				{
					// Find lowest y2s that is greater or equal than 0.
					// y2s = y1s + n * incy. Find n.
					y2s = y1s % incy;
					x2 = x1 + (y2s - y1s) / incy;
				}
			}
		}

		if (x1 < 0)
		{
			y1s -= incy * x1;
			x1 = 0;
		}
		if (x2 >= pixelWidth)
		{
			x2 = pixelWidth - 1;
		}

		int ys = y1s;

		// Walk the line!
		int y = ys >> PRECISION_SHIFT;
		int previousY = y;
		int index = x1 + y * pixelWidth;
		int k = incy < 0 ? 1 - pixelWidth : 1 + pixelWidth;
		int index2;
		if (brush == -1) {
			for (int x = x1; x <= x2; ++x) {
				index2 = index << 2;
				pixels[index2] = color;
				pixels[index2 + 1] = color;
				pixels[index2 + 2] = color;
				pixels[index2 + 3] = 255;

				ys += incy;
				y = ys >> PRECISION_SHIFT;
				if (y != previousY) {
					previousY = y;
					index += k;
				}
				else {
					++index;
				}
			}
		}
		else {
			byte c;
			for (int x = x1; x <= x2; ++x) {
				index2 = index << 2;
				int xx = 15 - (x % 16);
				c = 255 * (((brush & (1 << (xx))) >> (xx)) ^ 1);
				pixels[index2] = color | c;
				pixels[index2 + 1] = color | c;
				pixels[index2 + 2] = color | c;
				pixels[index2 + 3] = 255;

				ys += incy;
				y = ys >> PRECISION_SHIFT;
				if (y != previousY) {
					previousY = y;
					index += k;
				}
				else {
					++index;
				}
			}
		}
	}
	else
	{
		// Prevent divison by zero
		if (lenY == 0)
		{
			return;
		}
		if (dy < 0)
		{
			int t = x1;
			x1 = x2;
			x2 = t;
			t = y1;
			y1 = y2;
			y2 = t;
		}

		// Init steps and start
		int x1s = x1 << PRECISION_SHIFT;
		int x2s = x2 << PRECISION_SHIFT;
		int ws = pixelWidth << PRECISION_SHIFT;

		int incx = (dx << PRECISION_SHIFT) / dy;

		if (x1 < x2)
		{
			if (x1 >= pixelWidth || x2 < 0)
			{
				return;
			}
			if (x1s < 0)
			{
				if (incx == 0)
				{
					return;
				}
				int oldx1s = x1s;
				// Find lowest x1s that is greater or equal than 0.
				x1s = incx - 1 + ((x1s + 1) % incx);
				y1 += (x1s - oldx1s) / incx;
			}
			if (x2s >= ws)
			{
				if (incx != 0)
				{
					// Find highest x2s that is less or equal than ws - 1.
					// x2s = x1s + n * incx. Find n.
					x2s = ws - 1 - (ws - 1 - x1s) % incx;
					y2 = y1 + (x2s - x1s) / incx;
				}
			}
		}
		else
		{
			if (x2 >= pixelWidth || x1 < 0)
			{
				return;
			}
			if (x1s >= ws)
			{
				if (incx == 0)
				{
					return;
				}
				int oldx1s = x1s;
				// Find highest x1s that is less or equal than ws - 1.
				// x1s = oldx1s + n * incx. Find n.
				x1s = ws - 1 + (incx - (ws - 1 - oldx1s) % incx);
				y1 += (x1s - oldx1s) / incx;
			}
			if (x2s < 0)
			{
				if (incx != 0)
				{
					// Find lowest x2s that is greater or equal than 0.
					// x2s = x1s + n * incx. Find n.
					x2s = x1s % incx;
					y2 = y1 + (x2s - x1s) / incx;
				}
			}
		}

		if (y1 < 0)
		{
			x1s -= incx * y1;
			y1 = 0;
		}
		if (y2 >= pixelHeight)
		{
			y2 = pixelHeight - 1;
		}

		int index = x1s + ((y1 * pixelWidth) << PRECISION_SHIFT);

		// Walk the line!
		int inc = (pixelWidth << PRECISION_SHIFT) + incx;
		// 6010 1538560
		int index2;
		for (int y = y1; y <= y2; ++y)
		{
			index2 = (index >> PRECISION_SHIFT)<<2;
			//pixels[index >> PRECISION_SHIFT] = 255;//color;
			pixels[index2] = color;
			pixels[index2+1] = color;
			pixels[index2+2] = color;
			pixels[index2+3] = 255;
			index += inc;
		}
	}
}

void WriteableBitmapEx::DrawLineBresenham(int x1, int y1, int x2, int y2, byte color)
{
	int w = _width;
	int h = _height;

	// Distance start and end point
	int dx = x2 - x1;
	int dy = y2 - y1;

	// Determine sign for direction x
	int incx = 0;
	if (dx < 0)
	{
		dx = -dx;
		incx = -1;
	}
	else if (dx > 0)
	{
		incx = 1;
	}

	// Determine sign for direction y
	int incy = 0;
	if (dy < 0)
	{
		dy = -dy;
		incy = -1;
	}
	else if (dy > 0)
	{
		incy = 1;
	}

	// Which gradient is larger
	int pdx, pdy, odx, ody, es, el;
	if (dx > dy)
	{
		pdx = incx;
		pdy = 0;
		odx = incx;
		ody = incy;
		es = dy;
		el = dx;
	}
	else
	{
		pdx = 0;
		pdy = incy;
		odx = incx;
		ody = incy;
		es = dx;
		el = dy;
	}

	// Init start
	int x = x1;
	int y = y1;
	int error = el >> 1;
	int index;
	if(_vswr_mode == 3) {
		byte tmp1, tmp2;
		if (y < h && y >= 0 && x < w && x >= 0)
		{
			index = 4 * (y * w + x);
			tmp1 = 255 - color;
			tmp2 = 255 - pixels[index];
			pixels[index] = 255 - (tmp1 ^ tmp2);
			pixels[index+1] = pixels[index];
			pixels[index+2] = pixels[index];
			pixels[index+3] = 255;
		}

		// Walk the line!
		for (int i = 0; i < el; i++)
		{
			// Update error term
			error -= es;

			// Decide which coord to use
			if (error < 0)
			{
				error += el;
				x += odx;
				y += ody;
			}
			else
			{
				x += pdx;
				y += pdy;
			}

			// Set pixel
			if (y < h && y >= 0 && x < w && x >= 0)
			{
				index = 4 * (y * w + x);
				tmp1 = 255 - color;
				tmp2 = 255 - pixels[index];
				pixels[index] = 255 - (tmp1 ^ tmp2);;
				pixels[index+1] = pixels[index];
				pixels[index+2] = pixels[index];
				pixels[index+3] = 255;
			}
		}
	} else {
		if (y < h && y >= 0 && x < w && x >= 0)
		{
			index = 4 * (y * w + x);
			pixels[index] = color;
			pixels[index+1] = color;
			pixels[index+2] = color;
			pixels[index+3] = 255;
		}

		// Walk the line!
		for (int i = 0; i < el; i++)
		{
			// Update error term
			error -= es;

			// Decide which coord to use
			if (error < 0)
			{
				error += el;
				x += odx;
				y += ody;
			}
			else
			{
				x += pdx;
				y += pdy;
			}

			// Set pixel
			if (y < h && y >= 0 && x < w && x >= 0)
			{
				index = 4 * (y * w + x);
				pixels[index] = color;
				pixels[index+1] = color;
				pixels[index+2] = color;
				pixels[index+3] = 255;
			}
		}
	}
}

void WriteableBitmapEx::Render()
{
	Bitmap->Invalidate();
}