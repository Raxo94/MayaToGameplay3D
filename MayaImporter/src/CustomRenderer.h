#ifndef CustomRenderer_H_
#define CustomRenderer_H_

#include "gameplay.h"
#include "MayaData.h"
#include <vector>

using namespace gameplay;

/**
 * Main game class.
 */
class CustomRenderer: public Game
{
public:
	
    /**
     * Constructor.
     */
    CustomRenderer();
	~CustomRenderer();
    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Draws the scene each frame.
     */

	MayaData* mayaData;
	Material* tempMaterial;
    bool drawScene(Node* node);

    Scene* _scene;
    bool _wireframe;
};

#endif
