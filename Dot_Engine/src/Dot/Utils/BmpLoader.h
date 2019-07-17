#pragma once
#define BYTE   unsigned char
#define DWORD  unsigned int
#define LONG   int
#define UINT   unsigned int
#define WORD   unsigned short int

namespace Dot {

#pragma pack(1) // Prevents padding
	typedef struct tagBITMAPFILEHEADER {
		WORD    bfType;
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;
	} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

	typedef struct tagBITMAPINFOHEADER {
		DWORD      biSize;
		LONG       biWidth;
		LONG       biHeight;
		WORD       biPlanes;
		WORD       biBitCount;
		DWORD      biCompression;
		DWORD      biSizeImage;
		LONG       biXPelsPerMeter;
		LONG       biYPelsPerMeter;
		DWORD      biClrUsed;
		DWORD      biClrImportant;
	
	} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

	typedef struct tagRGBTRIPLE {
		BYTE    rgbtBlue;
		BYTE    rgbtGreen;
		BYTE    rgbtRed;
	} RGBTRIPLE, * PRGBTRIPLE;

#pragma pack()

	struct BMPIMAGE {
		BITMAPINFOHEADER infoHeader;
		BITMAPFILEHEADER fileHeader;
		RGBTRIPLE rgb;
	};


	class BMPLoader
	{
	public:
		static std::shared_ptr<BMPIMAGE>LoadBMP(const std::string& filename);
	};
}