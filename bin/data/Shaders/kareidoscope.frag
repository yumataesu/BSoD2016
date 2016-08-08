#version 120
#extension GL_ARB_texture_rectangle: enable
#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

uniform sampler2DRect image;
varying vec3 pos;

uniform int ksectors;
uniform float kangleRad = 0.0;
uniform vec2 screenCenter = vec2( 512.0, 512.0 ) / 2.0;
uniform vec2 kcenter;
uniform int trueWidth, trueHeight;

void main(){
    //vec2 kcenter = vec2(kcenter) / 2.0;
    vec2 texCoord = vec2(pos.x , pos.y);
    vec2 v = texCoord - vec2( trueWidth, trueHeight ) / 2.0;
    float r = length( v );
    float a = atan( v.y, v.x );
    float b = TWO_PI/float(ksectors);
    a = mod( a, b );
    if ( a > b/2.0 ) a = b - a;
    a -= kangleRad;

    vec2 u = vec2( cos(a), sin(a) ) * r;
    u += kcenter;
    gl_FragColor = texture2DRect( image, u );
}