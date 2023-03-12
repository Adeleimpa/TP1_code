//
// Created by Adèle Imparato on 09/03/2023.
//

#ifndef TP1_CODE_SCENEOBJECT_H
#define TP1_CODE_SCENEOBJECT_H

#include <iostream>
#include <glm/glm.hpp>


class SceneObject {

private:

public:

    std::vector<unsigned short> indices; // Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> coord_texture; // texture
    std::vector<glm::vec3> normals;

    SceneObject() {}

    void draw() const {
        if( triangles.size() == 0 ) return;

        glEnableClientState(GL_VERTEX_ARRAY) ;
        glEnableClientState (GL_NORMAL_ARRAY);
        glNormalPointer (GL_FLOAT, 3*sizeof (float), (GLvoid*)(normals.data()));
        glVertexPointer (3, GL_FLOAT, 3*sizeof (float) , (GLvoid*)(indexed_vertices.data()));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
        // or try last arg = (void*)0
    }

    void loadBuffers(GLuint vertexbuffer, GLuint elementbuffer){
        // Load data (vertices, meshes, etc.) into VBO's
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
    }

    void clearVectors(){
        indexed_vertices.clear();
        indices.clear();
        triangles.clear();
        normals.clear();
        coord_texture.clear();
    }

};


#endif //TP1_CODE_SCENEOBJECT_H
