#pragma once
#include "Linker.h"

enum MessageType { MayaMesh, MayaCamera, MayaMaterial, MayaNodeDelete };

struct vertices
{
	float pos[3];
	float nor[3];
	float uv[2];
};

struct Transform
{
	float translation[3];
	double scale[3];
	double rotation[4];
};

struct HeaderTypeMesh
{
	char messageType[256];
	size_t vertexArray;
	size_t vertexCount;
	Transform transform;
};

struct HeaderTypeCamera
{
	char messageType[256];
	bool isPerspective;
	float projectionMatrix[16];
	Transform transform;
	float nearPlane;
	float farPlane;
	float fieldOfView;

};

char* message;
CircBufferFixed *circPtr;
size_t offset = 0;
MCallbackIdArray idList;
MStatus res = MS::kSuccess;

HeaderTypeMesh meshHeader;
HeaderTypeCamera camHeader;
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
double rotCoordCamera[4];
double rotCoordMesh[4];
double scaleMesh[3];
