//
// Created by Adèle Imparato on 14/03/2023.
//

#ifndef TP1_CODE_TRANSFORM_H
#define TP1_CODE_TRANSFORM_H

#define PI 3.14159265

class Transform {

public:

    // general method to update scene graph
    void updateGraph(SceneGraph root, GLuint programID, Camera *camera){

        SceneObject obj = root.getData();

        // transform Model matrix
        for(int i = 0; i < obj.transformations.size(); i++){

            if(obj.index_transf[i]==0){
                camera->scaleModelMat(obj.transformations[i]);
            }else if(obj.index_transf[i]==1){
                camera->translateModelMat(obj.transformations[i]);
            }else if(obj.index_transf[i]==2){
                camera->rotateModelMat(obj.transformations[i] , 0.07);
            }
        }

        // update MVP matrix
        camera->sendMVPtoShader(programID);

        obj.loadBuffers();
        obj.draw(programID);

        for(SceneGraph *child : root.getChildren()){

            SceneObject child_obj = child->getData();

            updateGraph(*child, programID, camera);
        }

    }

    // general method to draw scene graph
    void drawGraph(SceneGraph root, GLuint programID){

        SceneObject obj_root = root.getData();

        obj_root.loadBuffers();
        obj_root.draw(programID);

        for(SceneGraph *child : root.getChildren()){
            SceneObject child_obj = child->getData();

            child_obj.loadBuffers();
            child_obj.draw(programID);
        }

    }

};


#endif //TP1_CODE_TRANSFORM_H
