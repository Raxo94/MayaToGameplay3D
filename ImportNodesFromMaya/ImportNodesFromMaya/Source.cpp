#include <Windows.h>
#include "Linker.h"
#include "CircBuffer.h"

//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref__abc_export_2_maya_mesh_writer_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_mesh_op_cmd_2mesh_op_fty_action_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_obj_export_2obj_export_8cpp_example_html

MCallbackIdArray idList;
MObjectArray object[20];

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

void getNodesInTheScene(MFnMesh &mesh)
{
	MFloatPointArray pts;
	unsigned int i;

	MGlobal::displayInfo("current mesh: " + mesh.name());

	std::vector<vertices> points;

	MIntArray triangleCounts;
	MIntArray triangleVertexIDs;
	MVector vertexNormal;
	MIntArray normalList, normalCount;
	MFloatArray u, v;
	MFloatVectorArray normals;

	mesh.getPoints(pts, MSpace::kObject);
	mesh.getUVs(u, v, 0);
	mesh.getTriangles(triangleCounts, triangleVertexIDs);
	mesh.getNormals(normals, MSpace::kObject);

	points.resize(triangleVertexIDs.length());
	
	mesh.getNormalIds(normalCount, normalList);
	

	for (i = 0; i < triangleVertexIDs.length(); i++) {

		points.at(i).pos[0] = pts[triangleVertexIDs[i]].x;
		points.at(i).pos[1] = pts[triangleVertexIDs[i]].y;
		points.at(i).pos[2] = pts[triangleVertexIDs[i]].z;

		//mesh.getVertexNormal(normalList[i], vertexNormal, MSpace::kObject);;
		
		points.at(i).nor[0] = normals[triangleVertexIDs[i]].x;
		points.at(i).nor[1] = normals[triangleVertexIDs[i]].y;
		points.at(i).nor[2] = normals[triangleVertexIDs[i]].z;

		points.at(i).uv[0] = u[triangleVertexIDs[i]];
		points.at(i).uv[1] = v[triangleVertexIDs[i]];

	}

	char* message = new char[5000];
	HeaderType header{"mesh", points.size() * sizeof(vertices), points.size()};

	memcpy(message, &header, sizeof(HeaderType));
	memcpy((vertices*)message, points.data(), sizeof(vertices) * points.size());

	CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	circPtr->push(message, sizeof(HeaderType) + sizeof(vertices) * points.size());
	//CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	//circPtr->push(points.data(), sizeof(vertices) * points.size());

}

void MNodeFunction(MObject &node, void* clientData)
{
	if (node.hasFn(MFn::kMesh)) {
		MFnMesh mesh(node);
		if (mesh.canBeWritten()) {
			getNodesInTheScene(mesh);
			}
		}
}

//void timeElapsedFunction(float elapsedTime, float lastTime, void *clientData)
//{
//	MStatus res = MS::kSuccess;
//
//	
//}

EXPORT MStatus initializePlugin(MObject obj)
{
	//this indicates errors
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya Plugin", "1.0", "Any", &res);
	if (MFAIL(res)) {
		CHECK_MSTATUS(res);
	}

	MItDag it(MItDag::kBreadthFirst, MFn::kMesh);
	while (it.isDone() == false)
	{
		MFnMesh mesh(it.currentItem());
		
		getNodesInTheScene(mesh);

		it.next();
	}
	//MCallbackId timeId = MTimerMessage::addTimerCallback(5, timeElapsedFunction, NULL, &res);
	//
	//if (res == MS::kSuccess) {
	//	idList.append(timeId);
	//	MGlobal::displayInfo("time callback Succeeded");
	//}
	//else {
	//	MGlobal::displayInfo("time callback Failed");
	//}

	MCallbackId nodeId = MDGMessage::addNodeAddedCallback(MNodeFunction, kDefaultNodeType, NULL, &res);

	if (res == MS::kSuccess) {
		idList.append(nodeId);
		MGlobal::displayInfo("node callback Succeeded");
	}
	else {
		MGlobal::displayInfo("node callback Failed");
	}

	MGlobal::displayInfo("Maya plugin loaded!");

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MGlobal::displayInfo("Maya plugin unloaded!");

	MMessage::removeCallbacks(idList);

	return MS::kSuccess;
}
