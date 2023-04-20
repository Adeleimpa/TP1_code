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
#include "Transform.h"
#include "SolarSystem.h"
#include "Sphere.h"


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
Transform transformer; // to transform scene objects one by one

//rotation
float angle = 0.;
float zoom = 1.;

bool heightmap_activated = false;

// plane data
float plane_len =  3.8;
int plane_dim = 40;
Plane *plane = new Plane(plane_len, plane_len, plane_dim, plane_dim);

// sphere data
Sphere *sphere = new Sphere();
double initial_speed = 1.5;

SceneGraph *root = new SceneGraph();

// height map and textures
Texture *height_map = new Texture();
GLTexture *grass_texture = new GLTexture();
GLTexture *rock_texture = new GLTexture();
GLTexture *snowrocks_texture = new GLTexture();
GLTexture *sun_texture = new GLTexture();

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
    // SOLAR SYSTEM (TP3)
    // ------------------------------------------------------------------------------------
    /*SceneGraph *root = new SceneGraph();
    SolarSystem *solarSystem = new SolarSystem();
    solarSystem->createSolarSystem(root);
    cameraRotates = true;*/
    // -----------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------
    // GENERATE TERRAIN (TP1 & 2)
    // ------------------------------------------------------------------------------------
    // generate plane -> fill arrays of indices, triangles and indexed_vertices
    plane->generatePlane();
    plane->setIsTerrain(1);
    plane->generateBuffers();
    //plane->addRelief();

    // use height map
    if(heightmap_activated){
        height_map->readPGMTexture((char*)"textures/Heightmap_Mountain512.pgm");
        plane->addHeightMap(height_map->data, height_map->height, height_map->width);
    }

    plane->setColor(glm::vec4(0.2, 0.8, 0.05, 0.0));
    scene_objects.push_back(plane);
    // ------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------
    // SPHERE OBJECT (TP4)
    // -----------------------------------------------------------------------------------
    sphere->m_radius =  0.1f;//0.05f;
    sphere->m_center = glm::vec3(plane->center[0], 0.0, plane->center[2]);
    double height_sphere = 0.0;
    if(heightmap_activated){
        height_sphere = plane->getHeightFromCoords(height_map->data, height_map->height, height_map->width, sphere->m_center);
    }
    sphere->m_center[1] = height_sphere + sphere->m_radius;
    //sphere->m_center = center_sphere;
    sphere->build_arrays();
    sphere->build_arrays_for_resolutions();
    sphere->setColor(glm::vec4(1.0,0.0,0.0,0.0));
    sphere->generateBuffers();

    sphere->transformations.push_back(glm::vec3(0.0,0.0,0.0));
    sphere->index_transf.push_back(1);

    scene_objects.push_back(sphere);
    // -----------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------
    // SCENE GRAPH
    // ------------------------------------------------------------------------------------
    //SceneGraph *root = new SceneGraph();
    root->setData(plane);
    root->setLevel(0);
    SceneGraph *node_child = root->addChild(new SceneGraph(sphere));
    // ------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------
    // add textures
    // ------------------------------------------------------------------------------------
    sun_texture->generateTexture();
    sun_texture->loadTexture((char*)"textures/sun.jpg");
    sun_texture->defineParameters();

    grass_texture->generateTexture();
    grass_texture->loadTexture((char*)"textures/grass.png");
    grass_texture->defineParameters();

    rock_texture->generateTexture();
    rock_texture->loadTexture((char*)"textures/rock.png");
    rock_texture->defineParameters();

    snowrocks_texture->generateTexture();
    snowrocks_texture->loadTexture((char*)"textures/snowrocks.png");
    snowrocks_texture->defineParameters();
    // ------------------------------------------------------------------------------------

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double counter_flying = 0.0;

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

        // flying sphere
        if(sphere->isFlying){
            sphere->fly(deltaTime);
            if(sphere->m_center[1]-sphere->m_radius < 0.00001){
                sphere->velocity = -sphere->velocity;
                sphere->velocity[0] = -sphere->velocity[0];
            }
            if(sphere->velocity[1] < 0.00001 and sphere->m_center[1]-sphere->m_radius < 0.00001){
                sphere->isFlying = false;
                sphere->velocity = glm::vec3(0.0,0.0,0.0);
                std::cout << "fly is over" << std::endl;
            }
        }

        // Draw the triangles !
        /*for(int i = 0; i < scene_objects.size(); i++){

            if(scene_objects[i]->isTerrain==0){ // terrain
                // send textures to shader
                grass_texture->sendTextureToShader(programID, "texture_grass", 0);
                rock_texture->sendTextureToShader(programID, "texture_rock", 1);
                snowrocks_texture->sendTextureToShader(programID, "texture_snowrocks", 2);
            }else{
                sun_texture->sendTextureToShader(programID, "texture_sun", 3);
            }

            scene_objects[i]->loadBuffers();
            scene_objects[i]->draw(programID);
        }*/

        // solar system
        transformer.updateGraph(*root, programID, camera, grass_texture, rock_texture, snowrocks_texture, sun_texture);

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

    double offset = 0.01;

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

        // TODO doesn't work when center of plane is not 0,0,0
        if(cameraRotates){
            cameraRotates = false;
            setCamPosition(glm::vec3( 0, 0.55, 5));
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

    }else if ( key == GLFW_KEY_Z and action == GLFW_PRESS ) { // W on macbook keyboard
        std::cout << "You have pressed the key W : rotation slows down" << std::endl;

        /// slows down camera
        slowDown = true;

    }

    // DISPLACE SPHERE USING T,F,V,G
    else if ( key == GLFW_KEY_T ){
        std::cout << "You have pressed the key T : sphere translation back" << std::endl;

        // if object doesn't go farther than terrain area
        if(sphere->m_center[2] - offset > plane->top_right[2] and sphere->m_center[2] - offset < plane->bottom_right[2]) {
            sphere->transformations[0][2] -= offset;
            sphere->m_center[2] -= offset;
        }


    }else if ( key == GLFW_KEY_V ){
        std::cout << "You have pressed the key V : sphere translation front" << std::endl;

        // if object doesn't go farther than terrain area
        if(sphere->m_center[2] + offset > plane->top_right[2] and sphere->m_center[2] + offset < plane->bottom_right[2]){
            sphere->transformations[0][2] += offset;
            sphere->m_center[2] += offset;
        }

    }else if ( key == GLFW_KEY_F ){
        std::cout << "You have pressed the key F : sphere translation left" << std::endl;

        // if object doesn't go farther than terrain area
        if(sphere->m_center[0] - offset > plane->top_right[2] and sphere->m_center[0] - offset < plane->bottom_right[2]){
            sphere->transformations[0][0] -= offset;
            sphere->m_center[0] -= offset;
        }

    }else if ( key == GLFW_KEY_G ){
        std::cout << "You have pressed the key G : sphere translation right" << std::endl;

        // if object doesn't go farther than terrain area
        if(sphere->m_center[0] + offset > plane->top_right[2] and sphere->m_center[0] + offset < plane->bottom_right[2]) {
            sphere->transformations[0][0] += offset;
            sphere->m_center[0] += offset;
        }
    }else if ( key == GLFW_KEY_SPACE and action == GLFW_PRESS ){
        sphere->isFlying = true;
        std::cout << "fly starts" << std::endl;
        sphere->velocity = glm::vec3(1.0,1.0,0.0) * glm::vec3(initial_speed,initial_speed,initial_speed);
    }

    if( key == GLFW_KEY_G or key == GLFW_KEY_F or key == GLFW_KEY_V or key == GLFW_KEY_T){
        // ----------------------------------------------------------------
        // follow height of terrain according to heightmap
        sphere->transformations[0][1] -= sphere->m_center[1];
        double height_sphere = 0.0;
        if(heightmap_activated){
            height_sphere = plane->getHeightFromCoords(height_map->data, height_map->height, height_map->width, sphere->m_center);
        }
        sphere->transformations[0][1] += height_sphere + sphere->m_radius;
        sphere->m_center[1] = height_sphere + sphere->m_radius;
        // ----------------------------------------------------------------

        // ----------------------------------------------------------------
        // check if object is too far from camera -> decrease resolution
        glm::vec3 camPos = getCamPosition();
        double distance_from_cam = camPos[2] - sphere->m_center[2];

        unsigned int new_resol;
        int reso;
        if(distance_from_cam > 6.0){ // back
            reso = 0;
            new_resol = sphere->back_resolution;
        }else if(distance_from_cam <= 6.0 and distance_from_cam > 4.0){ // middle
            reso = 1;
            new_resol = sphere->middle_resolution;
        }else{ // front
            reso = 2;
            new_resol = sphere->front_resolution;
        }

        // do it only if new resolution
        if(new_resol != sphere->resolution){
            sphere->setResolution(new_resol);
            sphere->clearVectors();
            sphere->switchResolution(reso);
        }
        // ----------------------------------------------------------------
    }

    if( (key == GLFW_KEY_SLASH or key == GLFW_KEY_EQUAL) and action == GLFW_PRESS){

        // EDIT PLANE
        plane->setDimension(plane_dim, plane_dim);
        plane->clearVectors();
        plane->generatePlane();
        plane->addHeightMap(height_map->data, height_map->height, height_map->width);
    }

}
