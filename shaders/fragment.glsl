#version 460 core
#extension GL_ARB_gpu_shader_int64 : require

struct PrimitiveTransform {
    vec4 position;
    vec4 rotation;
    vec4 scale;
    vec4 data;
};

struct PrimitiveInfo {
    int type;
    int colorID;
    float boundingDistance;
};

struct VoxelGridMetaData {
    ivec3 resolution;
    vec3 position;
    vec3 scale;
};

layout(location = 0) out vec4 color;

layout(binding = 0, std140) uniform UBO1 {
    vec4 uRayOrigin;
    vec2 uCameraAngle;
    ivec2 uResolution;
};

layout(binding = 1, std430) readonly buffer SSBO1 {
    PrimitiveInfo primitiveInfo[]; 
};

layout(binding = 2, std430) readonly buffer SSBO2 {
    PrimitiveTransform primitiveTransforms[];
};

layout(binding = 3, std140) uniform UBO2 {
    int primitiveCount;
};

layout(binding = 4, std430) readonly buffer SSBO3 {
    uint primitiveHitListTemplete[];
};

layout(binding = 5, std430) buffer SSBO4 {
    uint primitivePixelHitList[];
};

layout(binding = 6, std430) readonly buffer SSBO5 {
    uint64_t voxelGrid[];
};

layout(binding = 7, std430) readonly buffer SSBO6 {
    VoxelGridMetaData voxelGridMetaData;
};

struct MapOutput {
    float rayDistance;
    int primitiveIndex;
};

#define INVERSE_SQRT_OF_3 0.577350269189626 
#define MAX_RAY_DISTANCE 100
#define VOXEL_PADDING 1 //Make sure that this matches with the #define in 'voxelGrid.h'

#define SPHERE 0
#define RECTANGLE 1
#define GROUND 2
#define CONE 3

bool hitListFlag(uint index, int baseOffset) {
    uint word = primitivePixelHitList[baseOffset + int(index >> 5)]; 
    uint bit  = index & 31u; 

    return (word & (1u << bit)) != 0u;
}

void hitListClear(uint index, int baseOffset){

    uint bit  = index & 31u; 
    primitivePixelHitList[baseOffset + int(index >> 5)] &= ~(1u << bit);
}

float sdBox(vec3 worldPos, vec3 b ){
  vec3 q = abs(worldPos) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdSphere(vec3 worldPos, float radius){
    return length(worldPos) - radius;
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
    else if (colorID == 2){
        return vec3(0.25, 0.75, 0.50);
    }
    else if (colorID == 3){
        return vec3(0.75, 0.25, 0.50);
    }  
    else {
        return vec3(1.0, 0.0, 1.0);
    }
}

float sdCone( vec3 worldPos) {
  vec2 q = vec2(1,-1.0);
    
  vec2 w = vec2( length(worldPos.xz), worldPos.y );
  vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
  vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
  float k = sign( q.y );
  float d = min(dot( a, a ),dot(b, b));
  float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
  return sqrt(d)*sign(s);
}

int getPixelHitBaseOffset(){
    int wordCount = (primitiveCount + 31) / 32;
    ivec2 pixelCoord = ivec2(gl_FragCoord.xy);
    int pixelIndex = pixelCoord.y * uResolution.x + pixelCoord.x;
    return wordCount * pixelIndex;

}

float sdShape(vec3 worldPos, int index){

    vec4 extraData = primitiveTransforms[index].data;
    vec3 scaler = primitiveTransforms[index].scale.xyz;

    if (primitiveInfo[index].type == SPHERE){
        return sdSphere(worldPos, scaler.x);
    }
    else if (primitiveInfo[index].type == RECTANGLE){
        return sdBox(worldPos, scaler);
    }
    else if (primitiveInfo[index].type == GROUND){
        return worldPos.y;
    }
    else if (primitiveInfo[index].type == CONE){
        vec3 localPos = worldPos;
        localPos /= scaler; 
        float minScale = min(scaler.x, min(scaler.y, scaler.z));
        return sdCone(localPos)*minScale;
    }
    else {
        return 100000000.0;
    }
}

uint getGridIndex(vec3 worldPos){

    int x = int(worldPos.x + voxelGridMetaData.resolution.x/2);
    int y = int(worldPos.y + voxelGridMetaData.resolution.y/2);
    int z = int(worldPos.z + voxelGridMetaData.resolution.z/2);
    return x + y * voxelGridMetaData.resolution.x + z * voxelGridMetaData.resolution.x * voxelGridMetaData.resolution.y;
}

float sdVoxelGrid(vec3 worldPos){

    vec3 paddingThickness = 2.0 / voxelGridMetaData.resolution;
    float boundingSDF = sdBox(worldPos, voxelGridMetaData.scale + paddingThickness);
    float gridSDF = sdBox(worldPos, voxelGridMetaData.scale);
    if (boundingSDF < 0){

        vec3 localPos = worldPos * voxelGridMetaData.resolution.xyz / 2;
        uint index = getGridIndex(localPos+1.5);
        uint64_t voxelValue = voxelGrid[index];

        if(voxelValue != 0 && gridSDF < 0){
            return 0;
        }
        else {return 0.01;}
    }
    else {return gridSDF;}
}

MapOutput map(vec3 worldPos){
    MapOutput mapOutput = MapOutput (
        1000000.0,
        -1
    );
    float shapeSDF;
    float voxelGridSDF;
    vec3 localPos;

    for (int i = 0; i < primitiveCount; i++){

        if (hitListFlag(i, getPixelHitBaseOffset())){

            vec3 shapePos = primitiveTransforms[i].position.xyz;
            vec3 shapeScale = primitiveTransforms[i].scale.xyz;
            vec3 shapeRotation = primitiveTransforms[i].rotation.xyz;
            localPos = worldPos;
            localPos = localPos - shapePos;

            shapeSDF = sdShape(localPos, i);
            if (mapOutput.rayDistance > shapeSDF){
                mapOutput.rayDistance = shapeSDF;
                mapOutput.primitiveIndex = i;
            }
        }
    }
    localPos = worldPos;
    voxelGridSDF = sdVoxelGrid(localPos);
    if (mapOutput.rayDistance > voxelGridSDF){
        mapOutput.rayDistance = voxelGridSDF;
        mapOutput.primitiveIndex = 3;
    }
    return mapOutput;
}

vec3 calcNormal(vec3 p) {
    float epsilon = 0.0001;
    vec2 epsilonVector = vec2(1.0, -1.0) * INVERSE_SQRT_OF_3 * epsilon;

    return normalize(
        epsilonVector.xyy * map(p + epsilonVector.xyy).rayDistance +
        epsilonVector.yyx * map(p + epsilonVector.yyx).rayDistance +
        epsilonVector.yxy * map(p + epsilonVector.yxy).rayDistance +
        epsilonVector.xxx * map(p + epsilonVector.xxx).rayDistance
    );
}

void initPossibleHitList(vec3 rayOrigin, vec3 rayDirection){

    int wordCount = (primitiveCount + 31) / 32;
    int baseOffset = getPixelHitBaseOffset();

    for (int i = 0; i < wordCount; i++){
        primitivePixelHitList[baseOffset + i] = primitiveHitListTemplete[i];
    }

    for (int i = 0; i < primitiveCount; i++){

        vec3 position =  primitiveTransforms[i].position.xyz;
        vec3 shapePosition = position - rayOrigin;

        float t = dot(shapePosition, rayDirection);
        t = clamp(t, 0.0, float(MAX_RAY_DISTANCE));

        vec3 closestPointToRay = rayOrigin + rayDirection*t;
        float distanceFromRay = distance(closestPointToRay, position); 
        float boundingRadius = primitiveInfo[i].boundingDistance;

        if (distanceFromRay > boundingRadius){

            hitListClear(uint(i), baseOffset);
            
        }
    }
}

void main(){

    vec2 uv = (gl_FragCoord.xy *2.0 - vec2(uResolution.xy))/ float(uResolution.y);

    vec3 rayDirection = normalize(vec3(uv, 1));
    vec3 col = vec3(0.1);
   
    rayDirection = rot3D(rayDirection, vec3(1, 0, 0), uCameraAngle.y); 
    rayDirection = rot3D(rayDirection, vec3(0, 1, 0), uCameraAngle.x);

    initPossibleHitList(uRayOrigin.xyz, rayDirection);

    float t = 0.0;
    vec3 worldPos;
    vec3 normal;
    float rayDistance;
    MapOutput mapOutput;
 
    for (int i = 0; i < 10000; i++){
        worldPos = uRayOrigin.xyz + rayDirection * t;
        mapOutput = map(worldPos);
        rayDistance = mapOutput.rayDistance;
        t += rayDistance;

        if (rayDistance < .0005){
            int index = mapOutput.primitiveIndex;
            col = getColor(worldPos, primitiveInfo[index].colorID);
            normal = calcNormal(worldPos);
            vec3 lightDir = normalize(vec3(0.5, 0.6, -0.75));
            float diffuse = max(dot(normal, lightDir), 0.2);

            col *= diffuse;

            break;  
        } 

        if (t > MAX_RAY_DISTANCE){
            break;
        }
    }


    color = vec4(col, 1.0);
}