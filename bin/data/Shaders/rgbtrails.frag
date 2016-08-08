#version 120

uniform sampler2DRect image;
uniform float rand;
uniform float time;

varying vec3 pos;

void main(){
vec2 texCoord = vec2(pos.x , pos.y);
vec4 col = texture2DRect(image, texCoord);
vec4 stablecol = texture2DRect(image, texCoord);

gl_FragColor = vec4(mix(col.r, stablecol.r, sin(time)),
                    mix(col.g, stablecol.g, 0.82),
                    mix(col.b, stablecol.b, 0.50),
                    mix(col.a, stablecol.a, 0.28)
                    );
}

//以下の書き換え必要
//vec4        stalePixel = IMG_THIS_PIXEL(accum);