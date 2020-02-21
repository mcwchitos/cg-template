#include "read_obj.h"


#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>

#include <algorithm>

ObjParser::ObjParser(std::string filename):
	filename(filename)
{
}


ObjParser::~ObjParser()
{

}

int ObjParser::Parse()
{
	std::ifstream obj_file(filename, std::ifstream::in);
	if (obj_file.fail())
		return 1;
	std::string line;
	while (std::getline(obj_file, line)){
		if (line.rfind("v ", 0) == 0){
			std::vector<std::string> vertexData = Split(line, ' ');
			float4 vertex{0, 0, 0, 1};
			for (int i = 1; i < vertexData.size(); ++i)
				vertex[i - 1] = std::stof(vertexData.at(i));
			vertexes.push_back(vertex);
		}
		else if (line.rfind("f ", 0) == 0){
			std::vector<std::string> facesData = Split(line, ' ');
			std::vector<int> indexes;
			for (int i = 1; i < facesData.size(); ++i)
			{
				std::vector<std::string> indexData = Split(facesData.at(i), '/');
				int index = std::stoi(indexData.at(0));
				if (index > 0)
					--index;
				else
					index += vertexes.size();
				indexes.push_back(index);
			}
			for (int i = 0; i < indexes.size() - 2; ++i)
			{
				face face;
				face.vertexes[0] = vertexes[indexes.at(0)];
				face.vertexes[1] = vertexes[indexes.at(i + 1)];
				face.vertexes[2] = vertexes[indexes.at(i + 2)];
				faces.push_back(face);
			}
		}
	}
	return 0;
}

const std::vector<face>& ObjParser::GetFaces()
{
	return faces;
}

std::vector<std::string> ObjParser::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}


ReadObj::ReadObj(unsigned short width, unsigned short height, std::string obj_file): LineDrawing(width, height)
{
	parser = new ObjParser(obj_file);
}

ReadObj::~ReadObj()
{
	delete parser;
}


void ReadObj::DrawScene()
{
	parser->Parse();

	float w_center = width / 2.0f;
	float h_center = height / 2.0f;
	float radius = std::min(w_center, h_center) - 40.0f;

	for each (face face in parser->GetFaces())
	{
		DrawLine(static_cast<unsigned short>(w_center + radius * face.vertexes[0].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[0].y),
			static_cast<unsigned short>(w_center + radius * face.vertexes[1].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[1].y),
			color(255, 0, 0));

		DrawLine(static_cast<unsigned short>(w_center + radius * face.vertexes[1].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[1].y),
			static_cast<unsigned short>(w_center + radius * face.vertexes[2].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[2].y),
			color(0, 255, 0));

		DrawLine(static_cast<unsigned short>(w_center + radius * face.vertexes[2].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[2].y),
			static_cast<unsigned short>(w_center + radius * face.vertexes[0].x),
			static_cast<unsigned short>(h_center + radius * face.vertexes[0].y),
			color(0, 0, 255));
	}
}

