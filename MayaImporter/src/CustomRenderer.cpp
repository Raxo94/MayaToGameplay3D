#include "CustomRenderer.h"
#include "TempFunctions.cpp"

// Declare our game instance
CustomRenderer game;



CustomRenderer::CustomRenderer()
    : _scene(NULL), _wireframe(false)
{
}

CustomRenderer::~CustomRenderer()
{
	delete mayaData;
}

void CustomRenderer::initialize()
{
	//AllocConsole();
   // _scene = Scene::load("res/demo.scene");  // Load game scene from file
	_scene = Scene::create();
	setVsync(false);
    // Get the box model and initialize its material parameter values and bindings
	//Node* boxNode = _scene->findNode("box");

	

	//Light

	Light* light = Light::createDirectional(0.75f, 0.75f, 0.75f);
	Node* lightNode = _scene->addNode("DirectionalLightShape1");
	lightNode->setLight(light);
	SAFE_RELEASE(light); 	// Release the light because the node now holds a reference to it.
	
	Node* lightNode2 = Node::create("pointLightShape1");
	Light* light2 = Light::createPoint(Vector3(1.0f, 0.7f, 0.5f), 100);
	lightNode2->setLight(light2);
	lightNode2->translate(Vector3(0, 0, 0));
	_scene->addNode(lightNode2);
	lightNode2->release();
	light2->release();

	//CAMERA
	Node* cameraNode = getManualCamera();
	_scene->addNode(cameraNode);
	Camera* cam = cameraNode->getCamera();
	_scene->setActiveCamera(cam);
	cam->release();

	//MESH
	Node* meshNode = Node::create("Mesh1");
	Model* model = createCubeMesh();
	_scene->addNode(meshNode);
	model->setMaterial(createDefaultMaterial(_scene));
	meshNode->setDrawable(model);
	model->release();

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
			
			
			Model* model = createMayaMesh(mayaData->mesh);


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
		else if (mayaData->messageType == MessageType::MayaTransform)
		{
			Node* meshNode = _scene->findNode(mayaData->transform->meshName);
			if (meshNode)
			{
				meshNode->setTranslationX(mayaData->transform->translation[0]);
				meshNode->setTranslationY(mayaData->transform->translation[1]);
				meshNode->setTranslationZ(mayaData->transform->translation[2]);

				Matrix roationMatrix;
				Quaternion Test(mayaData->transform->rotation[0], mayaData->transform->rotation[1], mayaData->transform->rotation[2], mayaData->transform->rotation[3]);
				Matrix::createRotation(Test, &roationMatrix);
				meshNode->setRotation(roationMatrix);

				meshNode->setScale(mayaData->transform->scale[0], mayaData->transform->scale[1], mayaData->transform->scale[2]);
				_scene->addNode(meshNode);
				
			}
		}
		else if (mayaData->messageType == MessageType::MayaMaterial)
		{
			mayaData->material;
			Node* meshNode = _scene->findNode("Mesh1");
			
			char * asd = "Mesh1";
			if (meshNode)
			{
				_scene->removeNode(meshNode);
			/*	Model* model = static_cast<Model*>(meshNode->getDrawable());
				model->setMaterial(createMayaMaterial(_scene, mayaData->material));
				meshNode->setDrawable(model);
				model->release();
				_scene->addNode(meshNode);*/

				Node* meshNode = Node::create("Mesh2");
				Model* model = createCubeMesh();
				_scene->addNode(meshNode);
				model->setMaterial(createMayaMaterial(_scene,mayaData->material));
				meshNode->setDrawable(model);
				model->release();
				
			}

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
