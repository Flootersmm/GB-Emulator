// https://gist.githubusercontent.com/sebavan/cd82b33624a0da47a4f120f74b64cee4/raw/375f380674aea478e0e93607aaaac40275f9a348/gameboy.glsl
varying vec2 vUV;

// Default Sampler
uniform sampler2D textureSampler;

// Transform color to luminance.
float getLuminance(vec3 color)
{
    return clamp(dot(color, vec3(0.2126, 0.7152, 0.0722)), 0., 1.);
}

// Returns whether the given fragment position lies on a grid line.
bool onGridline(float distFrom, float spacing)
{
    return mod(floor(distFrom), spacing) == 0.0;
}

void main(void) 
{
    // Color Palette
    vec4 palette[4];
    palette[0] = vec4( 22, 30, 87, 255.)  / 255.;
    palette[1] = vec4( 78, 109, 90, 255.) / 255.;
    palette[2] = vec4(106, 149, 50, 255.) / 255.;
    palette[3] = vec4(115, 161, 43, 255.) / 255.;

    // Luminance adaptation
    const float lumaOffset = 0.2;

    // Grid lines
    const vec4 gridLineColor = vec4(vec3(120, 168, 51) / 255. , 1.0);
    const float gridSize = 8.;

    // Screen Definition
    const float screenSize = 512.;
    vec2 tileUV = floor(gl_FragCoord.xy / gridSize) * gridSize / screenSize;

    // Square fetch of luminance
    vec4 tileColor = texture2D(textureSampler, tileUV);
    float tileLuminance = getLuminance(tileColor.rgb);

    // Adapt luma to effect and pick from the palette
    int colorChoice = int(tileLuminance * 3. + lumaOffset);
    #ifdef WEBGL2
        vec4 finalColor = palette[colorChoice];
    #else
        vec4 finalColor = vec4(0.);
        if (colorChoice == 0) {
            finalColor = palette[0];
        } else if (colorChoice == 1) {
            finalColor = palette[1];
        } else if (colorChoice == 2) {
            finalColor = palette[2];
        } else {
            finalColor = palette[3];
        }
    #endif

    // Are we on a line.
    if (onGridline(gl_FragCoord.x, gridSize) || onGridline(gl_FragCoord.y, gridSize))
    {
        gl_FragColor = gridLineColor;
        return;
    }

    gl_FragColor = finalColor;
}
