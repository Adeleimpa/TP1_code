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

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// MVP
glm::mat4 View_Matrix;
glm::mat4 Projection_Matrix;
glm::mat4 Model_Matrix;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 3.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;
/*******************************************************************************/

void generatePlane(int h, int w, std::vector<unsigned short> &indices, std::vector<std::vector<unsigned short> > &triangles, std::vector<glm::vec3> &indexed_vertices){
    double x_len = 0.6;
    double z_len = 0.6;
    glm::vec3 corner1(-x_len/2.0, 0.0, 0.0); // bottom left
    glm::vec3 corner2(x_len/2.0, 0.0, 0.0); // bottom right
    glm::vec3 corner3(-x_len/2.0, 0.0, -z_len); // top left
    glm::vec3 corner4(x_len/2.0, 0.0, -z_len); // top right
    double x_step = x_len/w;
    double z_step = z_len/h;

    glm::vec3 current_c1;
    glm::vec3 current_c2;
    glm::vec3 current_c3;
    glm::vec3 current_c4;

    int index = -1;

    for(int i = 0; i < w ; i++){
        for(int j = 0 ; j < h; j++){
            std::vector<unsigned short> triangle1;
            std::vector<unsigned short> triangle2;

            current_c1[0] = corner1[0] + i*x_step;
            current_c1[1] = corner1[1];
            current_c1[2] = corner1[2] + j*z_step;

            current_c2 = current_c1;
            current_c2[0] += (i+1)*x_step;

            current_c3 = current_c1;
            current_c3[2] += (j+1)*z_step;

            current_c4 = current_c1;
            current_c4[0] += (i+1)*x_step;
            current_c4[2] += (j+1)*z_step;

            indexed_vertices.push_back(current_c1);
            indexed_vertices.push_back(current_c2);
            indexed_vertices.push_back(current_c3);
            indexed_vertices.push_back(current_c4);

            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
            indices.push_back(index + 3);
            indices.push_back(index + 2);
            indices.push_back(index + 4);
            index+=4;

            triangle1.push_back(index + 1);
            triangle1.push_back(index + 2);
            triangle1.push_back(index + 3);
            triangle2.push_back(index + 3);
            triangle2.push_back(index + 2);
            triangle2.push_back(index + 4);

            triangles.push_back(triangle1);
            triangles.push_back(triangle2);
        }
    }
}

void generatePlaneXY(double x_len, double y_len, 
                    int h, int w, 
                    std::vector<unsigned short> &indices, 
                    std::vector<std::vector<unsigned short> > &triangles, 
                    std::vector<glm::vec3> &indexed_vertices, 
                    std::vector<glm::vec2> &coord_texture){

    glm::vec3 corner1(-x_len/2.0, -y_len/2.0, 0.0); // bottom left
    glm::vec3 corner2(x_len/2.0, -y_len/2.0, 0.0); // bottom right
    glm::vec3 corner3(-x_len/2.0, y_len/2.0, 0.0); // top left
    glm::vec3 corner4(x_len/2.0, y_len/2.0, 0.0); // top right
    double x_step = x_len/(double)h;
    double y_step = y_len/(double)w;

    glm::vec3 current_c1;
    glm::vec3 current_c2;
    glm::vec3 current_c3;
    glm::vec3 current_c4;

    int index = -1;

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            std::vector<unsigned short> triangle1;
            std::vector<unsigned short> triangle2;

            current_c1[0] = corner1[0] + i*x_step;
            current_c1[1] = corner1[1] + j*y_step;
            current_c1[2] = corner1[2];

            current_c2 = current_c1;
            current_c2[0] += x_step;

            current_c3 = current_c1;
            current_c3[1] += y_step;

            current_c4 = current_c1;
            current_c4[0] += x_step;
            current_c4[1] += y_step;

            indexed_vertices.push_back(current_c1);
            indexed_vertices.push_back(current_c2);
            indexed_vertices.push_back(current_c3);
            indexed_vertices.push_back(current_c4);

            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
            indices.push_back(index + 3);
            indices.push_back(index + 4);
            indices.push_back(index + 2);

            triangle1.push_back(index + 1);
            triangle1.push_back(index + 2);
            triangle1.push_back(index + 3);
            triangle2.push_back(index + 3);
            triangle2.push_back(index + 4);
            triangle2.push_back(index + 2);

            index+=4;

            triangles.push_back(triangle1);
            triangles.push_back(triangle2);

            // texture 
            /*coord_texture.push_back({i*x_step, j*y_step});
            coord_texture.push_back({i*x_step, j*y_step + y_step});
            coord_texture.push_back({i*x_step + x_step, j*y_step});
            coord_texture.push_back({i*x_step + x_step, j*y_step + y_step});*/
            coord_texture.push_back({current_c1[0]/x_len, current_c1[1]/y_len});
            coord_texture.push_back({current_c2[0]/x_len, current_c2[1]/y_len});
            coord_texture.push_back({current_c3[0]/x_len, current_c3[1]/y_len});
            coord_texture.push_back({current_c4[0]/x_len, current_c4[1]/y_len});
        }
    }
}

void addZRelief(std::vector<glm::vec3> &indexed_vertices){
    for(glm::vec3 vertex : indexed_vertices){

        double f = (double)rand() / RAND_MAX;
        double rand_z = -0.2 + f * (0.2 - (-0.2));

        vertex[2] = vertex[2] + rand_z;
    }
}

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
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> coord_texture; // texture

    // Loading mesh file
    //std::string filename("chair.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    // generate plane -> fill arrays of indices, triangles and indexed_vertices
    generatePlaneXY(1.6, 1.6, 16, 16, indices, triangles, indexed_vertices, coord_texture);
    //addZRelief(indexed_vertices); // TODO once camera is set

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // ---------------------------------------------------------------------------------------------------------
    // texture
    // ---------------------------------------------------------------------------------------------------------
    unsigned int texture;
    GLuint buffer_coord_txt;

    // on cree un buffer pour les textures
    glGenBuffers(1, &buffer_coord_txt);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
    glBufferData(GL_ARRAY_BUFFER, coord_texture.size() * sizeof(glm::vec2), &coord_texture[0], GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // on charge et on genere la texture a l'aide de l'image texture (.png)
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/carre.jpg", &width , &height , &nrChannels , 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE , data);
        glGenerateMipmap(GL_TEXTURE_2D); }
    else {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data);

    // definit les comportements de la texture qd elle est etiree ou reduite
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // ---------------------------------------------------------------------------------------------------------


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

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);


        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change
        //Model_Matrix = glm::mat4(1.0f); // specifies diagonal of 1's
        //computeMatricesFromInputs();

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        //View_Matrix = glm::lookAt(camera_position, camera_target + camera_position, camera_up);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        //Projection_Matrix =  glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms
        //glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, &Model_Matrix[0][0]); // location, count, transpose, value
        //glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, &View_Matrix[0][0]);
        //glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, &Projection_Matrix[0][0]);

        computeMatricesFromInputs();
        View_Matrix = getViewMatrix();
        Model_Matrix = glm::mat4(1.0f);
        Projection_Matrix = getProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, &Model_Matrix[0][0]); // location, count, transpose, value
        glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, &View_Matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, &Projection_Matrix[0][0]);


        /****************************************/


        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // texture
        glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void *) 0);
        int indexActiveTexture = 0;// 0 to 31
        glBindTexture(GL_TEXTURE_2D , texture); // bind texture as Texture 0
        glActiveTexture(GL_TEXTURE0 + indexActiveTexture);
        // set used active texture (Modern OpenGL)
        GLuint loc2 = glGetUniformLocation(programID, "texture_local");
        glUniform1i(loc2, indexActiveTexture);
        glEnableVertexAttribArray(2);

        // glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // TEST

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(2); 

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;

    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
