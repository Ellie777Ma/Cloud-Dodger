#include "engine.h"
#include <iostream>

//Background colors
const color nightSky(35/255.0, 35/255.0, 120/255.0);
const color concrete(60/255.0, 60/255.0, 60/255.0);
const color darkGreen(25/255.0, 85/255.0, 50/255.0);
const color white(1, 1, 1);

// Building colors
const color smallBuilding(225/255.0, 200/255.0, 255/255.0);
const color mediumBuilding(122/255.0, 191/255.0, 255/255.0);
const color largeBuilding(71/255.0, 71/255.0, 255/255.0);

//  Hover colors
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Set uniforms that never change
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // Initialize the user to be a 10x10 white block
    // centered at (0, 0)
    user = make_unique<Rect>(shapeShader, vec2(width/4, height/2), vec2(10, 10), white); // placeholder for compilation

    // Init grass
    grass = make_unique<Rect>(shapeShader, vec2(width/2, 50), vec2(width, height / 3), concrete);

    // Init moon
    moon.push_back(make_unique<Circle>(shapeShader, vec2(width, height), vec2(20, 20), color(white)));

    // Init mountains
    mountains.push_back(make_unique<Triangle>(shapeShader, vec2(width/4, 300), vec2(width, 400), darkGreen));
    mountains.push_back(make_unique<Triangle>(shapeShader, vec2(2*width/3, 300), vec2(width, 500), darkGreen));

    // Init buildings from closest to furthest
    int totalBuildingWidth = 0;
    vec2 buildingSize;
    while (totalBuildingWidth < width + 50) {
        // Building height between 50-100
        buildingSize.y = rand() % 101 + 150;
        // Building width between 30-50
        buildingSize.x = rand() % 21 + 30;
        buildings1.push_back(make_unique<Rect>(shapeShader,
                                               vec2(totalBuildingWidth + (buildingSize.x / 2.0) + 5,
                                                    ((buildingSize.y / 2.0) + 50)),
                                               buildingSize, smallBuilding));
        totalBuildingWidth += buildingSize.x + 5;
    }
    // Populate second set of buildings
    totalBuildingWidth = 0;
    while (totalBuildingWidth < width + 100) {
        // Populate this vector of mediumBuilding buildings
        // Building height between 100-200
        buildingSize.y = rand() % 151 + 200;
        // Building width between 50-100
        buildingSize.x = rand() % 51 + 50;
        buildings2.push_back(make_unique<Rect>(shapeShader,
                                                 vec2(totalBuildingWidth + (buildingSize.x / 2.0) + 5,
                                                      ((buildingSize.y / 2.0) + 50)),
                                                 buildingSize, mediumBuilding));
        totalBuildingWidth += buildingSize.x + 5;
    }
    // Populate third set of buildings
    totalBuildingWidth = 0;
    while (totalBuildingWidth < width + 200) {
        // Populate this vector of largeBuilding buildings
        // Building height between 200-400
        buildingSize.y = rand() % 201 + 250;
        // Building width between 100-200
        buildingSize.x = rand() % 101 + 200;
        buildings3.push_back(make_unique<Rect>(shapeShader,
                                                 vec2(totalBuildingWidth + (buildingSize.x / 2.0) + 5,
                                                      ((buildingSize.y / 2.0) + 50)),
                                                 buildingSize, largeBuilding));
        totalBuildingWidth += buildingSize.x + 5;
    }

    // Init cloud obstacles
    clouds.push_back(Cloud(shapeShader, vec2(450, 200)));
    clouds.push_back(Cloud(shapeShader, vec2(450, 450)));
    clouds.push_back(Cloud(shapeShader, vec2(450, 550)));
    clouds.push_back(Cloud(shapeShader, vec2(600, 250)));
    clouds.push_back(Cloud(shapeShader, vec2(600, 350)));
    clouds.push_back(Cloud(shapeShader, vec2(600, 500)));
    clouds.push_back(Cloud(shapeShader, vec2(750, 150)));
    clouds.push_back(Cloud(shapeShader, vec2(750, 400)));
    clouds.push_back(Cloud(shapeShader, vec2(750, 550)));
    clouds.push_back(Cloud(shapeShader, vec2(900, 200)));
    clouds.push_back(Cloud(shapeShader, vec2(900, 350)));
    clouds.push_back(Cloud(shapeShader, vec2(900, 450)));
    clouds.push_back(Cloud(shapeShader, vec2(1150, 150)));
    clouds.push_back(Cloud(shapeShader, vec2(1150, 300)));
    clouds.push_back(Cloud(shapeShader, vec2(1150, 500)));
    clouds.push_back(Cloud(shapeShader, vec2(1300, 250)));
    clouds.push_back(Cloud(shapeShader, vec2(1300, 450)));
    clouds.push_back(Cloud(shapeShader, vec2(1300, 550)));
}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Update mouse rect to follow mouse
    MouseY = height - MouseY; // make sure mouse y-axis isn't flipped

    // Make the user move with the mouse's y position
    user->setPosY(MouseY);

    // Small buildings turn orange when overlapping
    for (const unique_ptr<Rect>& r : buildings1) {
        if (r->isOverlapping(*user)) {
            r->setColor(orange);
        } else {
            r->setColor(smallBuilding);
        }
    }

    // Medium buildings turn cyan when overlapping
    for (const unique_ptr<Rect>& r : buildings2) {
        if (r->isOverlapping(*user)) {
            r->setColor(cyan);
        } else {
            r->setColor(mediumBuilding);
        }
    }

    // Large buildings turn magenta when overlapping
    for (const unique_ptr<Rect>& r : buildings3) {
        if (r->isOverlapping(*user)) {
            r->setColor(magenta);
        } else {
            r->setColor(largeBuilding);
        }
    }

    // when the user overlaps with the clouds.
    for (const Cloud& c : clouds) {
        if (c.isOverlapping(*user)) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Update clouds
    for (Cloud& c : clouds) {
        c.moveXWithinBounds(-1, width);
    }

    // Update small buildings
    for (int i = 0; i < buildings1.size(); ++i) {
        // Move all the red buildings to the left
        buildings1[i]->moveX(-1.5);
        // If a building has moved off the screen
        if (buildings1[i]->getPosX() < -(buildings1[i]->getSize().x/2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (buildings1[i] == buildings1[0]) ? buildings1.size()-1 : i - 1;
            buildings1[i]->setPosX(buildings1[buildingOnLeft]->getPosX() + buildings1[buildingOnLeft]->getSize().x/2 + buildings1[i]->getSize().x/2 + 5);
        }
    }

    // Update medium buildings
    for (int i = 0; i < buildings2.size(); ++i) {
        buildings2[i]->moveX(-0.75); // Move slower
        if (buildings2[i]->getPosX() < -(buildings2[i]->getSize().x/2)) {
            int buildingOnLeft = (buildings2[i] == buildings2[0]) ? buildings2.size()-1 : i - 1;
            buildings2[i]->setPosX(buildings2[buildingOnLeft]->getPosX() + buildings2[buildingOnLeft]->getSize().x/2 + buildings2[i]->getSize().x/2 + 5);
        }
    }

    // Update large buildings
    for (int i = 0; i < buildings3.size(); ++i) {
        buildings3[i]->moveX(-0.375); // Move even slower
        if (buildings3[i]->getPosX() < -(buildings3[i]->getSize().x/2)) {
            int buildingOnLeft = (buildings3[i] == buildings3[0]) ? buildings3.size()-1 : i - 1;
            buildings3[i]->setPosX(buildings3[buildingOnLeft]->getPosX() + buildings3[buildingOnLeft]->getSize().x/2 + buildings3[i]->getSize().x/2 + 5);
        }
    }
}

void Engine::render() {
    glClearColor(nightSky.red,nightSky.green, nightSky.blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const unique_ptr<Triangle>& m : mountains) {
        m->setUniforms();
        m->draw();
    }

    for (const unique_ptr<Circle>& mo : moon) {
        mo->setUniforms();
        mo->draw();
    }

    grass->setUniforms();
    grass->draw();

    // Add logic to draw the user and the buildings.
    for (const unique_ptr<Rect>& b : buildings3) {
        b->setUniforms();
        b->draw();
    }
    for (const unique_ptr<Rect>& b : buildings2) {
        b->setUniforms();
        b->draw();
    }
    for (const unique_ptr<Rect>& b : buildings1) {
        b->setUniforms();
        b->draw();
    }

    for (Cloud& c : clouds) {
        c.setUniformsAndDraw();
    }

    user->setUniforms();
    user->draw();

    glfwSwapBuffers(window);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}