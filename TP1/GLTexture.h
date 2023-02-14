//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_GLTEXTURE_H
#define TP1_CODE_GLTEXTURE_H

#include "Texture.h"


class GLTexture : public Texture {

private:
    unsigned int texture;
    GLuint buffer_coord_txt;

public:
    void generateBuffer(std::vector<glm::vec2> & coord_texture){
        glGenBuffers(1, &buffer_coord_txt);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
        glBufferData(GL_ARRAY_BUFFER, coord_texture.size() * sizeof(glm::vec2), &coord_texture[0], GL_STATIC_DRAW);
    }

    void generateTexture(){
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void defineParameters(){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }

    void sendTextureToShader(GLuint programID){
        glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void *) 0);
        int indexActiveTexture = 0;// 0 to 31
        glBindTexture(GL_TEXTURE_2D , texture); // bind texture as Texture 0
        glActiveTexture(GL_TEXTURE0 + indexActiveTexture);
        // set used active texture (Modern OpenGL)
        GLuint loc2 = glGetUniformLocation(programID, "texture_local");
        glUniform1i(loc2, indexActiveTexture);
        glEnableVertexAttribArray(2);
    }

};


#endif //TP1_CODE_GLTEXTURE_H
