#include "CustomRenderer.h"
#include "TempFunctions.cpp"
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
	Model* boxModel = dynamic_cast<Model*>(boxNode->getDrawable());
	Material* boxMaterial = boxModel->getMaterial();
	//boxNode->setDrawable(false);
	
	VertexFormat::Element elements[] = {
		VertexFormat::Element(VertexFormat::POSITION, 3),
		VertexFormat::Element(VertexFormat::NORMAL, 3),
		VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
	};

	Node* MeshNode = Node::create("MeshNode");
	Vector3 one(1, 0, 0);
	Vector3 two(1, 3, 0);
	Vector3 three(0, 0, 0);
	Vector3 four(0, 3, 0);
	

	//Mesh* Quid = Mesh::createQuad(one,two,three,four); //This is the quad!
	const VertexFormat vertFormat(elements, 3);

	Mesh* Quid = Mesh::createMesh(vertFormat, 6,false);
	Mesh* Temp = boxModel->getMesh();
	int vertCount = Temp->getVertexCount();
	//Temp->getv

	//Quid->setVertexData((float*)verteciesData, 0);


	

	Model* triModel = Model::create(Temp);

	MeshNode->setDrawable(triModel);
	
	_scene->addNode(MeshNode);

	triModel->setMaterial(boxMaterial);



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
