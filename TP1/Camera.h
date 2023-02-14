//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_CAMERA_H
#define TP1_CODE_CAMERA_H


class Camera {

private:
    // MVP
    glm::mat4 View_Matrix;
    glm::mat4 Projection_Matrix;
    glm::mat4 Model_Matrix;

public:
    void MVP(GLuint programID){
        computeMatricesFromInputs();

        View_Matrix = getViewMatrix();
        Model_Matrix = glm::mat4(); // identity matrix (model will be at the origin) then change
        Projection_Matrix = getProjectionMatrix();

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms
        glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, &Model_Matrix[0][0]); // location, count, transpose, value
        glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, &View_Matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, &Projection_Matrix[0][0]);
    }

};


#endif //TP1_CODE_CAMERA_H
