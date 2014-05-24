// RGBpixmap.h,  F.S. Hill,Jr., 10/14/99, for ECE660
// class to support working with RGB pixmaps.
// fixed 11/7/98 to prevent OpenGL from trying to align pixels to four byte boundaries in memory
// which it does by default - see p.299.
#ifndef _RGBPIXMAP
#define _RGBPIXMAP
#include <windows.h>
#include <assert.h>
#ifdef _MSC_VER
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;
#else
#include <iostream.h>
#include <stdlib.h>
#include <fstream.h>
#include <string>
#include <strstream.h>
#include <stdio.h>
#include <algorithm>
using namespace std;
#endif

#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;
//$$$$$$$$$$$$$$$$$$$ class IntPoint $$$$$$$$$$$$$$
class IntPoint{
public:
	int x, y;
	IntPoint(){ x = y = 0;}
	IntPoint(int xx, int yy){ x = xx; y = yy;} // constructor
	void set(int xx, int yy){x = xx; y = yy;}
	void set(IntPoint p) {x = p.x; y = p.y;}
};
//$$$$$$$$$$$$ class IntRect $$$$$$$$$$$
class IntRect{
public:
	int left, top, right, bott;
	void set(int ll, int tt, int rr, int bb){left = ll; top = tt; right = rr; bott = bb;}
	void set(IntRect r){left = r.left;top = r.top; right = r.right; bott = r.bott;}
	void print()
	{ 
		cout << "IntRect: " << left << " " << top << " " << right << " " << bott << endl;
		int nCols = right - left;
		int nBytes = 3 * nCols;
		int nBytesInRow = ((nBytes + 3)/4)*4;
		int numPadBytes = nBytesInRow - nBytes;
		cout << " nCols, # pad bytes = " << nCols << "," <<numPadBytes << endl;
	}
	void fix()
	{ // reestablish order of left, right, etc.
		if(left > right){ int tmp = left; left = right; right = tmp;} //swap
		if(bott > top){ int temp = top; top = bott; bott = temp;}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<< drawRubber >>>>>>>>>>>
	void draw() // draw rectangle
	{
		glBegin(GL_LINE_LOOP);
		glVertex2i(left, top);  glVertex2i(right,top);
		glVertex2i(right,bott);	glVertex2i(left, bott);
		glEnd();
		glFlush();
	}
	void drawDiag() // draw diagonal of rect (for rubber line)
	{
		glBegin(GL_LINES);
		glVertex2i(left,top);
		glVertex2i(right, bott);
		glEnd();
	}
}; // end of IntRect

//$$$$$$$$$$$$$$$$$$ class mRGB $$$$$$$$$$$$$$$$$$
class mRGB{ // the name RGB is reserved in windows
public: uchar r,g,b;
		  mRGB(){r = g = b = 0;}
		  mRGB(mRGB& p){r = p.r; g = p.g; b = p.b;}
		  mRGB(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
		  void set(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
};
//$$$$$$$$$$$$$$$$$ RGBPixmap $$$$$$$$$$$$$$$
class RGBpixmap{
private: 
	mRGB* pixel; // array of pixels
	
public:
	int nRows, nCols; // dimensions of the pixmap
	RGBpixmap() {nRows = nCols = 0; pixel = 0;}

	void setTexture(unsigned int textureName);
	void makeCheckerboard();

	RGBpixmap(int rows, int cols) //constructor
	{
		nRows = rows;
		nCols = cols;
		pixel = new mRGB[rows*cols]; 
	}
	int readBMPFile(string fname); // read BMP file into this pixmap
	int writeBMPFile(string fname); // write this pixmap to a BMP file
	void freeIt() // give back memory for this pixmap
	{
		delete []pixel;
		nRows = nCols = 0;
	}
	//<<<<<<<<<<<<<<<<<< copy >>>>>>>>>>>>>>>>>>>
	void copy(IntPoint from, IntPoint to, int x, int y, int width, int height)
	{ // copy a region of the display back onto the display
		if(nRows == 0 || nCols == 0) return;
		glCopyPixels(x, y, width, height,GL_COLOR);
	}
	//<<<<<<<<<<<<<<<<<<< draw >>>>>>>>>>>>>>>>>
	void draw()
	{ // draw this pixmap at current raster position
		if(nRows == 0 || nCols == 0) return;
		//tell OpenGL NOT to try to align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glDrawPixels(nCols, nRows,GL_RGB, GL_UNSIGNED_BYTE,pixel);
	}
	//<<<<<<<<<<<<<<<<< read >>>>>>>>>>>>>>>>
	int read(int x, int y, int wid, int ht)
	{ // read a rectangle of pixels into this pixmap
		nRows = ht;
		nCols = wid;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL NOT to try to align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(x, y, nCols, nRows, GL_RGB,GL_UNSIGNED_BYTE,pixel);
		return 0;
	}
	//<<<<<<<<<<<<<<<<< read from IntRect >>>>>>>>>>>>>>>>
	int read(IntRect r)
	{ // read a rectangle of pixels into this pixmap
		nRows = r.top - r.bott;
		nCols = r.right - r.left;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL NOT to try to align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(r.left,r.bott, nCols, nRows, GL_RGB,GL_UNSIGNED_BYTE,pixel);
		return 0;
	}
	//<<<<<<<<<<<<<< setPixel >>>>>>>>>>>>>
	void setPixel(int x, int y, mRGB color)
	{
		if(x>=0 && x <nCols && y >=0 && y < nRows)
			pixel[nCols * y + x] = color;
	}
	//<<<<<<<<<<<<<<<< getPixel >>>>>>>>>>>
	mRGB getPixel(int x, int y)
	{
		mRGB bad(255,255,255);
		if(x < 0 || x >= nCols || y < 0 || y >= nRows)
		{
			cout << "\nx,y = " << x << "," << y << " bad in getPixel()";
			return bad;
		}
		return pixel[nCols * y + x];
	}
}; //end of class RGBpixmap
#endif