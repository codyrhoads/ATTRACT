#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

void main(){
    
    float depth = texture(texBuf, texCoord).r;
    color = vec4(vec3(depth), 1.0);
}
