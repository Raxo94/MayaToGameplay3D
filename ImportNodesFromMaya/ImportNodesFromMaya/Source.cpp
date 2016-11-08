#include <Windows.h>
#include "Linker.h"
#include "CircBuffer.h"
#include "PluginDeclarations.h"

void GetMeshes(MFnMesh &mesh)
{
	mesh.getPoints(pts, MSpace::kObject);
	mesh.getUVs(u, v, 0);
	mesh.getAssignedUVs(uvCounts, uvIDs); //indices for UV:s

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getVertices(vertexCounts, polygonVertexIDs); //get vertex polygon indices
	mesh.getNormals(normals, MSpace::kObject);

	points.resize(triangleIndices.length());
	
	mesh.getNormalIds(normalCount, normalList);
	

	for (i = 0; i < triangleIndices.length(); i++) { //for each triangle index (36)

		points.at(i).pos[0] = pts[polygonVertexIDs[triangleIndices[i]]].x;
		points.at(i).pos[1] = pts[polygonVertexIDs[triangleIndices[i]]].y;
		points.at(i).pos[2] = pts[polygonVertexIDs[triangleIndices[i]]].z;

		points.at(i).nor[0] = normals[normalList[triangleIndices[i]]].x;
		points.at(i).nor[1] = normals[normalList[triangleIndices[i]]].y;
		points.at(i).nor[2] = normals[normalList[triangleIndices[i]]].z;

		points.at(i).uv[0] = u[uvIDs[triangleIndices[i]]];
		points.at(i).uv[1] = v[uvIDs[triangleIndices[i]]];

	}
	
	offset = 0;

	int Type = MessageType::MayaMesh;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&meshHeader, mesh.name().asChar(), sizeof(const char[256]));
	meshHeader.vertexArray = nullptr;
	meshHeader.vertexCount = points.size();

	memcpy( (message + offset), &meshHeader, sizeof(HeaderTypeMesh));
	offset += sizeof(HeaderTypeMesh);

	memcpy(message + offset, points.data(), sizeof(vertices) * points.size());
	offset += sizeof(vertices) * points.size();
	
	circPtr->push(message, offset);

}

void GetTransform(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void* clientData)
{
	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{

		MFnTransform transform(plug.node(), &res);

		if (res == MS::kSuccess)
		{

			MObject child = transform.child(0);
			MFnMesh mesh(child);

			transformHeader.translation[0] = transform.getTranslation(MSpace::kTransform).x;
			transformHeader.translation[1] = transform.getTranslation(MSpace::kTransform).y;
			transformHeader.translation[2] = transform.getTranslation(MSpace::kTransform).z;

			transform.getScale(transformHeader.scale);

			transform.getRotationQuaternion(transformHeader.rotation[0], transformHeader.rotation[1], transformHeader.rotation[2], transformHeader.rotation[3], MSpace::kTransform);

			offset = 0;

			int Type = MessageType::MayaTransform;
			memcpy(message, &Type, sizeof(int));
			offset += sizeof(int);

			memcpy(&transformHeader.meshName, mesh.name().asChar(), sizeof(const char[256]));

			memcpy((message + offset), &transformHeader, sizeof(HeaderTypeTransform));
			offset += sizeof(HeaderTypeTransform);

			circPtr->push(message, offset);
		}
	}
}

void GetTransform(MFnTransform &transform,MFnMesh &mesh)
{

	transformHeader.translation[0] = transform.getTranslation(MSpace::kTransform).x;
	transformHeader.translation[1] = transform.getTranslation(MSpace::kTransform).y;
	transformHeader.translation[2] = transform.getTranslation(MSpace::kTransform).z;

	transform.getScale(transformHeader.scale);

	transform.getRotationQuaternion(transformHeader.rotation[0], transformHeader.rotation[1], transformHeader.rotation[2], transformHeader.rotation[3], MSpace::kTransform);

	offset = 0;

	int Type = MessageType::MayaTransform;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&transformHeader.meshName, mesh.name().asChar(), sizeof(const char[256]));

	memcpy((message + offset), &transformHeader, sizeof(HeaderTypeTransform));
	offset += sizeof(HeaderTypeTransform);
	circPtr->push(message, offset);
	
}

void CreateMeshCallback(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void* clientData)
{

	if (msg & MNodeMessage::AttributeMessage::kAttributeSet) {
		MFnMesh mesh(plug.node(), &res);

		if (res == MS::kSuccess) {

			GetMeshes(mesh);

		}
	}
}

void MNodeFunction(MDagPath &child, MDagPath &parent, void* clientData)
{
	if (child.hasFn(MFn::kMesh)) {
		
		MCallbackId meshCreateId = MNodeMessage::addAttributeChangedCallback(child.node(), CreateMeshCallback,&res);
		isCallbackaSuccess(res, meshCreateId);

		if (parent.hasFn(MFn::kTransform))
		{
			MCallbackId transformId = MNodeMessage::addAttributeChangedCallback(parent.node(), GetTransform,&res);
			isCallbackaSuccess(res, transformId);

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
	}
	else {
		camHeader.isPerspective = true;

	}

}

void GetMaterial(MObject &iteratorNode)
{
	matHeader = HeaderTypeMaterial(); // reseting matheader
	
	MPlugArray textureGroup;
	MPlugArray shadingGoupArray;
	MPlugArray dagSetMemberConnections;
	MPlugArray objInstArray;
	MFnDependencyNode materialNode(iteratorNode);

	MString materialName = materialNode.name();

	MGlobal::displayInfo("material name: " + materialName);

	MPlug outColor = materialNode.findPlug("outColor"); //to go further in the plugs
	MPlug color = materialNode.findPlug("color"); //to get the color values
	MPlug diffuse = materialNode.findPlug("diffuse"); //to get the diffuse of the material
	color.connectedTo(textureGroup, true, false, &res); //color is connected to a destination

	MObject data;
	color.getValue(data);
	MFnNumericData nData(data);
	nData.getData(matHeader.color[0], matHeader.color[1], matHeader.color[2]);
	diffuse.getValue(matHeader.diffuse);

	matHeader.hasTexture = false;

	//if the length is not 0 then we have a texture
	for (int i = 0; i < textureGroup.length(); i++)
	{
		MFnDependencyNode textureNode(textureGroup[i].node());

		MString filename;
		textureNode.findPlug("fileTextureName").getValue(filename);

		memcpy(&matHeader.textureFilepath, filename.asChar(), sizeof(const char[256]));

		matHeader.hasTexture = true;
	}


	//find surfaceShader of the material
	outColor.connectedTo(shadingGoupArray, false, true, &res); //true = connection to source (outColor) 

	for (int i = 0; i < shadingGoupArray.length(); i++) {
		if (shadingGoupArray[i].node().hasFn(MFn::kShadingEngine)) {
			
			MFnDependencyNode shadingNode(shadingGoupArray[i].node());

			if (strcmp(shadingNode.name().asChar(), "initialParticleSE") != 0) {

				//ShadingNode = initialShadingGroup if lambert1 and Blinn1SG if blinn

				MPlug dagSetMember = shadingNode.findPlug("dagSetMembers", &res);

				for (int child = 0; child < dagSetMember.numElements(); child++) {
					dagSetMember[child].connectedTo(dagSetMemberConnections, true, false, &res); //true = connection to destination
					
					for (int d = 0; d < dagSetMemberConnections.length(); d++) {
						MFnDependencyNode dagSetMemberNode(dagSetMemberConnections[d].node());
						if (strcmp(dagSetMemberNode.name().asChar(), "shaderBallGeom1") != 0) {

							MFnMesh mesh(dagSetMemberNode.object());

							memcpy(&meshStruct, mesh.name().asChar(), sizeof(Meshes));
							meshVector.push_back(meshStruct);
							matHeader.amountOfMeshes += 1;

						}
					}
					
				}
			}
		}
	}

	offset = 0;

	int Type = MessageType::MayaMaterial;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&matHeader.materialName, materialNode.name().asChar(), sizeof(const char[256]));
	memcpy((message + offset), &matHeader, sizeof(HeaderTypeMaterial));
	offset += sizeof(HeaderTypeMaterial);


	memcpy(message + offset, meshVector.data(), sizeof(Meshes) * meshVector.size());
	offset += sizeof(Meshes) * meshVector.size();
	

	circPtr->push(message, offset);
	meshVector.clear();
}
void UpdateMaterial(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void* clientData)
{
	if (msg & MNodeMessage::AttributeMessage::kAttributeSet) {

		GetMaterial(plug.node());

	}

}
void UpdateCamera(const MString &panelName, void* clientdata)
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

		}
		else {

			camHeader.isPerspective = true;

		}

	}
	else {

		//do nothing
	}

	projMatrix = fnCam.projectionMatrix();
	memcpy(&camHeader.projectionMatrix, &projMatrix, sizeof(float) * 16);

	parentCamera = fnCam.parent(0);
	if (parentCamera.hasFn(MFn::kTransform))
	{
		MFnTransform transformParent(parentCamera);

		camHeader.translation[0] = transformParent.getTranslation(MSpace::kTransform).x;
		camHeader.translation[1] = transformParent.getTranslation(MSpace::kTransform).y;
		camHeader.translation[2] = transformParent.getTranslation(MSpace::kTransform).z;

		transformParent.getRotationQuaternion(camHeader.rotation[0], camHeader.rotation[1], camHeader.rotation[2], camHeader.rotation[3], MSpace::kTransform);
	}

	camHeader.nearPlane = fnCam.nearClippingPlane();
	camHeader.farPlane = fnCam.farClippingPlane();
	camHeader.fieldOfView = fnCam.horizontalFieldOfView();

	offset = 0;

	int Type = MessageType::MayaCamera;
	memcpy(message, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&camHeader, fnCam.name().asChar(), sizeof(const char[256]));

	memcpy((message + offset), &camHeader, sizeof(HeaderTypeCamera));
	offset += sizeof(HeaderTypeCamera);

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

	MItDag it(MItDag::kDepthFirst);
	MObject parent;
	//MItDag parent(MItDag::kDepthFirst);
	while (it.isDone() == false)
	{
		if (it.currentItem().hasFn(MFn::kMesh))
		{
			MFnMesh mesh(it.currentItem());
			GetMeshes(mesh);
			MCallbackId meshCreateId = MNodeMessage::addAttributeChangedCallback(it.currentItem(), CreateMeshCallback, &res);
			isCallbackaSuccess(res, meshCreateId);

			
			if (parent.hasFn(MFn::kTransform))
			{
				MCallbackId transformId = MNodeMessage::addAttributeChangedCallback(parent, GetTransform, &res);
				isCallbackaSuccess(res, transformId);
				
				MFnTransform transform(parent);
				GetTransform(transform, mesh);
			}

		}
		parent = it.currentItem();
		it.next();
	}

	MItDependencyNodes itDepNode(MFn::kLambert);

	while (itDepNode.isDone() == false)
	{
		MObject mNode = itDepNode.item();

		GetMaterial(mNode);

		MCallbackId MplugId = MNodeMessage::addAttributeChangedCallback(mNode, UpdateMaterial, &res);
		isCallbackaSuccess(res, MplugId);

		itDepNode.next();
	}

	GetCamera();

	MCallbackId nodeId = MDagMessage::addChildAddedCallback(MNodeFunction, NULL, &res);
	isCallbackaSuccess(res, nodeId);


	MCallbackId viewId1 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel1", UpdateCamera, NULL, &res);
	MCallbackId viewId2 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel2", UpdateCamera, NULL, &res);
	MCallbackId viewId3 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel3", UpdateCamera, NULL, &res);
	MCallbackId viewId4 = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel4", UpdateCamera, NULL, &res);

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


bool isCallbackaSuccess(MStatus result, MCallbackId MplugId)
{
	if (res == MS::kSuccess) {
		idList.append(MplugId);
		MGlobal::displayInfo("mesh callback Succeeded");
		return true;

	}
	else {
		MGlobal::displayInfo("mesh callback Failed");
		return false;
	}
}
