#include "stdafx.h"
#include "ImageLoader.h"

#define BITMAP_ID 0x4D42	
#define UNCOMPRESSED_BMP 0L
namespace Dot {
	_BMPIMAGE ImageLoader::LoadBMP(const std::string& filename)
	{
		_BMPIMAGE img;
		
		std::ifstream file(filename, std::ios::binary);
		
		_BITMAPFILEHEADER bmFileHeader;
		file.read(reinterpret_cast<char*>(&bmFileHeader),sizeof(_BITMAPFILEHEADER));
		if (bmFileHeader.bfType != BITMAP_ID)
		{
			LOG_ERR("Not supported file format %s", filename);
			return img;
		}

		_BITMAPINFOHEADER bmInfoHeader;
		file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(_BITMAPINFOHEADER));

		if (bmInfoHeader.biBitCount != 24)
		{
			LOG_ERR("Only 24 bit file is supported!");
			return img;
		}
		if (bmInfoHeader.biCompression != UNCOMPRESSED_BMP)
		{
			LOG_ERR("Only uncompressed files supported!");
			return img;
		}

		img.width = bmInfoHeader.biWidth;
		img.height = bmInfoHeader.biHeight;

		file.seekg(bmFileHeader.bfOffBits);
		const int padding = (4 - (img.width * 3) % 4) % 4;

		img.pixels.resize(img.height);
		for (int y = img.height-1; y >= 0; y--)
		{
			img.pixels[y].resize(img.width);
			for (int x = img.width-1; x >= 0; x--)
			{
				img.pixels[y][x].r = file.get();
				img.pixels[y][x].g = file.get();
				img.pixels[y][x].b = file.get();
			}
			file.seekg(padding, std::ios::cur);
		}
		return img;
	}
}