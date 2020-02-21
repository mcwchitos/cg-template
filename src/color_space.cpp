#include "color_space.h"


ColorSpace::ColorSpace(unsigned short width, unsigned short height): BlackImage(width, height)
{

}

ColorSpace::~ColorSpace()
{
}

void ColorSpace::DrawScene()
{
	for (short x = 0; x < width; ++x) {
		for (short y = 0; y < height; ++y) {
			float r;
			if (x / static_cast<float>(width) + y / static_cast<float>(height) <= 1)
				r = x / static_cast<float>(width) + y / static_cast<float>(height);
			else
				r = 2.0f - (x / static_cast<float>(width) + y / static_cast<float>(height));
			float g = x / static_cast<float>(width) * y / static_cast<float>(height);
			float b = (1.0f - x / static_cast<float>(width)) * (1.0f - y / static_cast<float>(height));


			SetPixel(x, y, color(static_cast<unsigned char>(255 * r),
				static_cast<unsigned char>(255 * g),
				static_cast<unsigned char>(255 * b)));
		}
	}

}

void ColorSpace::SetPixel(unsigned short x, unsigned short y, color color)
{
	frame_buffer[y * width + x] = color;
}

