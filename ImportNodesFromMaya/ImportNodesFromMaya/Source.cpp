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

void getNodesInTheScene(MFnMesh &mesh)
{
	MFloatPointArray pts;
	unsigned int i;

	MGlobal::displayInfo("current mesh: " + mesh.name());

	mesh.getPoints(pts, MSpace::kObject);
	std::vector<vertices> points;

	/*points.resize(pts.length());*/

	////////////////////////////
	MIntArray triangleCounts;
	MIntArray triangleVertexIDs;
	mesh.getTriangles(triangleCounts, triangleVertexIDs);

	points.resize(triangleVertexIDs.length());
	////////////////////////////

	MVector vertexNormal;
	MIntArray normalList, normalCount;
	MFloatArray u, v;
		
	mesh.getNormalIds(normalCount, normalList);
	mesh.getUVs(u, v, 0);

	for (i = 0; i < triangleVertexIDs.length(); i++) {

		points.at(i).pos[0] = pts[triangleVertexIDs[i]].x;
		points.at(i).pos[1] = pts[triangleVertexIDs[i]].y;
		points.at(i).pos[2] = pts[triangleVertexIDs[i]].z;

		mesh.getVertexNormal(normalList[i], vertexNormal, MSpace::kObject);;
		
		points.at(i).nor[0] = vertexNormal[points.at(i).pos[0]];
		points.at(i).nor[1] = vertexNormal[points.at(i).pos[1]];
		points.at(i).nor[2] = vertexNormal[points.at(i).pos[2]];

		points.at(i).uv[0] = u[i];
		points.at(i).uv[1] = v[i];

	}

	CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	circPtr->push(points.data(), sizeof(float) * 8 * points.size());

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
		MGlobal::displayInfo("time callback Succeeded");
	}
	else {
		MGlobal::displayInfo("time callback Failed");
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
