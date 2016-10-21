#pragma once
#include "Linker.h"

enum MessageType { MayaMesh, MayaCamera, MayaMaterial, MayaNodeDelete };

struct vertices
{
	float pos[3];
	float nor[3];
	float uv[2];
};

struct HeaderType
{
	char messageType[256];
	size_t vertexArray;
	size_t vertexCount;
};

char* message;
CircBufferFixed *circPtr;
size_t offset = 0;
MCallbackIdArray idList;
MStatus res = MS::kSuccess;

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

