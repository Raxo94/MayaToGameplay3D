#include "CustomRenderer.h"
#include "TempFunctions.cpp"
// Declare our game instance
CustomRenderer game;


struct Vertex
{
	Vector3 pos;
	Vector3 norm;
	Vector2 UV;
};

CustomRenderer::CustomRenderer()
    : _scene(NULL), _wireframe(false)
{
}

void CustomRenderer::initialize()
{
    // Load game scene from file
    _scene = Scene::load("res/demo.scene");

    // Get the box model and initialize its material parameter values and bindings
	Node* boxNode = _scene->findNode("box");
	

	boxNode->setTranslationX(100.5);
	boxNode->setTranslationY(2.3);
	boxNode->setTranslationZ(7.2);


	Model* boxModel = dynamic_cast<Model*>(boxNode->getDrawable());
	Material* boxMaterial = boxModel->getMaterial();
	Mesh* boxMesh = boxModel->getMesh();
	//boxNode->setDrawable(false);
	
	VertexFormat::Element elements[] = {
		VertexFormat::Element(VertexFormat::POSITION, 3),
		VertexFormat::Element(VertexFormat::NORMAL, 3),
		VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
	};
	int numVertecies = 3;

	Node* MeshNode = Node::create("MeshNode");
	Vector3 one(1, 0, 0);
	Vector3 two(1, 3, 0);
	Vector3 three(0, 0, 0);
	Vector3 four(0, 3, 0);
	
	
	Vertex* triangel = new Vertex();
	Vertex temp1;
	temp1.pos = Vector3(1, 0, 0);
	temp1.norm = Vector3(0, 1, 0);
	temp1.UV = Vector2(0, 0);

	triangel[0] = temp1;

	Vertex temp2;
	temp2.pos = Vector3(1, 3, 0);
	temp2.norm = Vector3(0, 1, 0);
	temp2.UV = Vector2(0, 0);

	triangel[1] = temp2;

	Vertex temp3;
	temp3.pos = Vector3(0, 0, 0);
	temp3.norm = Vector3(0, 1, 0);
	temp3.UV = Vector2(0, 0);
	
	triangel[2] = temp3;

	
		
	
	const VertexFormat vertFormat(elements, 3);
	int vertCount = boxMesh->getVertexCount();

	//Mesh* Quid = Mesh::createQuad(one,two,three,four); //This is the quad!
	Mesh* Quid = Mesh::createMesh(vertFormat, numVertecies,false);
	Quid->setVertexData(triangel);

	/*float verteciesData[34];
	float Positions[9] = { 1, 0, 0 , 1, 3, 0 ,  0, 0, 0};
	float Normals[12] = {0,1,0 , 0,1,0 , 0,1,0};
	float UVs[8] = {0,0 ,1,1 ,0,0};
*/
	

	//for (size_t i = 0; i <3; i++)
	//{
	//	size_t VerticeDataOffset = i * 8;
	//	size_t OthersOffset = i * 3;


	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		verteciesData[VerticeDataOffset + j] = Positions[(OthersOffset + j)];
	//	}
	//	VerticeDataOffset += 3;

	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		verteciesData[VerticeDataOffset + j] = Normals[(OthersOffset + j)];
	//	}
	//	VerticeDataOffset += 3;

	//	for (size_t j = 0; j < 2; j++)
	//	{
	//		verteciesData[VerticeDataOffset + j] = UVs[(OthersOffset + j)];
	//	}
	//}
	//
	



	Model* triModel = Model::create(Quid);
	MeshNode->setDrawable(triModel);
	_scene->addNode(MeshNode);
	triModel->setMaterial(boxMaterial);


	//CAMERA
	Node* cameraNode = getManualCamera();
	_scene->addNode(cameraNode);
	Camera* cam = cameraNode->getCamera();


	_scene->setActiveCamera(cam);
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio()); // Set the aspect ratio for the scene's camera to match the current resolution
	
}

void CustomRenderer::finalize()
{
    SAFE_RELEASE(_scene);
}

void CustomRenderer::update(float elapsedTime)
{
    // Rotate model
   // _scene->findNode("MeshNode")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
}

void CustomRenderer::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &CustomRenderer::drawScene);
}

bool CustomRenderer::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable(); 
    if (drawable)
        drawable->draw(_wireframe);

    return true;
}

void CustomRenderer::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void CustomRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _wireframe = !_wireframe;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
