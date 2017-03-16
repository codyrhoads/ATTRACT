#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D shadowDepth;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec2 vTex;
in vec3 fragPos;
in vec3 fragNor;
in vec4 fragPosLS;
in vec4 fragPos1;
in vec4 prevFragPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec2 MotionVector;

void calcVelocity()
{
    //    vec3 projCoords = fragPos1.xyz / fragPos1.w;
    //    vec3 shift = projCoords * 0.5 + 0.5;
    //    float zOverW = texture(cameraDepth, shift.xy).r;
    //    //vec2 shift = vTex * 0.5 + 0.5;
    //
    //    vec4 H = vec4(shift.x, shift.y, zOverW, 1.0);
    //    vec4 D = viewProjectionInverseMatrix * H;
    //    vec4 worldPos = D / D.w;
    //
    //    vec4 curPos = H;
    //    vec4 prevPos = prevViewProjectionMatrix * worldPos;
    //    prevPos = prevPos / prevPos.w;
    //
    //    MotionVector = (curPos - prevPos).xy;
    MotionVector = ((fragPos1.xyz/fragPos1.w - prevFragPos.xyz/prevFragPos.w).xy + 1) / 2;
}

/* returns 1 if shadowed */
/* called with the point projected into the light's coordinate space */
float TestShadow(vec4 LSfPos) {
    float bias = 0.02;
    
    // perspective divide isn't needed since we're using ortho projection, but
    // just in case it changes to persepective.
    vec3 projCoords = LSfPos.xyz / LSfPos.w;
    
    //1: shift the coordinates from -1, 1 to 0 ,1
    vec3 shift = projCoords * 0.5 + 0.5;
    //2: read off the stored depth (.r) from the ShadowDepth, using the shifted.xy
    float curD = shift.z;
    float lightD = texture(shadowDepth, shift.xy).r;
    
    if (shift.x > 1.0 || shift.y > 1.0 || shift.x < 0.0 || shift.y < 0.0) {
        return -1.0;
    }
    
    //3: compare to the current depth (.z) of the projected depth
    //4: return 1 if the point is shadowed
    if (curD - bias > lightD) {
        return 1.0;
    }
    
    return 0.0;
}

void main()
{
    vec3 color = texture(diffuseTex, vTex).rgb;
    //    vec3 ks = texture(specularTex, vTex).rgb;
    
    vec3 normal = normalize(fragNor);
    
    vec3 lightColor = vec3(1.0);
    vec3 ambient = 0.3 * color;
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    float shadow = TestShadow(fragPosLS);
    
    if (shadow < -0.5) {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else {
        vec3 lighting = ambient + ((1.0 - shadow) * (diffuse + specular)) * color;
        
        FragColor = vec4(lighting, 1.0f);
    }
    
    calcVelocity();
}
