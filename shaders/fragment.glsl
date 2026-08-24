#version 460 core

layout(location = 0) out vec4 color;

uniform vec2 uResolution;
uniform float uCameraPhi;
uniform float uCameraTheda;
uniform float uCameraDistance;
uniform vec3 uRayOrigin;

struct PrimitiveTransform {
    vec4 position;
    vec4 rotation;
    vec4 scale;
    vec4 data;
};

struct PrimitiveInfo {
    int type;
    int colorID;
};

layout(binding = 1, std430) readonly buffer SSBO1 {
     PrimitiveInfo primitiveInfo[];
};

layout(binding = 2, std430) readonly buffer SSBO2 {
    PrimitiveTransform primitiveTransforms[];
};

struct MapOutput {
    float rayDistance;
    vec3 color;
};

#define SPHERE 0
#define RECTANGLE 1

float sdBox( vec3 p, vec3 b ){
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdSphere(vec3 worldPos, float radius){
    return length(worldPos) - radius;
}

vec3 rot3D(vec3 worldPos, vec3 axis, float angle){
    return mix(dot(axis, worldPos) * axis, worldPos, cos(angle)) + cross(axis, worldPos) * sin(angle);
}

float sdShape(vec3 worldPos, int index){
    vec3 shapePos = primitiveTransforms[index].position.xyz;
    vec3 shapeScale = primitiveTransforms[index].scale.xyz;
    vec3 shapeRotation = primitiveTransforms[index].rotation.xyz;

    if (primitiveInfo[index].type == SPHERE){
        return sdSphere(worldPos - shapePos, shapeScale.x);
    }
    else if (primitiveInfo[index].type == RECTANGLE){
        return sdBox(worldPos - shapePos, shapeScale);
    }
    else {
        return 100000.0;
    }
}

MapOutput map(vec3 worldPos){
    MapOutput mapOutput = MapOutput (
        100.0,
        vec3(1.0, 0.0, 1.0)
    );

    for (int i = 0; i < 2; i++){
        float shape = sdShape(worldPos, i);
        if (mapOutput.rayDistance > shape){
            mapOutput.rayDistance = shape;
        }
    }
    return mapOutput;
}

void main(){

    vec2 uv = (gl_FragCoord.xy *2.0 - uResolution.xy )/ uResolution.y;

    vec3 rayDirection = normalize(vec3(uv, 1));
    vec3 col = vec3(0);
   
    rayDirection = rot3D(rayDirection, vec3(1, 0, 0), uCameraTheda); 
    rayDirection = rot3D(rayDirection, vec3(0, 1, 0), uCameraPhi);

    float t = 0.0;
    vec3 worldPos;
    float rayDistance;
    MapOutput mapOutput;
 
    for (int i = 0; i < 100; i++){
        worldPos = uRayOrigin + rayDirection * t;
        mapOutput = map(worldPos);
        rayDistance = mapOutput.rayDistance;
        col = mapOutput.color;
        t += rayDistance;

        if (rayDistance < .0001 || t > 100.) break;
    }

    col = vec3(t * .05);

    color = vec4(col, 1.0);
}