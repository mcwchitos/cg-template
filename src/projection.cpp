#include "projection.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


Projection::Projection(unsigned short width, unsigned short height, std::string obj_file) : ReadObj(width, height, obj_file)
{
}

Projection::~Projection()
{
}

void Projection::DrawScene()
{
	parser->Parse();

	float w_center = width / 2.0f;
	float h_center = height / 2.0f;

	float rotationAngleY = 0.0f * M_PI / 180.0f;
	/*float rotationAngleZ = 15.0f * M_PI / 180.0f;*/

	float3 eye	 { 0, 0, 0};
	float3 lookAt{ 0, 0, 2};
	float3 up	 { 0, 1, 0};

	float fov_y_angle = 60.0 * M_PI / 180.0;
	float zFar = 10;
	float zNear = 1;

	/*float4x4 rotationtry
	{
		{cos(rotationanglez), -sin(rotationanglez), 0, 0},
		{sin(rotationanglez), cos(rotationangley) * cos(rotationanglez), sin(rotationangley), 0},
		{0, -sin(rotationangley), cos(rotationangley), 0},
		{0, 0, 0, 1}
	};*/
	float4x4 rotation{
		{1, 0, 0, 0},
		{0, cos(rotationAngleY), sin(rotationAngleY), 0},
		{0, -sin(rotationAngleY), cos(rotationAngleY), 0},
		{0, 0, 0, 1}
	};

	float4x4 translation{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 2, 1}
	};

	float4x4 world = mul(translation, rotation);

	float3 zaxis = normalize(eye - lookAt);
	float3 xaxis = normalize(cross(up, zaxis));
	float3 yaxis = cross(zaxis, xaxis);

	float4x4 view{
		{xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye)},
		{yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye)},
		{zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye)},
		{0, 0, 0, 1}
	};

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	float yScale = 1.0f / tan(fov_y_angle / 2.0f);
	float xScale = yScale / aspect;

	float4x4 projection{
		{xScale, 0, 0, 0},
		{0, yScale, 0, 0},
		{0, 0, zFar / (zFar - zNear), -zNear * zFar / (zFar - zNear)},
		{0, 0, 1, 0}
	};


	float4x4 translateMatrix = mul(projection, view, world);

	for each (face face in parser->GetFaces()){
		float4 translated[3];
		for (int i = 0; i < 3; i++){
			translated[i] = mul(translateMatrix, face.vertexes[i]);
			translated[i] /= translated[i].w;
		}
	
		DrawLine(static_cast<unsigned short>(w_center + w_center * translated[0].x),
			static_cast<unsigned short>(h_center - h_center * translated[0].y),
			static_cast<unsigned short>(w_center + w_center * translated[1].x),
			static_cast<unsigned short>(h_center - h_center * translated[1].y),
			color(255, 0, 0));

		DrawLine(static_cast<unsigned short>(w_center + w_center * translated[1].x),
			static_cast<unsigned short>(h_center - h_center * translated[1].y),
			static_cast<unsigned short>(w_center + w_center * translated[2].x),
			static_cast<unsigned short>(h_center - h_center * translated[2].y),
			color(0, 255, 0));

		DrawLine(static_cast<unsigned short>(w_center + w_center * translated[2].x),
			static_cast<unsigned short>(h_center - h_center * translated[2].y),
			static_cast<unsigned short>(w_center + w_center * translated[0].x),
			static_cast<unsigned short>(h_center - h_center * translated[0].y),
			color(0, 0, 255));
	}
}


