#pragma once
#include "CircBuffer.h"
#include <iostream>
#include <Vector>
using namespace std;

enum MessageType { MayaMesh, MayaCamera, MayaMaterial, MayaNodeDelete };

struct HeaderType
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

struct MayaCamera
{
	//transform
	bool isPerspective;
	float projectionMatrix[16]; //matrix
	float nearPlane;
	float farPlane;
	float fieldOfView;

};

struct HeaderTypeCam
{
	char messageType[256];
	bool orthographic;
};

class MayaData
{
private:
	char* NodeName;
	
	//Mesh
	Vertex* vertexArray;
	unsigned int vertexCount;

	//Camera
	bool isOrthographic;

	char* message;
	CircBufferFixed* circBuffer;


public:
	bool read();
	int messageType;
	Vertex* GetVertexArray();
	unsigned int GetVertexCount();
	char* GetNodeName();


	MayaData();
	~MayaData();
};