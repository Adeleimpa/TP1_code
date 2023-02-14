//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_TEXTURE_H
#define TP1_CODE_TEXTURE_H


class Texture {

private:


public:
    int width, height, nrChannels;
    unsigned char *data;

    void readTexture(char * path){
        data = stbi_load(path, &width , &height , &nrChannels , 0);

        if (data) {} else { std::cout << "Failed to load texture" << std::endl; }
        stbi_image_free(data);
    }
};


#endif //TP1_CODE_TEXTURE_H
