uniform sampler2DRect image;
uniform float rand;
uniform float levels;

varying vec3 pos;

void main(void){
    vec2 texCoord = vec2(pos.x , pos.y);
    vec4 col = texture2DRect(image, texCoord);

    vec4 amountPerLevel = vec4(1.0/levels);
    vec4 numOfLevels = floor(col/amountPerLevel);

    col.r = numOfLevels.r * (1.0 / (levels - 1.0));
    col.g = numOfLevels.g * (1.0 / (levels - 1.0));
    col.b = numOfLevels.b * (1.0 / (levels - 1.0));

    gl_FragColor.rgba = col.rgba;
}