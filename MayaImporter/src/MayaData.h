#pragma once
#include "CircBuffer.h"
#include <iostream>
#include <vector>
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
	Vertex* vertexArray; //pointer to allocated vertexArray Memory
	size_t vertexCount;
};


struct HeaderTypeCamera
{
	char CameraName[256];
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
	char meshName[256]; //what mesh will suffer the transformation
	float translation[3];
	double scale[3];
	double rotation[4];
};

struct HeaderTypeMaterial
{
	int amountOfMeshes;
	bool hasTexture;
	char materialName[256];
	char textureFilepath[256];
	float diffuse;
	float color[3];
};

struct MeshName
{
	char meshName[256];
};

class MayaData
{
private:
	Vertex* vertexArray; //used to store vertices of current mesh. pointer since vertexcount can vary
	char* message; //allocated space for saving pop messages
	CircBufferFixed* circBuffer;


public:
	
	HeaderTypeCamera* cam; //pointer to buffer
	HeaderTypeMesh* mesh; //pointer to buffer
	HeaderTypeTransform* transform; //pointer to buffer
	HeaderTypeMaterial* material; //pointer to buffer

	MeshName* meshName; //Array For storing the names of meshes bound to material

	bool read(); //getMessageData
	int messageType; //used in determening what kind of message MayaData has gotten/ enum MessageType


	MayaData();
	~MayaData();
};