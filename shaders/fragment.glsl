#version 460 core

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

layout(location = 0) out vec4 color;

layout(binding = 0, std140) uniform UBO1 {
    vec4 uRayOrigin;
    vec2 uCameraAngle;
    vec2 uResolution;
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

struct MapOutput {
    float rayDistance;
    int primitiveIndex;
};

#define INVERSE_SQRT_OF_3 0.577350269189626

#define SPHERE 0
#define RECTANGLE 1
#define GROUND 2
#define CONE 3

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
    else if (primitiveInfo[index].type == CONE){
        return sdCone(worldPos);
    }
    else {
        return 1000000.0;
    }
}

MapOutput map(vec3 worldPos){
    MapOutput mapOutput = MapOutput (
        1000000.0,
        -1
    );
    float shapeSDF;
    vec3 localPos;

    for (int i = 0; i < primitiveCount; i++){

        vec3 shapePos = primitiveTransforms[i].position.xyz;
        vec3 shapeScale = primitiveTransforms[i].scale.xyz;
        vec3 shapeRotation = primitiveTransforms[i].rotation.xyz;
        localPos = worldPos;
        localPos /= shapeScale; 
        localPos = localPos - shapePos;

        shapeSDF = sdShape(localPos, i);
        if (mapOutput.rayDistance > shapeSDF){
            mapOutput.rayDistance = shapeSDF;
            mapOutput.primitiveIndex = i;
        }
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

void main(){

    vec2 uv = (gl_FragCoord.xy *2.0 - uResolution.xy )/ uResolution.y;

    vec3 rayDirection = normalize(vec3(uv, 1));
    vec3 col = vec3(0.1);
   
    rayDirection = rot3D(rayDirection, vec3(1, 0, 0), uCameraAngle.y); 
    rayDirection = rot3D(rayDirection, vec3(0, 1, 0), uCameraAngle.x);

    float t = 0.0;
    vec3 worldPos;
    vec3 normal;
    float rayDistance;
    MapOutput mapOutput;
 
    for (int i = 0; i < 100000; i++){
        worldPos = uRayOrigin.xyz + rayDirection * t;
        mapOutput = map(worldPos);
        rayDistance = mapOutput.rayDistance;
        t += rayDistance;

        if (rayDistance < .0001){
            int index = mapOutput.primitiveIndex;
            col = getColor(worldPos, primitiveInfo[index].colorID);
            normal = calcNormal(worldPos);
            vec3 lightDir = normalize(vec3(0.5, 0.6, -0.75));
            float diffuse = max(dot(normal, lightDir), 0.2);

            col *= diffuse;

            break;  
        } 

        if (t > 75.0){
            break;
        }
    }


    color = vec4(col, 1.0);
}