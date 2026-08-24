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
#define GROUND 2

float sdBox(vec3 worldPos){
  vec3 q = abs(worldPos) - vec3(1.0, 1.0, 1.0);
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdSphere(vec3 worldPos){
    return length(worldPos) - 1;
}

vec3 rot3D(vec3 worldPos, vec3 axis, float angle){
    return mix(dot(axis, worldPos) * axis, worldPos, cos(angle)) + cross(axis, worldPos) * sin(angle);
}

vec3 getColor(const vec3 worldPos, int colorID){
    if (colorID == 0){
        return vec3(1.0, 1.0, 1.0);
    }
    else if (colorID == 1){
        return vec3(0.0, 1.0, 1.0);
    } 
    else {
        return vec3(1.0, 0.0, 1.0);
    }
}

float sdShape(const vec3 worldPos, int index){

    vec4 extraData = primitiveTransforms[index].data;

    if (primitiveInfo[index].type == SPHERE){
        return sdSphere(worldPos);
    }
    else if (primitiveInfo[index].type == RECTANGLE){
        return sdBox(worldPos);
    }
    else if (primitiveInfo[index].type == GROUND){
        return worldPos.y;
    }
    else {
        return 1000000.0;
    }
}

MapOutput map(vec3 worldPos){
    MapOutput mapOutput = MapOutput (
        1000000.0,
        vec3(0.0, 0.0, 0.0)
    );
    float shapeSDF;
    vec3 localPos;

    for (int i = 0; i < 3; i++){

        vec3 shapePos = primitiveTransforms[i].position.xyz;
        vec3 shapeScale = primitiveTransforms[i].scale.xyz;
        vec3 shapeRotation = primitiveTransforms[i].rotation.xyz;
        localPos = worldPos;
        localPos /= shapeScale; 
        localPos = localPos - shapePos;

        shapeSDF = sdShape(localPos, i);
        if (mapOutput.rayDistance > shapeSDF){
            mapOutput.rayDistance = shapeSDF;
            mapOutput.color = getColor(localPos, primitiveInfo[i].colorID);
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
 
    for (int i = 0; i < 100000; i++){
        worldPos = uRayOrigin + rayDirection * t;
        mapOutput = map(worldPos);
        rayDistance = mapOutput.rayDistance;
        t += rayDistance;

        if (rayDistance < .001){
            col = mapOutput.color;
            break;  
        } 
        if (t > 100.0){
            break;
        }
    }


    color = vec4(col, 1.0);
}