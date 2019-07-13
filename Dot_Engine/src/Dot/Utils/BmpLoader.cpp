#include "stdafx.h"
#include "BmpLoader.h"


namespace Dot {
	std::shared_ptr<BMPIMAGE> BMPLoader::LoadBMP(const std::string & filename)
	{
		std::shared_ptr<BMPIMAGE> image;
		image.reset(new BMPIMAGE());

		uint8_t* datBuff[2] = { nullptr, nullptr }; // Header buffers

		uint8_t* pixels = nullptr; // Pixels

		BITMAPFILEHEADER* bmpHeader = nullptr; // Header
		BITMAPINFOHEADER* bmpInfo = nullptr; // Info 

		std::ifstream file(filename, std::ios::binary);
		if (!file)
		{
			std::cout << "Failed to open bitmap file.\n";
		}
		datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
		datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];
		
		
		file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
		file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

		bmpHeader = (BITMAPFILEHEADER*)datBuff[0];
		bmpInfo = (BITMAPINFOHEADER*)datBuff[1];


		pixels = new uint8_t[bmpInfo->biSizeImage];
		image->width = bmpInfo->biWidth;
		image->height = bmpInfo->biHeight;
		// Go to where image data starts, then read in image data
		file.seekg(bmpHeader->bfOffBits);
		file.read((char*)pixels, bmpInfo->biSizeImage);

		RGB tmp(0,0,0);
		for (unsigned long i = 0; i < bmpInfo->biSizeImage; i+=3)
		{
			tmp.B = float(pixels[i]);
			tmp.G = float(pixels[i + 1]);
			tmp.R = float(pixels[i + 2]);

			image->rgb.push_back(tmp);
		}
	

		delete[] datBuff[0];
		delete[] datBuff[1];
		delete[] pixels;

		return image;
	
	}
}