// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "common/controls.hpp"

// in order to use stb_image.h and import a png texture image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Plane.h"
#include "GLTexture.h"
#include "Texture.h"
#include "Camera.h"
#include "SceneObject.h"
#include "MeshObject.h"
#include "SceneGraph.h"


void key (GLFWwindow *window, int key, int scancode, int action, int mods );

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera *camera = new Camera();
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);
float angle_in_degrees = 1.;
bool cameraRotates = false;
float cameraSpeed;
bool speedUp = false;
bool slowDown = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// scene objects
std::vector<SceneObject*> scene_objects;

//rotation
float angle = 0.;
float zoom = 1.;

// plane data
float plane_len = 3.0;
int plane_dim = 50;
Plane *plane = new Plane(plane_len, plane_len, plane_dim, plane_dim);

// height map and textures
Texture *height_map = new Texture();
GLTexture * grass_texture = new GLTexture();
GLTexture * rock_texture = new GLTexture();
GLTexture * snowrocks_texture = new GLTexture();

GLuint programID;
/*******************************************************************************/


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);

    // key
    glfwSetKeyCallback(window, key);
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    // ------------------------------------------------------------------------------------
    // SUZANNE OBJECT
    // ------------------------------------------------------------------------------------
    // Load mesh file
    MeshObject *suzanne_mesh = new MeshObject();
    std::string filename_suz("data_off/suzanne.off");
    suzanne_mesh->create(filename_suz);
    suzanne_mesh->generateBuffers();
    scene_objects.push_back(suzanne_mesh);

    // create scene graph
    SceneGraph *root = new SceneGraph(*suzanne_mesh);
    root->setLevel(0);

    MeshObject *sphere = new MeshObject();
    std::string filename_sphere("data_off/sphere.off");
    sphere->create(filename_sphere);
    sphere->setColor(glm::vec4(1.0,0.0,0.0,0.0));
    sphere->generateBuffers();
    scene_objects.push_back(sphere);
    SceneGraph node1 = root->addChild(*new SceneGraph(*sphere));

    MeshObject *sphere2 = new MeshObject();
    sphere2->create(filename_sphere);
    sphere2->setColor(glm::vec4(0.0,0.0,1.0,0.0));
    sphere2->generateBuffers();
    scene_objects.push_back(sphere2);
    SceneGraph node2 = node1.addChild(*new SceneGraph(*sphere2));
    // ------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------
    // GENERATE TERRAIN
    // ------------------------------------------------------------------------------------
    // generate plane -> fill arrays of indices, triangles and indexed_vertices
    /*plane->setIsTerrain(1);
    plane->generateBuffers();
    scene_objects.push_back(plane);

    // use height map
    height_map->readPGMTexture((char*)"textures/Heightmap_Mountain128.pgm");
    plane->generatePlane('y');
    plane->addHeightMap(height_map->data, height_map->height, height_map->width,'y');

    // add textures
    grass_texture->generateBuffer();
    grass_texture->fillBuffer(plane->coord_texture);
    grass_texture->generateTexture();
    grass_texture->loadTexture((char*)"textures/grass.png");
    grass_texture->defineParameters();

    rock_texture->generateBuffer();
    rock_texture->fillBuffer(plane->coord_texture);
    rock_texture->generateTexture();
    rock_texture->loadTexture((char*)"textures/rock.png");
    rock_texture->defineParameters();

    snowrocks_texture->generateBuffer();
    snowrocks_texture->fillBuffer(plane->coord_texture);
    snowrocks_texture->generateTexture();
    snowrocks_texture->loadTexture((char*)"textures/snowrocks.png");
    snowrocks_texture->defineParameters();*/
    // ------------------------------------------------------------------------------------

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{
        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // CAMERA
        camera->MVP(cameraRotates, speedUp, slowDown);
        speedUp = false;
        slowDown = false;
        camera->sendMVPtoShader(programID);

        // texture
        grass_texture->sendTextureToShader(programID, "texture_grass", 0);
        rock_texture->sendTextureToShader(programID, "texture_rock", 1);
        snowrocks_texture->sendTextureToShader(programID, "texture_snowrocks", 2);

        // Draw the triangles !
        for(SceneObject *obj: scene_objects){
            obj->loadBuffers();
            obj->draw(programID);
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    for(SceneObject *obj: scene_objects){
        obj->deleteBuffers();
    }
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void key (GLFWwindow *window, int key, int scancode, int action, int mods ) {

    if( key == GLFW_KEY_EQUAL and action == GLFW_PRESS ){ // minus on macbook keyboard

        std::cout << "You have pressed the key - : resolution decreases" << std::endl;

        /// DECREASE RESOLUTION

        if(plane_dim > 13){
            plane_dim -= 5;
        }else{
            plane_dim = 8;
        }

    }
    else if( key == GLFW_KEY_SLASH and action == GLFW_PRESS ){ // plus on macbook keyboard

        std::cout << "You have pressed the key + : resolution increases" << std::endl;

        /// INCREASE RESOLUTION

        if(plane_dim < 196){
            plane_dim += 5;
        }else{
            plane_dim = 200;
        }


    }else if( key == GLFW_KEY_C and action == GLFW_PRESS ){

        std::cout << "You have pressed the key c : switch to orbital camera" << std::endl;

        /// turn around axis

        if(cameraRotates){
            cameraRotates = false;
            setCamPosition(glm::vec3( 0, 0, 5));
            setVerticalAngle(0.0f);
        }else{
            cameraRotates = true;
            setCamPosition(glm::vec3( 0, 5, 5));
            setVerticalAngle(-3.14f/4.0f);
        }


    }else if( key == GLFW_KEY_W and action == GLFW_PRESS ){ // Z on macbook keyboard

        std::cout << "You have pressed the key Z : rotation speeds up" << std::endl;

        /// accelerates camera
        speedUp = true;

    }else if ( key == GLFW_KEY_Z and action == GLFW_PRESS ){ // W on macbook keyboard

        std::cout << "You have pressed the key W : rotation slows down" << std::endl;

        /// slows down camera
        slowDown = true;

    }

    if( (key == GLFW_KEY_SLASH or key == GLFW_KEY_EQUAL) and action == GLFW_PRESS){

        // EDIT PLANE
        plane->setDimension(plane_dim, plane_dim);
        plane->clearVectors();
        plane->generatePlane('y');
        plane->addHeightMap(height_map->data, height_map->height, height_map->width,'y');

        // UPDATE BUFFERS
        plane->loadBuffers();

        grass_texture->fillBuffer(plane->coord_texture);
        grass_texture->sendTextureToShader(programID, "texture_grass", 0);
        rock_texture->fillBuffer(plane->coord_texture);
        rock_texture->sendTextureToShader(programID, "texture_rock", 0);
        snowrocks_texture->fillBuffer(plane->coord_texture);
        snowrocks_texture->sendTextureToShader(programID, "texture_snowrocks", 0);
    }

}
