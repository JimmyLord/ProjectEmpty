// Shader_Font.glsl

#ifdef WIN32
#define lowp
#endif

varying lowp vec4 v_Color;
varying lowp vec2 v_UVCoord;

#ifdef VertexShader

uniform mat4 u_WorldViewProj;
uniform sampler2D u_TextureColor;
uniform vec4 u_TextureTintColor;

attribute vec4 a_Position;
attribute vec4 a_VertexColor;
attribute vec2 a_UVCoord;

void main()
{
    v_Color = u_TextureTintColor; //vec4( 0, 1, 0, 1 ); //a_VertexColor;
    gl_Position = u_WorldViewProj * a_Position;
    v_UVCoord = a_UVCoord;
}

#endif

#ifdef FragmentShader

uniform sampler2D u_TextureColor;

void main()
{
    gl_FragColor = texture2D( u_TextureColor, v_UVCoord ) * v_Color;
}

#endif
