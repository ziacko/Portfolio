#version 420

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

uniform int numLayers;
uniform int layer;

in vec2 uv[];
out vec2 uvGeom;

void main()
{
	int i = 0;
	for(i; i < 5; ++i)
	{
		switch(i)
		{
			case 0:
			{
				gl_Layer = 0;
				gl_PrimitiveID = 0;
				break;
			}

			case 1:
			{
				gl_Layer = 1;
				gl_PrimitiveID = 1;
				break;
			}

			case 2:
			{
				gl_Layer = 2;
				gl_PrimitiveID = 2;
				break;
			}

			case 3:
			{
				gl_Layer = 3;
				gl_PrimitiveID = 3;
				break;
			}

			case 4:
			{
				gl_Layer = 4;
				gl_PrimitiveID = 4;
				break;
			}
		}
		
		gl_Position = gl_in[0].gl_Position;
		uvGeom = uv[0];
		EmitVertex();

		gl_Position = gl_in[1].gl_Position;
		uvGeom = uv[1];
		EmitVertex();

		gl_Position = gl_in[2].gl_Position;
		uvGeom = uv[2];
		EmitVertex();

		EndPrimitive();
	}
}