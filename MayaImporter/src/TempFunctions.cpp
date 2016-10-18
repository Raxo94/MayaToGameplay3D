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
//inline Model* getManualModel()
//{
//	VertexFormat::Element elements[] = {
//		VertexFormat::Element(VertexFormat::POSITION, 3),
//		VertexFormat::Element(VertexFormat::NORMAL, 3),
//		VertexFormat::Element(VertexFormat::COLOR, 2)
//	};
//
//	unsigned int vertexCount = 3;
//	const VertexFormat vertFormat(elements, vertexCount);
//	
//	Vertex Vertices[5];
//
//	Vertices[0] = { { 0,0,0 },{ 0,1,0 },{ 255,0,0 } };
//
//	Vertices[1] = { { 0,5,0 },{ 0,1,0 },{ 255,0,0 } };
//
//	Vertices[2] = { { 3,0,0 },{ 0,1,0 },{ 0,0,0 } };
//
//	Vertices[3] = { { 0,0,0 },{ 0,1,0 },{ 0,0 } };
//
//	Vertices[4] = { { -3,4,0 },{ 0,1,0 },{ 0,0 } };
//
//	Vertices[5] = { { -3,0,0 },{ 0,1,0 },{ 0,0 } };
//
//	Mesh* mesh = Mesh::createMesh(vertFormat, vertexCount, true);
//	
//	if (mesh == NULL)
//	{
//		GP_ERROR("Failed to create mesh.");
//		return NULL;
//	}
//
//	mesh->setPrimitiveType(Mesh::TRIANGLES);
//	mesh->setVertexData(Vertices); //
//
//	Model* model = Model::create(mesh);
//	return model;
//
//
//}


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

	/*short indices[] =
	{
		0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 18, 17, 19, 20, 21, 22, 22, 21, 23
	};*/
	
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
	//MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
	//meshPart->setIndexData(indices, 0, indexCount);
	
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