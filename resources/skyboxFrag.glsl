#version 330 core

uniform samplerCube cubemap;
uniform sampler2D cameraDepth;
uniform mat4 prevViewProjectionMatrix;

in vec3 vNor; // normal in world space
in mat4 viewProjectionInverseMatrix;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec2 MotionVector;

void calcVelocity()
{
    float zOverW = texture(cameraDepth, vNor.xy).r;
    vec2 shift = vNor.xy * 0.5 + 0.5;
    
    vec4 H = vec4(shift.x, (1-shift.y), zOverW, 1.0);
    vec4 D = viewProjectionInverseMatrix * H;
    vec4 worldPos = D / D.w;
    
    vec4 currPos = H;
    vec4 prevPos = prevViewProjectionMatrix * worldPos;
    prevPos = prevPos / prevPos.w;
    
    MotionVector = (currPos - prevPos).xy;
}

void main()
{
    // We want the normal to be in world coords, not camera coords.
    FragColor = texture(cubemap, vNor);
    calcVelocity();
}
