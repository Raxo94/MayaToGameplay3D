#pragma once

#include "gameplay.h"
#include "MayaData.h"

using namespace gameplay;
 

inline Node* getManualCamera()
{
	Node* cameraNode = Node::create("Camera");

	cameraNode->setTranslationX(3);
	cameraNode->setTranslationY(5);
	cameraNode->setTranslationZ(10);

	Matrix roationMatrix;
	Quaternion Test(-0.142, 0.126, 0, 1);
	Matrix::createRotation(Test, &roationMatrix);
	cameraNode->setRotation(roationMatrix);

	Camera* cam = cameraNode->getCamera();

	cam = Camera::createPerspective(0, 0, 0, 0);

	float camMatrix[4][4];
	Matrix projectionMatrix(camMatrix[0][0], camMatrix[1][0], camMatrix[2][0], camMatrix[3][0],
		camMatrix[0][1], camMatrix[1][1], camMatrix[2][1], camMatrix[3][1],
		camMatrix[0][2], camMatrix[1][2], -camMatrix[2][2], -camMatrix[3][2],
		camMatrix[0][3], camMatrix[1][3], camMatrix[2][3], camMatrix[3][3]);

	cam->setProjectionMatrix(projectionMatrix);
	cam->resetProjectionMatrix();


	cam->setNearPlane(0.25);
	cam->setFarPlane(100);
	cam->setFieldOfView(27);
	cam->setAspectRatio((float)1280 / (float)720);


	cameraNode->setCamera(cam);
	return cameraNode;
}


inline Node* createMayaCamera(HeaderTypeCamera* Mcamera)
{
	Node* cameraNode = Node::create(Mcamera->CameraName);

	cameraNode->setTranslationX(Mcamera->translation[0]);
	cameraNode->setTranslationY(Mcamera->translation[1]);
	cameraNode->setTranslationZ(Mcamera->translation[2]);

	Matrix roationMatrix;
	Quaternion Test(Mcamera->rotation[0], Mcamera->rotation[1], Mcamera->rotation[2],Mcamera->rotation[3]);
	//Quaternion Test(-0.142, 0.126, 0, 1);
	Matrix::createRotation(Test, &roationMatrix);
	cameraNode->setRotation(roationMatrix);

	Camera* cam = cameraNode->getCamera();

	 
	if (Mcamera->isPerspective == true)
	{
		cam = Camera::createPerspective(0, 0, 0, 0);
		cam->setFieldOfView(Mcamera->fieldOfView);
	}
	else
	{
		cam->ORTHOGRAPHIC;
		cam = Camera::createOrthographic(0, 0 ,0, 0, 0);
	}
	
	Mcamera->projectionMatrix[10] *= -1;
	Mcamera->projectionMatrix[14] *= -1;

	cam->setProjectionMatrix(Mcamera->projectionMatrix);
	cam->setNearPlane(Mcamera->nearPlane);
	cam->setFarPlane(Mcamera->farPlane);
	
	cam->setAspectRatio((float)1280 / (float)720);

	cameraNode->setCamera(cam);
	return cameraNode;
}



static Model* createCubeMesh(float size = 1.0f)
{
	float a = size * 0.5f;
	float vertices[] =
	{
		-a, -a,  a,    0.0,  0.0,  1.0,   0.0, 0.0,
		a, -a,  a,    0.0,  0.0,  1.0,   1.0, 0.0,
		-a,  a,  a,    0.0,  0.0,  1.0,   0.0, 1.0,
		a,  a,  a,    0.0,  0.0,  1.0,   1.0, 1.0,
		-a,  a,  a,    0.0,  1.0,  0.0,   0.0, 0.0,
		a,  a,  a,    0.0,  1.0,  0.0,   1.0, 0.0,
		-a,  a, -a,    0.0,  1.0,  0.0,   0.0, 1.0,
		a,  a, -a,    0.0,  1.0,  0.0,   1.0, 1.0,
		-a,  a, -a,    0.0,  0.0, -1.0,   0.0, 0.0,
		a,  a, -a,    0.0,  0.0, -1.0,   1.0, 0.0,
		-a, -a, -a,    0.0,  0.0, -1.0,   0.0, 1.0,
		a, -a, -a,    0.0,  0.0, -1.0,   1.0, 1.0,
		-a, -a, -a,    0.0, -1.0,  0.0,   0.0, 0.0,
		a, -a, -a,    0.0, -1.0,  0.0,   1.0, 0.0,
		-a, -a,  a,    0.0, -1.0,  0.0,   0.0, 1.0,
		a, -a,  a,    0.0, -1.0,  0.0,   1.0, 1.0,
		a, -a,  a,    1.0,  0.0,  0.0,   0.0, 0.0,
		a, -a, -a,    1.0,  0.0,  0.0,   1.0, 0.0,
		a,  a,  a,    1.0,  0.0,  0.0,   0.0, 1.0,
		a,  a, -a,    1.0,  0.0,  0.0,   1.0, 1.0,
		-a, -a, -a,   -1.0,  0.0,  0.0,   0.0, 0.0,
		-a, -a,  a,   -1.0,  0.0,  0.0,   1.0, 0.0,
		-a,  a, -a,   -1.0,  0.0,  0.0,   0.0, 1.0,
		-a,  a,  a,   -1.0,  0.0,  0.0,   1.0, 1.0
	};
	short indices[] =
	{
		0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 18, 17, 19, 20, 21, 22, 22, 21, 23
	};
	unsigned int vertexCount = 24;
	unsigned int indexCount = 36;
	VertexFormat::Element elements[] =
	{
		VertexFormat::Element(VertexFormat::POSITION, 3),
		VertexFormat::Element(VertexFormat::NORMAL, 3),
		VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
	};
	Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
	if (mesh == NULL)
	{
		GP_ERROR("Failed to create mesh.");
		return NULL;
	}
	mesh->setVertexData(vertices, 0, vertexCount);
	MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
	meshPart->setIndexData(indices, 0, indexCount);

	
	
	Model* model = Model::create(mesh);
	return model;
}


static Model* createMayaMesh(HeaderTypeMesh* MayaMesh)
{
	
	VertexFormat::Element elements[] =
	{
		VertexFormat::Element(VertexFormat::POSITION, 3),
		VertexFormat::Element(VertexFormat::NORMAL, 3),
		VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
	};
	Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), MayaMesh->vertexCount, false);

	if (mesh == NULL)
	{
		GP_ERROR("Failed to create mesh.");
		return NULL;
	}

	mesh->setVertexData(MayaMesh->vertexArray, 0, MayaMesh->vertexCount);

	Model* model = Model::create(mesh);
	return model;
}


static Material* createDefaultMaterial(Scene* scene)
{
	Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", "POINT_LIGHT_COUNT 1");
	material->setParameterAutoBinding("u_worldViewMatrix", RenderState::AutoBinding::WORLD_VIEW_MATRIX);
	material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
	material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");

	// Set the ambient color of the material.
	material->getParameter("u_ambientColor")->setValue(Vector3(0.4f, 0.4f, 0.4f));
	material->getParameter("u_diffuseColor")->setValue(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	Node* lightNode = scene->findNode("pointLightShape1");
	material->getParameter("u_pointLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
	material->getParameter("u_pointLightRangeInverse[0]")->bindValue(lightNode->getLight(), &Light::getRangeInverse);
	material->getParameter("u_pointLightPosition[0]")->bindValue(lightNode, &Node::getTranslationView);



	material->getStateBlock()->setCullFace(true);
	material->getStateBlock()->setDepthTest(true);
	material->getStateBlock()->setDepthWrite(true);
	return material;
}


static Material* createMayaMaterial(Scene* scene,HeaderTypeMaterial* mayaMaterial)
{
	
	if (mayaMaterial->hasTexture == true)
	{
		Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag", "POINT_LIGHT_COUNT 1"); //REMOVED"DIRECTIONAL_LIGHT_COUNT 1"
		material->setParameterAutoBinding("u_worldViewMatrix", RenderState::AutoBinding::WORLD_VIEW_MATRIX);
		material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
		material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");

		// Set the ambient color of the material.
		material->getParameter("u_ambientColor")->setValue(Vector3(0.3f, 0.3f, 0.3f));

		string texture = mayaMaterial->textureFilepath;
		//Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue("res/png/crate.png", true); 	// Load the texture from file.
		Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue(mayaMaterial->textureFilepath, true); 	// Load the texture from file.
		sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);


		Node* lightNode = scene->findNode("pointLightShape1");
		material->getParameter("u_pointLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
		material->getParameter("u_pointLightRangeInverse[0]")->bindValue(lightNode->getLight(), &Light::getRangeInverse);
		material->getParameter("u_pointLightPosition[0]")->bindValue(lightNode, &Node::getTranslationView);

		material->getStateBlock()->setCullFace(true);
		material->getStateBlock()->setDepthTest(true);
		material->getStateBlock()->setDepthWrite(true);
		return material;
	}
	else
	{
		Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", "POINT_LIGHT_COUNT 1");
		material->setParameterAutoBinding("u_worldViewMatrix", RenderState::AutoBinding::WORLD_VIEW_MATRIX);
		material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
		material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");

		// Set the ambient color of the material.
		material->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));
		material->getParameter("u_diffuseColor")->setValue(Vector4(mayaMaterial->color[0]* mayaMaterial->diffuse, mayaMaterial->color[1] * mayaMaterial->diffuse, mayaMaterial->color[2] * mayaMaterial->diffuse, 1.0f));
		Node* lightNode = scene->findNode("pointLightShape1");
		material->getParameter("u_pointLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
		material->getParameter("u_pointLightRangeInverse[0]")->bindValue(lightNode->getLight(), &Light::getRangeInverse);
		material->getParameter("u_pointLightPosition[0]")->bindValue(lightNode, &Node::getTranslationView);



		material->getStateBlock()->setCullFace(true);
		material->getStateBlock()->setDepthTest(true);
		material->getStateBlock()->setDepthWrite(true);
		return material;
	}
	
}



