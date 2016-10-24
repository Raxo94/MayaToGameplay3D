#include <Windows.h>
#include "Linker.h"
#include "CircBuffer.h"
#include "PluginDeclarations.h"

#define	kVectorEpsilon 1.0e-3
#define     TOP         0
#define     FRONT       1
#define     SIDE        2
#define     PERSP       3

//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref__abc_export_2_maya_mesh_writer_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_mesh_op_cmd_2mesh_op_fty_action_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_obj_export_2obj_export_8cpp_example_html

//camera links
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_class_m_ui_message_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_class_m3d_view_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_move_tool_2move_tool_8cpp_example_html

void GetMeshes(MFnMesh &mesh)
{
	MGlobal::displayInfo("current mesh: " + mesh.name());

	mesh.getPoints(pts, MSpace::kObject);
	mesh.getUVs(u, v, 0);

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getTriangles(triangleCounts, triangleVertexIDs);
	mesh.getNormals(normals, MSpace::kObject);

	points.resize(triangleVertexIDs.length());
	
	mesh.getNormalIds(normalCount, normalList);
	

	for (i = 0; i < triangleVertexIDs.length(); i++) {

		points.at(i).pos[0] = pts[triangleVertexIDs[i]].x;
		points.at(i).pos[1] = pts[triangleVertexIDs[i]].y;
		points.at(i).pos[2] = pts[triangleVertexIDs[i]].z;

		points.at(i).nor[0] = normals[triangleVertexIDs[i]].x;
		points.at(i).nor[1] = normals[triangleVertexIDs[i]].y;
		points.at(i).nor[2] = normals[triangleVertexIDs[i]].z;

		points.at(i).uv[0] = u[triangleVertexIDs[i]];
		points.at(i).uv[1] = v[triangleVertexIDs[i]];

	}

	offset = 0;

	int Type = MessageType::MayaMesh;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	HeaderType header{"mesh", points.size() * sizeof(vertices), points.size()};
	memcpy( (message + offset), &header, sizeof(HeaderType));
	offset += sizeof(HeaderType);

	memcpy(message + offset, points.data(), sizeof(vertices) * points.size());
	offset += sizeof(vertices) * points.size();
	
	circPtr->push(message, offset);

}


void CreateMeshCallback(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void* clientData)
{

	MFnMesh mesh(plug.node(), &res);

	if (res == MS::kSuccess) {

		GetMeshes(mesh);
	}
}

void MNodeFunction(MDagPath &child, MDagPath &parent, void* clientData)
{
	if (child.hasFn(MFn::kMesh)) {
		
		MCallbackId meshCreateId = MNodeMessage::addAttributeChangedCallback(child.node(), CreateMeshCallback);

		if (res == MS::kSuccess) {
			idList.append(meshCreateId);
			MGlobal::displayInfo("mesh callback Succeeded");
		}
		else {
			MGlobal::displayInfo("mesh callback Failed");
		}

	}

}

void GetCamera(M3dView &camView)
{
	CameraHeader camHeader;

	camView = M3dView::active3dView();
	res = camView.getCamera(camera);
	MFnCamera fnCam(camera);

	MGlobal::displayInfo("Current camera name: " + fnCam.name());
	
	MSpace::Space space = MSpace::kWorld;

	MVector upDirection = fnCam.upDirection(space);
	MVector rightDirection = fnCam.rightDirection(space);
	 
	if (fnCam.isOrtho()) {
	MGlobal::displayInfo("ORTHOGRAPHIC VIEW");
	camHeader.orthographic = true;

		if (upDirection.isEquivalent(MVector::zNegAxis, kVectorEpsilon)) {
			currentView = TOP;
		
			MGlobal::displayInfo("TOP view");
		}
		else if (rightDirection.isEquivalent(MVector::xAxis, kVectorEpsilon)) {
			currentView = FRONT;

			MGlobal::displayInfo("FRONT view");
		}
		else {
			currentView = SIDE;
			MGlobal::displayInfo("SIDE view");
		}
	}
	else {
		currentView = PERSP;
		MGlobal::displayInfo("PERSPECTIVE VIEW");
		camHeader.orthographic = false;
	}

}

//void timeElapsedFunction(float elapsedTime, float lastTime, void *clientData)
//{
//	MStatus res = MS::kSuccess;
//	
//}

EXPORT MStatus initializePlugin(MObject obj)
{
	int messageSize = (1<<20)/4;
	message = new char[messageSize];
	circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	MFnPlugin myPlugin(obj, "Maya Plugin", "1.0", "Any", &res);
	if (MFAIL(res)) {
		CHECK_MSTATUS(res);
	}

	MItDag it(MItDag::kBreadthFirst);
	while (it.isDone() == false)
	{
		if (it.currentItem().hasFn(MFn::kMesh)) {
			MFnMesh mesh(it.currentItem());
			GetMeshes(mesh);
		}

		//if (it.currentItem().hasFn(MFn::kCamera)) {

		//}

		it.next();
	}

	GetCamera(camView);

	//MCallbackId timeId = MTimerMessage::addTimerCallback(5, timeElapsedFunction, NULL, &res);
	//
	//if (res == MS::kSuccess) {
	//	idList.append(timeId);
	//	MGlobal::displayInfo("time callback Succeeded");
	//}
	//else {
	//	MGlobal::displayInfo("time callback Failed");
	//}

	//MCallbackId nodeId = MDGMessage::addNodeAddedCallback(MNodeFunction, kDefaultNodeType, NULL, &res);
	MCallbackId nodeId = MDagMessage::addChildAddedCallback(MNodeFunction, NULL, &res);

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
	delete[] message;

	MFnPlugin plugin(obj);

	MGlobal::displayInfo("Maya plugin unloaded!");

	MMessage::removeCallbacks(idList);

	return MS::kSuccess;
}
