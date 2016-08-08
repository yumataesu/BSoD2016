#version 120

#define PI 3.14159265358979
#define TwoPI 6.28318530718

uniform sampler2D envMap;
varying vec3 reflectVec;

vec2 envMapEquirect(vec3 dir) {
    float phi = acos(-dir.y);
    float theta = atan(1.0 * dir.x, dir.z) + PI;
    return vec2(theta / TwoPI, phi / PI);
}


void main () {
    gl_FragColor = texture2D(envMap, envMapEquirect(reflectVec));
}