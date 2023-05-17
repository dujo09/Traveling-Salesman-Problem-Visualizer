#version 330 core

in GS_OUT 
{
    vec3 color;
    noperspective float lineWidth;    
    noperspective float lineLength;
    noperspective float u;
    float antiAliasingRadius;
} fs_in;

out vec4 fragmentColor;

void main()
{
    float alphaValue = 1.0 - smoothstep( 1.0 - ((2.0 * fs_in.antiAliasingRadius) / fs_in.lineWidth),  1.0, abs(fs_in.u / fs_in.lineWidth) );

    fragmentColor = vec4(fs_in.color, alphaValue);
};