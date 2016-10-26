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
	//AllocConsole();
    _scene = Scene::load("res/demo.scene");  // Load game scene from file

    // Get the box model and initialize its material parameter values and bindings
	Node* boxNode = _scene->findNode("box");
	boxNode->setTranslationX(-3.5);
	boxNode->setTranslationY(2.3);
	boxNode->setTranslationZ(0.2);
	

	//Light

	Light* light = Light::createDirectional(0.75f, 0.75f, 0.75f);
	Node* lightNode = _scene->addNode("light");
	lightNode->setLight(light);
	SAFE_RELEASE(light); 	// Release the light because the node now holds a reference to it.


	//Mesh
	/*Node* meshNode = Node::create("MeshNode");
	Model* model = createCubeMesh();
	meshNode->setDrawable(model);
	_scene->addNode(meshNode);*/
	
	
	//Material
	/*Material* material(createDefaultMaterial(_scene));
	model->setMaterial(material);
	material->release();*/
	
	
	
	//CAMERA
	Node* cameraNode = getManualCamera();
	_scene->addNode(cameraNode);
	Camera* cam = cameraNode->getCamera();
	_scene->setActiveCamera(cam);
	//_scene->getActiveCamera()->setAspectRatio(getAspectRatio()); // Set the aspect ratio for the scene's camera to match the current resolution
	cam->release();


	//Buffer
	mayaData = new MayaData();
	
	
}

void CustomRenderer::finalize()
{
    SAFE_RELEASE(_scene);
}

void CustomRenderer::update(float elapsedTime)
{

	if (mayaData->read())
	{
		if (mayaData->messageType == MessageType::MayaMesh)
		{

			Node* meshNode = _scene->findNode(mayaData->mesh->Name);

			if (meshNode)
				_scene->removeNode(_scene->findNode(mayaData->mesh->Name));

			else
				meshNode = Node::create(mayaData->mesh->Name);
			
			
			Model* model = createDynamicMesh(mayaData->mesh);

			meshNode->setTranslationX(0); meshNode->setTranslationY(1); meshNode->setTranslationZ(0);

			_scene->addNode(meshNode);
			model->setMaterial(createDefaultMaterial(_scene));
			meshNode->setDrawable(model);
			model->release();
			
		}
		else if (mayaData->messageType == MessageType::MayaCamera)
		{
			Node* camera = createMayaCamera(mayaData->cam);
			
			_scene->addNode(camera);
			Camera* cam = camera->getCamera();
			_scene->setActiveCamera(cam);
			cam->release();
		}
	
	}
   
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
