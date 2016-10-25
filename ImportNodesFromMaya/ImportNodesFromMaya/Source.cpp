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


	HeaderType header; //{ "sad", points.size() * sizeof(vertices), points.size() };
	memcpy(&header, mesh.name().asChar(), sizeof(const char[256]));
	header.vertexArray = points.size() * sizeof(vertices);
	header.vertexCount = points.size();

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

void GetCamera()
{
	camView = M3dView::active3dView();

	camView.getCamera(camera);
	MFnCamera fnCam(camera);

	MSpace::Space space = MSpace::kWorld;

	MVector upDirection = fnCam.upDirection(space);

	MVector rightDirection = fnCam.rightDirection(space);
	if (fnCam.isOrtho()) {
		camHeader.isPerspective = false;
		MGlobal::displayInfo("ORTHOGRAPHIC VIEW");

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
		camHeader.isPerspective = true;

	}

}

void StringFunc(const MString &panelName, void* clientdata)
{
	MString activeCameraPanelName;
	activeCameraPanelName = MGlobal::executeCommandStringResult("getPanel -wf");
	camView = M3dView::active3dView();

	camView.getCamera(camera);
	MFnCamera fnCam(camera);
	
	MSpace::Space space = MSpace::kWorld;

	MVector upDirection = fnCam.upDirection(space);
	MVector rightDirection = fnCam.rightDirection(space);

	//if panelName(if we are in ortho or persp) is equal to the active panel 
	//check the view of the camera
	if (activeCameraPanelName == panelName) {

		if (fnCam.isOrtho()) {
			camHeader.isPerspective = false;
			MGlobal::displayInfo("ORTHOGRAPHIC VIEW");

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
			camHeader.isPerspective = true;

		}

	}
	else {

		//do nothing
	}

	projMatrix = fnCam.projectionMatrix();
	memcpy(&camHeader.projectionMatrix, &projMatrix, sizeof(float) * 16);

	parent = fnCam.parent(0);
	if (parent.hasFn(MFn::kTransform))
	{
		MFnTransform transformParent(parent);

		camHeader.translation[0] = transformParent.getTranslation(MSpace::kTransform).x;
		camHeader.translation[1] = transformParent.getTranslation(MSpace::kTransform).y;
		camHeader.translation[2] = transformParent.getTranslation(MSpace::kTransform).z;
		
		transformParent.getRotationQuaternion(rotValues[0], rotValues[1], rotValues[2], rotValues[3], MSpace::kTransform);
		memcpy(&camHeader.rotation, &rotValues, sizeof(double) * 4);
	}

	camHeader.nearPlane = fnCam.nearClippingPlane();
	camHeader.farPlane = fnCam.farClippingPlane();
	camHeader.fieldOfView = fnCam.horizontalFieldOfView();

	offset = 0;

	int Type = MessageType::MayaCamera;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&camHeader, fnCam.name().asChar(), sizeof(const char[256]));

	memcpy((message + offset), &camHeader, sizeof(CameraHeader));
	offset += sizeof(CameraHeader);

	circPtr->push(message, offset);

}

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

		it.next();
	}

	GetCamera();

	MCallbackId nodeId = MDagMessage::addChildAddedCallback(MNodeFunction, NULL, &res);

	if (res == MS::kSuccess) {
		idList.append(nodeId);
		MGlobal::displayInfo("node callback Succeeded");
	}
	else {
		MGlobal::displayInfo("node callback Failed");
	}

	MCallbackId viewId1 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel1", StringFunc, NULL, &res);
	MCallbackId viewId2 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel2", StringFunc, NULL, &res);
	MCallbackId viewId3 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel3", StringFunc, NULL, &res);
	MCallbackId viewId4 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel4", StringFunc, NULL, &res);

	if (res == MS::kSuccess) {
		idList.append(viewId1);
		idList.append(viewId2);
		idList.append(viewId3);
		idList.append(viewId4);

		MGlobal::displayInfo("view callback Succeeded");
	}
	else {
		MGlobal::displayInfo("view callback Failed");
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
