#version 120

#pragma include "inverse.vert"

uniform mat4 viewTranspose;

attribute vec3  normal;
varying vec3 reflectVec;


void main() {
    vec3 v_normal = normalize(gl_NormalMatrix * gl_Normal);
    vec4 position = gl_ModelViewMatrix * gl_Vertex;
    
    vec3 relfect0 = reflect(-normalize(position.xyz), v_normal);
    reflectVec = vec3(gl_ModelViewMatrixTranspose * vec4(relfect0, 1.0));
    
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
}