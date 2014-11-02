// Shader_Texture.glsl

#ifdef VertexShader

uniform mat4 u_WorldViewProj;
uniform sampler2D u_TextureColor;
attribute vec4 a_Position;
attribute vec4 a_VertexColor;
attribute vec2 a_UVCoord;
#ifdef WIN32
varying vec4 v_Color;
#else
varying lowp vec4 v_Color;
#endif
varying lowp vec2 v_UVCoord;
void main()
{
    v_Color = vec4( 0, 1, 0, 1 ); //a_VertexColor;
    gl_Position = u_WorldViewProj * a_Position;
    v_UVCoord = a_UVCoord;
}

#endif

#ifdef FragmentShader

#ifdef WIN32
varying vec4 v_Color;
#else
precision mediump float;
varying lowp vec4 v_Color;
#endif
varying lowp vec2 v_UVCoord;
uniform sampler2D u_TextureColor;
void main()
{
    gl_FragColor = texture2D( u_TextureColor, v_UVCoord );
}

#endif
