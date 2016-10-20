#include "gameplay.h"
#include "MayaData.h"

using namespace::gameplay;
 



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


static Model* createDynamicMesh(Vertex* VertexArray, int vertexCount)
{
	
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

	mesh->setVertexData(VertexArray, 0, vertexCount);

	Model* model = Model::create(mesh);
	return model;
}


static Material* createDefaultMaterial()
{

	Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag"); //REMOVED"DIRECTIONAL_LIGHT_COUNT 1"

	// These parameters are normally set in a .material file but this example sets them programmatically.
// Bind the uniform "u_worldViewProjectionMatrix" to use the WORLD_VIEW_PROJECTION_MATRIX from the scene's active camera and the node that the model belongs to.
	material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
	material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
	// Set the ambient color of the material.
	material->getParameter("u_ambientColor")->setValue(Vector3(0.2f, 0.2f, 0.2f));

	// Bind the light's color and direction to the material.
	//material->getParameter("u_directionalLightColor[0]")->setValue(lightNode->getLight()->getColor());
	//material->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorWorld);

	// Load the texture from file.
	Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue("res/png/crate.png", true);
	sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);


	material->getStateBlock()->setCullFace(true);
	material->getStateBlock()->setDepthTest(true);
	material->getStateBlock()->setDepthWrite(true);

	return material;
}


