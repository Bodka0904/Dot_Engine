#include "stdafx.h"
#include "BmpLoader.h"


namespace Dot {
	std::shared_ptr<_BMPIMAGE> BMPLoader::LoadBMP(const std::string & filename)
	{
	
		std::shared_ptr<_BMPIMAGE> image;
		image.reset(new _BMPIMAGE());

		//uint8_t* datBuff[2] = { nullptr, nullptr }; // Header buffers

		//uint8_t* pixels = nullptr; // Pixels

		_BITMAPFILEHEADER* bmpHeader = nullptr; // Header
		_BITMAPINFOHEADER* bmpInfo = nullptr; // Info 
		FILE* l_file;

		l_file = fopen(filename.c_str(),"rb");
		fread(&image->fileHeader, sizeof(image->fileHeader), 1, l_file);
		fseek(l_file, sizeof(image->fileHeader), SEEK_SET);
		fread(&image->infoHeader, sizeof(image->infoHeader), 1, l_file);
	
		image->values.resize(image->infoHeader.biWidth * image->infoHeader.biHeight);
		_RGBTRIPLE rgb;
		for (int i = 0; i < image->infoHeader.biWidth * image->infoHeader.biHeight; i++)
		{
			fread(&rgb, sizeof(rgb), 1, l_file);
			image->values[i] = rgb;
			
		}
		fclose(l_file);
		
		
		//std::ifstream file(filename, std::ios::binary);
		//if (!file)
		//{
		//	std::cout << "Failed to open bitmap file.\n";
		//}
		//datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
		//datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];
		//
		//
		//file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
		//file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));
		//
		//bmpHeader = (BITMAPFILEHEADER*)datBuff[0];
		//bmpInfo = (BITMAPINFOHEADER*)datBuff[1];
		//
		//pixels = new uint8_t[bmpInfo->biSizeImage];
		//
		//// Go to where image data starts, then read in image data
		//file.seekg(bmpHeader->bfOffBits);
		//file.read((char*)pixels, bmpInfo->biSizeImage);
		//
		//
		//for (unsigned long i = 0; i < bmpInfo->biSizeImage; i+=3)
		//{
		//	
		//}
	

		//delete[] datBuff[0];
		//delete[] datBuff[1];
		//delete[] pixels;

		return image;
	
	}
}