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

	points.resize(pts.length());

	MVector vertexNormal;
	MIntArray normalList, normalCount;
	MFloatArray u, v;

	mesh.getNormalIds(normalCount, normalList);
	mesh.getUVs(u, v, 0);

	for (i = 0; i < points.size(); i++) {

		points.at(i).pos[0] = pts[i].x;
		points.at(i).pos[1] = pts[i].y;
		points.at(i).pos[2] = pts[i].z;

		mesh.getVertexNormal(normalList[i], vertexNormal, MSpace::kObject);;

		points.at(i).nor[0] = vertexNormal[0];
		points.at(i).nor[1] = vertexNormal[1];
		points.at(i).nor[2] = vertexNormal[2];

		points.at(i).uv[0] = u[i];
		points.at(i).uv[1] = v[i];

		MString px = "";
		px += points.at(i).pos[0];

		MString py = "";
		py += points.at(i).pos[1];

		MString pz = "";
		pz += points.at(i).pos[2];

		MGlobal::displayInfo("pos in x: " + px + " pos in y: " + py + " pos in z: " + pz);

		MString nx = "";
		nx += points.at(i).nor[0];

		MString ny = "";
		ny += points.at(i).nor[1];

		MString nz = "";
		nz += points.at(i).nor[2];

		MGlobal::displayInfo("normal in x: " + nx + " normal in y: " + ny + " normal in z: " + nz);

		MString u = "";
		u += points.at(i).uv[0];

		MString v = "";
		v += points.at(i).uv[1];

		MGlobal::displayInfo("u coord: " + u + " v coord: " + v);
	}


	CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	circPtr->push(points.data(), sizeof(float) * 3 * points.size());

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
