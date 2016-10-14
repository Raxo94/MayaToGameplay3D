#include <Windows.h>
#include "Linker.h"
#include "CircBuffer.h"

//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref__abc_export_2_maya_mesh_writer_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_mesh_op_cmd_2mesh_op_fty_action_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_obj_export_2obj_export_8cpp_example_html

MCallbackIdArray idList;

struct vertices
{
	float pos[3];
};

void getNodesInTheScene(MFnMesh &mesh)
{
	MFloatPointArray pts;
	MIntArray adjacentVertexList;
	unsigned int i;

	MGlobal::displayInfo("current mesh: " + mesh.name());

	mesh.getPoints(pts, MSpace::kObject);
	//unsigned int numPolys = mesh.numPolygons();
	std::vector<vertices> points;

	//multiply by 3 to make sure x,y and z is equal to one point
	points.resize(pts.length());

	//store points in float vector
	for (i = 0; i < pts.length(); i++) {

		//size_t local = i * 3;
		points.at(i).pos[0] = pts[i].x;
		points.at(i).pos[1] = pts[i].y;
		points.at(i).pos[2] = pts[i].z;

		//adjacentVertexList.append(points[i]);*/
		//MIntArray polyVerts = mesh.getPolygonVertices(i, adjacentVertexList);

		//MString n = "";
		//n += polyVerts[i];
		//MGlobal::displayInfo("normals: " + n);

	}

	CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);

	circPtr->push(points.data(), sizeof(float) * 3 * points.size());
	
	//MFloatVectorArray normal;
	//MFloatVector translation;
	//MVector vertexNormal;

	//MVector avarageNormal(0, 0, 0);
	///*mesh.getVertexNormal(adjacentVertexList.length(), vertexNormal, MSpace::kWorld);*/

	////norPoints.resize(adjacentVertexList.length() * 3);
	//for (i = 0; i < adjacentVertexList.length(); i++) {

	//	mesh.getVertexNormal(adjacentVertexList[i], vertexNormal, MSpace::kWorld);
	//	avarageNormal += vertexNormal;

	//	MString n = "";
	//	n += vertexNormal[i];
	//	MGlobal::displayInfo("normals: " + n);

	//}

	//if (avarageNormal.length() < 0.001) {
	//	avarageNormal = MVector(0.0, 1.0, 0.0);
	//}
	//else avarageNormal.normalize();
	//translation = avarageNormal;
}

void MNodeFunction(MObject &node, void* clientData)
{
	MStatus res = MS::kSuccess;

	if (node.hasFn(MFn::kMesh)) {

		MFnMesh mesh(node);
		
		/*getNodesInTheScene(mesh);*/

	}

}

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

	MCallbackId nodeId = MDGMessage::addNodeAddedCallback(MNodeFunction, kDefaultNodeType, NULL, &res);


	if (res == MS::kSuccess) {
		idList.append(nodeId);
		MGlobal::displayInfo("added node callback Succeeded");
	}
	else {
		MGlobal::displayInfo("added node callback Failed");
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
