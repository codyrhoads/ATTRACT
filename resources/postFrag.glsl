#version 330 core

in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D colorBuf;
uniform sampler2D motionVectors;

void main() {
    vec4 color = vec4(0.0);
//    color = texture(colorBuf, texCoord);
    vec2 motionV = texture(motionVectors, texCoord).xy;
    motionV = (motionV * 2 - 1)/60;
//    vec3 temp = normalize(vec3(motionV.x, motionV.y, 0.0));

//    if (motionV.x >= 0.5) {
//        FragColor = vec4(motio, 0.0, 1.0, 1.0);
//    }
//    else if (motionV.x <= 0.5) {
//        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//    }
//    else {
//        FragColor = color;
//    }
//FragColor = vec4(temp, 1.0);

    vec2 texCoord0 = texCoord;
    color += texture(colorBuf, texCoord0) * 0.4;
    texCoord0 -= motionV;
    color += texture(colorBuf, texCoord0) * 0.3;
    texCoord0 -= motionV;
    color += texture(colorBuf, texCoord0) * 0.2;
    texCoord0 -= motionV;
    color += texture(colorBuf, texCoord0) * 0.1;
    texCoord0 -= motionV;

    FragColor = color;
}
