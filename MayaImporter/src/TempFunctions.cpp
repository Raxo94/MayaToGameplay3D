#include "gameplay.h"

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

struct Vertex2
{
	Vector3 pos;
	Vector3 norm;
	Vector2 UV;
};

struct Vertex
{
	float pos[3] ;
	float norm[3] ;
	float UV[2] ;
};

static Model* createCubeMesh(float size = 1.0f)
{
	float a = size * 0.5f;
	unsigned int vertexCount = 24;
	unsigned int indexCount = 36;

	Vertex vertices[]
	{
		{ {-a, -a,  a}, {0.0,  0.0,  1.0},  {0.0, 0.0} },
		{ {a, -a,  a},  {0.0,  0.0,  1.0},  {1.0, 0.0} },
		{ {-a,  a,  a}, {0.0,  0.0,  1.0},  {0.0, 1.0} },
		{ {a,  a,  a},  {0.0,  0.0,  1.0},  {1.0, 1.0} },
		{ {-a, a, a},   {0.0,  1.0,  0.0},  {0.0, 0.0} },
		{ {a,  a, a},   {0.0,  1.0,  0.0},  {1.0, 0.0} },
		{ {-a, a,-a},   {0.0,  1.0,  0.0},  {0.0, 1.0} },
		{ {a, a, -a},   {0.0,  1.0,  0.0},  {1.0, 1.0} },
		{ {-a, a,-a},   {0.0,  0.0, -1.0},  {0.0, 0.0} },
		{ { a, a,-a},   { 0.0, 0.0, -1.0},  { 1.0,0.0} },
		{ {-a,-a,-a},   { 0.0, 0.0, -1.0},  { 0.0,1.0} },
		{ {a, -a,-a},   {0.0,  0.0, -1.0},  {1.0, 1.0} },
		{ {-a, -a, -a}, {0.0, -1.0,  0.0},  {0.0, 0.0} },
		{ {a, -a, -a},  {0.0, -1.0,  0.0},  {1.0, 0.0} },
		{ {-a, -a,  a}, { 0.0, -1.0, 0.0},  {0.0, 1.0} },
		{ {a, -a,  a},  {0.0, -1.0,  0.0},  {1.0, 1.0} },
		{ {a, -a,  a},  {1.0,  0.0,  0.0},  {0.0, 0.0} },
		{ {a, -a, -a},  {1.0,  0.0,  0.0},  {1.0, 0.0} },
		{ {a,  a,  a},  {1.0,  0.0,  0.0},  {0.0, 1.0} },
		{ {a,  a, -a},  {1.0,  0.0,  0.0},  {1.0, 1.0} },
		{ {-a, -a, -a}, {-1.0,  0.0, 0.0},  {0.0,0.0 } },
		{ {-a, -a,  a}, {-1.0,  0.0, 0.0},  {1.0,0.0 } },
		{ {-a,  a, -a}, {-1.0,  0.0, 0.0},  {0.0, 1.0} },
		{ {-a,  a,  a}, {-1.0,  0.0, 0.0},  {1.0, 1.0} }
	};

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