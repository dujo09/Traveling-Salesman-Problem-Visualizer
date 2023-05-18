#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 viewportSize;


in VS_OUT 
{
    vec3 color;
    float lineWidth;
} gs_in[]; 

out GS_OUT 
{
    vec3 color;
    noperspective float lineWidth;    
    noperspective float lineLength;
    noperspective float u;
    float antiAliasingRadius;
} gs_out;

const float antiAliasingRadius = 1.5f;

void main() 
{    
    float viewportWidth = viewportSize[0];
    float viewportHeight = viewportSize[1];
    float aspectRatio = viewportHeight / viewportWidth;

    vec2 lineVector = gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy;
    vec2 viewportLineVector = lineVector * viewportSize;
    vec2 direction = normalize(vec2( lineVector.x, lineVector.y * aspectRatio ));
    
    float lineWidthTotal = max( 1.0f, gs_in[0].lineWidth) + antiAliasingRadius;
    float lineLength = length( viewportLineVector );
    
    vec2 normal = vec2( lineWidthTotal / viewportWidth, lineWidthTotal / viewportHeight ) * vec2( -direction.y, direction.x );

    gs_out.color = gs_in[0].color;
    gs_out.antiAliasingRadius = antiAliasingRadius;
    gs_out.u = lineWidthTotal;
    gs_out.lineWidth = lineWidthTotal;
    gs_out.lineLength = lineLength * 0.5f;
    gl_Position = vec4( (gl_in[0].gl_Position.xy + normal) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw );
    EmitVertex();

    gs_out.u = -lineWidthTotal;
    gs_out.lineWidth = lineWidthTotal;
    gs_out.lineLength = lineLength * 0.5f;
    gl_Position = vec4( (gl_in[0].gl_Position.xy - normal) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw );
    EmitVertex();

    // gs_out.color = mix(gs_in[0].color, mixColor, gs_in[0].color);
    gs_out.u = lineWidthTotal;
    gs_out.lineWidth = lineWidthTotal;
    gs_out.lineLength = lineLength * 0.5f;
    gl_Position = vec4( (gl_in[1].gl_Position.xy + normal) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw );
    EmitVertex();

    gs_out.u = -lineWidthTotal;
    gs_out.lineWidth = lineWidthTotal;
    gs_out.lineLength = lineLength * 0.5f;
    gl_Position = vec4( (gl_in[1].gl_Position.xy - normal) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw );
    EmitVertex();

    EndPrimitive();
} 