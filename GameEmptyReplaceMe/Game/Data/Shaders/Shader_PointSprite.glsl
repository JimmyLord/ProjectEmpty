// Shader_PointSprite.glsl

#ifdef WIN32
#version 120
#define lowp
#else
precision mediump float;
#endif

varying lowp vec4 v_Color;
varying lowp vec2 v_UVCoord;

#ifdef VertexShader

attribute vec2 a_Position;
attribute vec2 a_UVCoord;
attribute vec4 a_VertexColor;

uniform mat4 u_WorldViewProj;

void main()
{
    v_UVCoord = a_UVCoord;
    v_Color = a_VertexColor;

    //gl_Position = vec4( a_Position, 0, 1 );
    gl_Position = u_WorldViewProj * vec4( a_Position, 0, 1 );
}

#endif

#ifdef FragmentShader

uniform sampler2D u_TextureColor;

void main()
{
    vec4 texcolor = texture2D( u_TextureColor, v_UVCoord );
    gl_FragColor = texture2D( u_TextureColor, v_UVCoord ) * texcolor.a * v_Color;
}

#endif
