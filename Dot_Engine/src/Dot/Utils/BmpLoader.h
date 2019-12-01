#pragma once
#define _BYTE   unsigned char
#define _DWORD  unsigned int
#define _LONG   int
#define _UINT   unsigned int
#define _WORD   unsigned short int

namespace Dot {

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
	} _RGBTRIPLE,  _PRGBTRIPLE;

#pragma pack()

	struct _BMPIMAGE {
		_BITMAPINFOHEADER infoHeader;
		_BITMAPFILEHEADER fileHeader;
		
		std::vector<_RGBTRIPLE> values;
	};


	class BMPLoader
	{
	public:
		static std::shared_ptr<_BMPIMAGE>LoadBMP(const std::string& filename);
	};
}