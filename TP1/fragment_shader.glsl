#version 330 core

// Ouput data
out vec3 color;

out vec4 FragColor;

// texture
uniform sampler2D texture_local;
in vec2 coord_txt;

void main(){

        //color =vec3(0.2, 0.2,0.4);

        FragColor = texture(texture_local, coord_txt);

}
