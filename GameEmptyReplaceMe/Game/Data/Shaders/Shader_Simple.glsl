// Shader_Simple.glsl

#ifdef VertexShader

uniform mat4 u_WorldViewProj;
attribute vec4 a_Position;
attribute vec4 a_VertexColor;
#ifdef WIN32
varying vec4 v_Color;
#else
varying lowp vec4 v_Color;
#endif
void main()
{
    v_Color = a_VertexColor;
    gl_Position = u_WorldViewProj * a_Position;
}

#endif

#ifdef FragmentShader

#ifdef WIN32
varying vec4 v_Color;
#else
precision mediump float;
varying lowp vec4 v_Color;
#endif
void main()
{
    gl_FragColor = v_Color;
}

#endif
