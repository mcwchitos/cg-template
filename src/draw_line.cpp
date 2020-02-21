#include "draw_line.h"


#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>


LineDrawing::LineDrawing(unsigned short width, unsigned short height): ColorSpace(width, height)
{

}

LineDrawing::~LineDrawing()
{
}

void LineDrawing::DrawLine(unsigned short x_begin, unsigned short y_begin, unsigned short x_end, unsigned short y_end, color color)
{
	bool steep = abs(x_end - x_begin) < abs(y_end - y_begin);
	if (steep)
	{
		std::swap(y_begin, x_begin);
		std::swap(y_end, x_end);
	}

	if (x_begin > x_end)
	{
		std::swap(x_begin, x_end);
		std::swap(y_begin, y_end);
	}

	float y = static_cast<float>(y_begin);
	float deltaX = static_cast<float>(x_end - x_begin);
	float deltaY = fabs(static_cast<float>(y_end - y_begin));

	float err = deltaX / 2.0f;
	short value = y_begin > y_end ? -1 : 1;
	for (unsigned short x = x_begin; x <= x_end; ++x)
	{
		if (steep)
			SetPixel(y, x, color);
		else
			SetPixel(x, y, color);

		err -= deltaY;

		if (err < 0)
		{
			err += deltaX;
			y += value;
		}
	}
}

void LineDrawing::DrawScene()
{
	float w_center = width / 2.0f;
	float h_center = height / 2.0f;
	float radius = std::min(w_center, h_center) - 40.0f;
	for (float angle = 0.0f; angle < 360.0f; angle += 5.0f) {
		DrawLine(w_center, h_center, w_center + radius * cos(angle * M_PI / 180.0f), h_center + radius * sin(angle * M_PI / 180.0f), 
			color(255 * sin(angle * M_PI / 180.0f), 255 * cos(angle * M_PI / 180.0f), 255));
	}
}