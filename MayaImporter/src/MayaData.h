#pragma once
#include "CircBuffer.h"
#include <iostream>
using namespace std;

enum MessageType { MayaMesh, MayaCamera, MayaTransform, MayaMaterial, MayaNodeDelete };



struct Vertex
{
	float pos[3];
	float norm[3];
	float UV[2];
};


struct HeaderTypeMesh
{
	char   Name[256];
	Vertex* vertexArray;
	size_t vertexCount;
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

struct HeaderTypeTransform
{
	char meshName[256];
	float translation[3];
	double scale[3];
	double rotation[4];
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
	HeaderTypeMesh* mesh;
	HeaderTypeTransform* transform;


	bool read();
	int messageType;
	Vertex* GetVertexArray();
	unsigned int GetVertexCount();
	char* GetNodeName();


	MayaData();
	~MayaData();
};