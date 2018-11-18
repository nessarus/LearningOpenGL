#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

//naming convension u_Variable means a uniform variable
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//samples texture at texture coordinatates
	vec4 texColor = texture(u_Texture, v_TexCoord);

	//sends color to draw
	color = texColor;
}
