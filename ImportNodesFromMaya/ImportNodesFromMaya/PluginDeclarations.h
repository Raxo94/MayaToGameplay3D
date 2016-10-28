#pragma once
#include "Linker.h"

enum MessageType { MayaMesh, MayaCamera, MayaTransform, MayaMaterial, MayaNodeDelete};

struct vertices
{
	float pos[3];
	float nor[3];
	float uv[2];
};

struct HeaderTypeTransform
{
	char meshName[256];
	float translation[3];
	double scale[3];
	double rotation[4];
};

struct HeaderTypeMesh
{
	char messageType[256];
	vertices* vertexArray;
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

struct HeaderTypeMaterial
{
	int amountOfMeshes;
	char materialName[256];
	char textureFilepath[256];
	float diffuse;
	float color[3];
};

struct Meshes
{
	char meshName[256];
};

char* message;
CircBufferFixed *circPtr;
size_t offset = 0;
MCallbackIdArray idList;
MStatus res = MS::kSuccess;

HeaderTypeMesh meshHeader;
HeaderTypeCamera camHeader;
HeaderTypeTransform transformHeader;
HeaderTypeMaterial matHeader;

MFloatPointArray pts;
std::vector<vertices> points;
MIntArray triangleCounts;
MIntArray triangleVertexIDs;
MVector vertexNormal;
MIntArray normalList, normalCount;
MFloatArray u, v;
MFloatVectorArray normals;

MIntArray triangleCountsOffsets;
MIntArray triangleIndices;

unsigned int i;

int currentView;
M3dView camView;
MDagPath camera;

MFloatMatrix projMatrix;
MObject parentCamera;
MObject parentMesh;

vector<Meshes> meshVector;
Meshes meshStruct;
