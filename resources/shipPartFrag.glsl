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

out vec4 FragColor;

/* returns 1 if shadowed */
/* called with the point projected into the light's coordinate space */
float TestShadow(vec4 LSfPos) {
    float bias = 0.004;
    
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
    
    //    float shadow = 0.0;
    //    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    //    for(int x = -1; x <= 1; ++x)
    //    {
    //        for(int y = -1; y <= 1; ++y)
    //        {
    //            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
    //            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    //        }
    //    }
    //    shadow /= 9.0;
    
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
        //        if (shadow > 0.5) {
        //            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        //        }
        //        else {
        //            FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        //        }
        //FragColor = vec4(1.0, 1.0, shadow, 1.0);
        
        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
        
        FragColor = vec4(lighting, 1.0f);
    }
}
