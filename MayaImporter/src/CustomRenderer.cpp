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
	_scene = Scene::create();
	setVsync(false);
 
	//Light
	Node* lightNode2 = Node::create("pointLightShape1");
	Light* light2 = Light::createPoint(Vector3(1.0f, 1.0f, 1.0f), 100);
	lightNode2->setLight(light2);
	lightNode2->translate(Vector3(-10, 20, 10));
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

			Node* meshNode = _scene->findNode(mayaData->mesh->Name); // check if the mesh already exists
			Model* model;
			if (meshNode)
			{
				Model* oldModel = static_cast<Model*>(meshNode->getDrawable()); //use the old model to get the old material
				
				model = createMayaMesh(mayaData->mesh); // create the new mesh
				model->setMaterial(oldModel->getMaterial()); //we set the old material
				_scene->removeNode(_scene->findNode(mayaData->mesh->Name)); //remove the the old model
				
				_scene->addNode(meshNode);
				meshNode->setDrawable(model);
				model->release();

			}
			else
			{
				meshNode = Node::create(mayaData->mesh->Name); //make a new node
				Model* model = createMayaMesh(mayaData->mesh); // create the new mesh
				model->setMaterial(createDefaultMaterial(_scene)); 
				
				_scene->addNode(meshNode);
				meshNode->setDrawable(model);
				model->release();
			}	
		} //END OF MESH 


		else if (mayaData->messageType == MessageType::MayaCamera)
		{
			Node* camera = createMayaCamera(mayaData->cam); //createGameplayCamera from MayaCamera

			_scene->addNode(camera); 
			Camera* cam = camera->getCamera();
			_scene->setActiveCamera(cam); //Set newCamera as the currentCamera
			cam->release();
		}//END OF CAMERA 


		else if (mayaData->messageType == MessageType::MayaTransform)
		{
			Node* meshNode = _scene->findNode(mayaData->transform->meshName); //check if the transform's mesh exists in gameplay
			if (meshNode) //only then do we act
			{
				_scene->removeNode(meshNode); //Make node inactive
				meshNode->setTranslationX(mayaData->transform->translation[0]);
				meshNode->setTranslationY(mayaData->transform->translation[1]);
				meshNode->setTranslationZ(mayaData->transform->translation[2]);

				Matrix roationMatrix;
				Quaternion Test(mayaData->transform->rotation[0], mayaData->transform->rotation[1], mayaData->transform->rotation[2], mayaData->transform->rotation[3]);
				Matrix::createRotation(Test, &roationMatrix);
				meshNode->setRotation(roationMatrix);

				meshNode->setScale(mayaData->transform->scale[0], mayaData->transform->scale[1], mayaData->transform->scale[2]);
				_scene->addNode(meshNode); //re-activate Node

			}
		} //END OF TRANSFORM


		else if (mayaData->messageType == MessageType::MayaMaterial)
		{
			for (size_t i = 0; i < mayaData->material->amountOfMeshes; i++) //for all meshes bound to material
			{
				Node* meshNode = _scene->findNode(mayaData->meshName[i].meshName); //se if mesh exists in gameplay
				if (meshNode)
				{

					_scene->removeNode(meshNode); //Make node inactive

					Model* model = static_cast<Model*>(meshNode->getDrawable()); //get the model 

					model->setMaterial(createMayaMaterial(_scene, mayaData->material)); //replace the material
					meshNode->setDrawable(model);
					_scene->addNode(meshNode); //re-activate Node

				}

			}

		}
	
	}//END OF MATERIAL 
   
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
