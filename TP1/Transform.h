//
// Created by Adèle Imparato on 14/03/2023.
//

#ifndef TP1_CODE_TRANSFORM_H
#define TP1_CODE_TRANSFORM_H

#define PI 3.14159265

class Transform {

public:

    void transformAndDraw(SceneGraph root, GLuint programID, Camera *camera){

        // Sun
        SceneObject obj_root = root.getData();

        camera->scaleModelMat(0.8f);
        // update MVP matrix
        camera->sendMVPtoShader(programID);

        obj_root.loadBuffers();
        obj_root.draw(programID);

        // Earth
        for(SceneGraph *child : root.getChildren()){
            SceneObject earth = child->getData();

            camera->scaleModelMat(0.5f); // earth two times smaller than sun
            camera->translateModelMat(5, 0, 0);

            // earth turns around sun
            camera->translateModelMat(-5, 0, 0);
            camera->rotateModelMat(glm::vec3(0.0,1.0,0.0));
            camera->translateModelMat(5, 0, 0);
            camera->rotateModelMat(glm::vec3(0.0,-1.0,0.0));

            // earth turns around itself
            camera->rotateModelMat(glm::vec3(sin(23.f*PI/180),cos(23.f*PI/180),0));

            // update MVP matrix
            camera->sendMVPtoShader(programID);

            earth.loadBuffers();
            earth.draw(programID);

            // Moon
            for(SceneGraph *child_earth : child->getChildren()) {

                SceneObject moon = child_earth->getData();

                // get moon from earth
                camera->scaleModelMat(0.45f);
                camera->translateModelMat(5*cos(23.f*PI/180), (-5)*sin(23.f*PI/180), 0);

                // moon turns around earth
                camera->translateModelMat(-5, 0, 0); // move it to center of earth
                camera->rotateModelMat(glm::vec3(sin(23.f*PI/180),cos(23.f*PI/180),0)); // same rotation axis as earth
                camera->translateModelMat(5, 0, 0); // move it back to its place

                // update MVP matrix
                camera->sendMVPtoShader(programID);

                moon.loadBuffers();
                moon.draw(programID);
            }
        }
    }

};


#endif //TP1_CODE_TRANSFORM_H
