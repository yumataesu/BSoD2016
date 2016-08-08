uniform sampler2DRect image;
uniform float debug1;
uniform float debug255;

varying vec3 pos;

const vec4      gridRot = vec4(15.0, 45.0, 0.0, 75.0);

vec4 RGBAtoCMYK(vec4 inColor){
    vec4        ret;
    ret.w = 1.0 - max(max(inColor.x, inColor.y), inColor.z);
    ret.x = (1.0-inColor.x-ret.w)/(1.0-ret.w);
    ret.y = (1.0-inColor.y-ret.w)/(1.0-ret.w);
    ret.z = (1.0-inColor.z-ret.w)/(1.0-ret.w);
    return ret;
}
vec4 CMYKtoRGBA(vec4 inColor){
    vec4        ret;
    ret.xyz = (1.0-inColor.xyz)*(1.0-inColor.w);
    ret.w = 1.0;
    return ret;
}



void main(void){
    vec2 texCoord = vec2(pos.x, pos.y);
    vec4 col = texture2DRect(image,texCoord);

    vec4 cmykAmounts = vec4(0.0);
    for (int i=0; i<4; ++i) {
        float rotRad = radians(gridRot[i]);
        mat2 ccTrans = mat2(vec2(cos(rotRad), sin(rotRad)), vec2(-1.0*sin(rotRad), cos(rotRad)));
        mat2 cTrans = mat2(vec2(cos(rotRad), -1.0*sin(rotRad)), vec2(sin(rotRad), cos(rotRad)));

        //  render loc -> grid loc -> grid dot loc -> grid dot loc in render coords -> pixel color under grid dot loc
        vec2 gridFragLoc = cTrans * gl_FragCoord.xy;
        vec2 gridDotLoc = vec2(floor(gridFragLoc.x/debug255)*debug255, floor(gridFragLoc.y/debug255)*debug255);
        gridDotLoc = gridDotLoc + vec2(debug255/2.0);
        vec2 renderDotLoc = ccTrans * gridDotLoc;

        vec2 texCoord = vec2(pos.x, pos.y);
        vec4 col = texture2DRect(image,texCoord);

        vec4 renderDotImageColorRGB = texture2DRect(image,texCoord);
        vec4 renderDotImageColorCMYK = RGBAtoCMYK(renderDotImageColorRGB);

        float       channelAmount = renderDotImageColorCMYK[i];
        float       dotRadius = channelAmount * (debug255/2.0);
        float       fragDistanceToGridCenter = distance(gl_FragCoord.xy, renderDotLoc);
        //  the amount of the channel depends on the distance to the center of the grid, the size of the dot, and smoothing
        float       smoothDist = debug1 * (debug255/6.0);
        cmykAmounts[i] += smoothstep(dotRadius, dotRadius-(dotRadius*debug1), fragDistanceToGridCenter);
    }

    gl_FragColor = CMYKtoRGBA(cmykAmounts);
}