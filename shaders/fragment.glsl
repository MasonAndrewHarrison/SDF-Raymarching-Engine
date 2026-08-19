#version 330 core

layout(location = 0) out vec4 color;
uniform vec2 uResolution;
uniform float uCameraPhi;
uniform float uCameraDistance;

float sdSphere(vec3 worldPos, float radius){
    return length(worldPos) - radius;
}

vec3 rot3D(vec3 worldPos, vec3 axis, float angle){
    return mix(dot(axis, worldPos) * axis, worldPos, cos(angle)) + cross(axis, worldPos) * sin(angle);
}

float map(vec3 worldPos){
    vec3 spherePos = vec3(3, 0, 0);
    vec3 rotatedPos = rot3D(worldPos, vec3(0, 1, 0), uCameraPhi);
    float sphere = sdSphere(rotatedPos - spherePos, 1.);

    float ground = worldPos.y + .75;
    return min(ground, sphere);
}

void main(){

    vec2 uv = (gl_FragCoord.xy *2.0 - uResolution.xy )/ uResolution.y;


    vec3 ro = vec3(0, 0, uCameraDistance);
    vec3 rd = normalize(vec3(uv, 1));
    vec3 col = vec3(0);

    float t = 0.0;
    vec3 worldPos;
    float rayDistance;

    for (int i = 0; i < 100; i++){
        worldPos = ro + rd * t;
        rayDistance = map(worldPos);
        t += rayDistance;

        if (rayDistance < .0001 || t > 100.) break;
    }

    col = vec3(t * .05);

    color = vec4(col, 1.0);
}