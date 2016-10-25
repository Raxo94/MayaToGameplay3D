#pragma once
#include "CircBuffer.h"
#include <iostream>
using namespace std;

enum MessageType { MayaMesh, MayaCamera, MayaMaterial, MayaNodeDelete };

struct HeaderTypeMesh
{
	char   Name[256];
	size_t vertexArray;
	size_t vertexCount;
};

struct Vertex
{
	float pos[3];
	float norm[3];
	float UV[2];
};



struct HeaderTypeCamera
{
	char messageType[256];
	bool isPerspective;
	float projectionMatrix[16];
	float translation[3];
	double rotation[4];
	float nearPlane;
	float farPlane;
	float fieldOfView;

};



class MayaData
{
private:
	char* NodeName;
	
	//Mesh
	Vertex* vertexArray;
	unsigned int vertexCount;

	//Camera
	bool isPerspective;
	float projectionMatrix[16];

	char* message;
	CircBufferFixed* circBuffer;


public:
	HeaderTypeCamera* cam;
	HeaderTypeMesh mesh;


	bool read();
	int messageType;
	Vertex* GetVertexArray();
	unsigned int GetVertexCount();
	char* GetNodeName();


	MayaData();
	~MayaData();
};