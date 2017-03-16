#version 330 core

in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D colorBuf;
uniform sampler2D motionVectors;

void main()
{
    vec4 color = vec4(0.0);
    vec2 motionV = texture(motionVectors, texCoord).xy / 30.0;

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
