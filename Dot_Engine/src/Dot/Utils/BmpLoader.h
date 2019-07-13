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

#pragma pack()

	struct RGB {
		RGB(float r,float g,float b)
			:R(r),G(g),B(b)
		{}

		float R, G, B;
	};
	struct BMPIMAGE {
		std::vector<RGB> rgb;
		int width;
		int height;
	};


	class BMPLoader
	{
	public:
		static std::shared_ptr<BMPIMAGE>LoadBMP(const std::string& filename);
	};
}