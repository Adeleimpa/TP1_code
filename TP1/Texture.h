//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_TEXTURE_H
#define TP1_CODE_TEXTURE_H


class Texture {

private:
    int width, height, nrChannels;
    unsigned char *data;


public:
    void loadTexture(char * path){
        data = stbi_load(path, &width , &height , &nrChannels , 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE , data);
            glGenerateMipmap(GL_TEXTURE_2D); }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
};


#endif //TP1_CODE_TEXTURE_H
