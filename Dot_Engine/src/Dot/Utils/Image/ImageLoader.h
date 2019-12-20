#pragma once

#define _BYTE   unsigned char
#define _DWORD  unsigned int
#define _LONG   int
#define _UINT   unsigned int
#define _WORD   unsigned short int



#pragma pack(1) // Prevents padding
	typedef struct _tagBITMAPFILEHEADER {
		_WORD    bfType;
		_DWORD   bfSize;
		_WORD    bfReserved1;
		_WORD    bfReserved2;
		_DWORD   bfOffBits;
	} _BITMAPFILEHEADER, *_PBITMAPFILEHEADER;

	typedef struct _tagBITMAPINFOHEADER {
		_DWORD      biSize;
		_LONG       biWidth;
		_LONG       biHeight;
		_WORD       biPlanes;
		_WORD       biBitCount;
		_DWORD      biCompression;
		_DWORD      biSizeImage;
		_LONG       biXPelsPerMeter;
		_LONG       biYPelsPerMeter;
		_DWORD      biClrUsed;
		_DWORD      biClrImportant;
	
	} _BITMAPINFOHEADER, *_PBITMAPINFOHEADER;

	typedef struct tagRGBTRIPLE {
		_BYTE    rgbtBlue;
		_BYTE    rgbtGreen;
		_BYTE    rgbtRed;
		_BYTE    rgbReserved;
	} _RGBTRIPLE,  _PRGBTRIPLE;

#pragma pack()

namespace Dot {
	
	struct _PIXEL
	{
		unsigned char r,g,b;
	};

	struct _BMPIMAGE
	{
		unsigned int width;
		unsigned int height;
		std::vector<std::vector<_PIXEL>> pixels;
	};

	class ImageLoader
	{
	public:
		static _BMPIMAGE LoadBMP(const std::string& filename);
	};

}