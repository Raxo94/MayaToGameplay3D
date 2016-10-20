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
	//AllocConsole();
    _scene = Scene::load("res/demo.scene");  // Load game scene from file

    // Get the box model and initialize its material parameter values and bindings
	Node* boxNode = _scene->findNode("box");
	boxNode->setTranslationX(-3.5);
	boxNode->setTranslationY(2.3);
	boxNode->setTranslationZ(0.2);
	

	//Light

	//Node* lightNode = Node::create("pointLightShape1");
	//Light* light = Light::createPoint(Vector3(0.5f, 0.5f, 0.5f), 20);
	//lightNode->setLight(light);
	//lightNode->translate(Vector3(0, 0, 0));
	//_scene->addNode(lightNode);
	//lightNode->release();
	//light->release();

	//Directional light

	Light* light = Light::createDirectional(0.75f, 0.75f, 0.75f);
	Node* lightNode = _scene->addNode("light");
	lightNode->setLight(light);
	SAFE_RELEASE(light); 	// Release the light because the node now holds a reference to it.



	//Mesh
	Node* meshNode = Node::create("MeshNode");
	Model* model = createCubeMesh();


	meshNode->setDrawable(model);
	_scene->addNode(meshNode);
	
	//Material
	Material* material(createDefaultMaterial());
	model->setMaterial(material);
	
	//CAMERA
	Node* cameraNode = getManualCamera();
	_scene->addNode(cameraNode);
	Camera* cam = cameraNode->getCamera();


	_scene->setActiveCamera(cam);
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio()); // Set the aspect ratio for the scene's camera to match the current resolution


	//Buffer
	mayaData = new MayaData();
	
	
}

void CustomRenderer::finalize()
{
    SAFE_RELEASE(_scene);
}

void CustomRenderer::update(float elapsedTime)
{


	
    
	if (mayaData->read());
	{
		//std::vector<Vertex> vertexVector;

	//	Vertex* vertexArray = (Vertex*)message;

	//	int vertexCount = 36;

	//	for ( size_t i = 0; i < vertexCount; i++)
	//	{
	//		vertexVector.push_back(vertexArray[i]);
	//	}
	//	
	//	

	//	//Node* meshNode1 = Node::create("MeshNode1");
	//	//Model* model1 = createDynamicMesh(vertexArray, vertexCount);
	//	meshNode1->setDrawable(model1);
	//	_scene->addNode(meshNode1);
	//	model1->setMaterial(tempMaterial);
	//	_scene->removeNode(_scene->findNode("MeshNode"));
	//	//Gör ett material.
	//	//Testa att ta in med circle buffern.
	//}
		;
	}

   _scene->findNode("MeshNode")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
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
