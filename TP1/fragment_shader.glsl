#version 330 core

// Ouput data
out vec3 color;

out vec4 FragColor;

// texture
uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snowrocks;
in vec2 coord_txt;

// heightmap
in float height;

void main(){

        // color = vec3(0.2, 0.2,0.4);

        if(height < 0.5){
                //color = vec3(0.0, 0.0, 0.0); // black
                FragColor = texture(texture_grass, coord_txt);
        }else if (height > 0.5 && height < 0.7){
                //color = vec3(0.5, 0.5, 0.5); // grey
                FragColor = texture(texture_rock, coord_txt);
        }else{
                //color = vec3(0.9, 0.9, 0.9); // white
                FragColor = texture(texture_snowrocks, coord_txt);
        }

}
