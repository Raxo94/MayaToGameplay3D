#include "CustomRenderer.h"
#include "TempFunctions.cpp"
#include <Vector>
// Declare our game instance
CustomRenderer game;



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
	
	//AllocConsole();

	boxNode->setTranslationX(-3.5);
	boxNode->setTranslationY(2.3);
	boxNode->setTranslationZ(0.2);

	//MESH
	Model* boxModel = dynamic_cast<Model*>(boxNode->getDrawable());
	this->tempMaterial = boxModel->getMaterial();

	
	Node* meshNode = Node::create("MeshNode");
	Model* model = createCubeMesh();
	meshNode->setDrawable(model);
	_scene->addNode(meshNode);
	model->setMaterial(tempMaterial);


	//CAMERA
	Node* cameraNode = getManualCamera();
	_scene->addNode(cameraNode);
	Camera* cam = cameraNode->getCamera();


	_scene->setActiveCamera(cam);
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio()); // Set the aspect ratio for the scene's camera to match the current resolution


	//Buffer
	this-> message = new char[1 << 20 / 4];
	this->circBuffer = new CircBufferFixed(L"buff", false, 1 << 20, 256);
	
}

void CustomRenderer::finalize()
{
    SAFE_RELEASE(_scene);
}

void CustomRenderer::update(float elapsedTime)
{


	
    // Rotate model
	if(circBuffer->pop(message))
	{
		std::vector<Vertex> vertexVector;

		Vertex* vertexArray = (Vertex*)message;

		int vertexCount = 36;

		for ( size_t i = 0; i < vertexCount; i++)
		{
			vertexVector.push_back(vertexArray[i]);
		}
		
		

		Node* meshNode1 = Node::create("MeshNode1");
		Model* model1 = createDynamicMesh(vertexArray, vertexCount);
		meshNode1->setDrawable(model1);
		_scene->addNode(meshNode1);
		model1->setMaterial(tempMaterial);
		//_scene->removeNode(_scene->findNode("MeshNode"));
		
	}


   //_scene->findNode("MeshNode")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
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
