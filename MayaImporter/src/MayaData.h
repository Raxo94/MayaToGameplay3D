#pragma once

struct Vertex
{
	float pos[3];
	float norm[3];
	float UV[2];
};

class mayaData
{
public:
	char Name[256];
	Vertex* vertexArray;
	
};