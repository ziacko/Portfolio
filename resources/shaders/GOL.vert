#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UV;

out defaultBlock
{
	vec4 position;
	vec2 uv;
	vec4 color;
} outBlock;

//unsigned int NeighborCount = 0;
//unsigned int DeadNeighborCount = 0;

layout(packed, binding = 0) uniform defaultSettings 
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};
//ok use a shader storage buffer to store all tihs cell data
layout(std140, binding = 1) uniform GOLSettings 
{
	vec4 AliveColor;
	vec4 DeadColor;
	vec4 EmptyColor;
	float Dimensions;
};

//all this was an attempt at implementing game of life inside a shader that I could never get working using shader buffer object. (similar to uniform buffer)

layout(std430, binding = 0) buffer GOLStatus 
{
	int Status[];
};
/*
void CheckNode(unsigned int State)
{
	switch(State)
	{
		case 0:
		{
			NeighborCount++;
			break;
		}

		case 1:
		{
			DeadNeighborCount++;
			break;
		}

		case 2:
		{
			break;
		}
	}
}

void Tick()
{ 
	if (gl_InstanceID < Dimensions - 1)
	{
		//check if the Item is not in the last column
		if (mod(gl_InstanceID, (Dimensions - 1)) == 0)
		{
			//next row, next column
			CheckNode(Status[gl_InstanceID + int((Dimensions - 1)) + 1]);
			//next row, this column
			CheckNode(Status[gl_InstanceID + int((Dimensions - 1))]);
			//this row, next column
			CheckNode(Status[gl_InstanceID + 1]);
		}

		//check if its the last column in the row
		else if (mod(gl_InstanceID, (Dimensions - 1)) == 0)
		{
		//Status[gl_InstanceID]
			//next row, last column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1)) - 1]);
			//next row, this column
			CheckNode(Status[gl_InstanceID + int(Dimensions - 1)]);
			//this row, last column
			CheckNode(Status[gl_InstanceID - 1]);
		}

		else
		{
			//5 neighbors to consider
			//this row, last column
			CheckNode(Status[gl_InstanceID - 1]);
			//this row, next column
			CheckNode(Status[gl_InstanceID + 1]);
			//next row, last column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1)) - 1]);
			//next row, this column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1))]);
			//next row, next column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1)) + 1]);
		}
	}

	//check if node is in the last row
	else if (gl_InstanceID >
		(((Dimensions * Dimensions) - Dimensions) - 1)
		&& gl_InstanceID < ((Dimensions * Dimensions) -1))
	{
		//check if the column is a multiple of the dimension (the first column)
		if (mod(gl_InstanceID, Dimensions) == 0)
		{
			//last row, next column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1)) + 1]); //lower middle node
			//last row, this column
			CheckNode(Status[gl_InstanceID - int(Dimensions - 1)]); //lower middle node
			//this row, next column
			CheckNode(Status[gl_InstanceID + 1]); //lower middle node
		}

		else if (mod(gl_InstanceID, (Dimensions - 1)) == 0)
		{
			//last row, this column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1))]); //lower middle node
			//last row, last column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1))]); //lower middle node
			//this row, last column
			CheckNode(Status[gl_InstanceID - 1]); //lower middle node
		}

		else
		{
			//5 neighbors to consider
			//this row, last column
			CheckNode(Status[gl_InstanceID - 1]);
			//this row, next column
			CheckNode(Status[gl_InstanceID + 1]);
			//last row, last column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1)) - 1]);
			//last row, this column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1))]);
			//last row, next column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1)) + 1]);
		}
	}

	else 
	{
		if (gl_InstanceID < (Dimensions * Dimensions) - 1)
		{
			//8 neighbors to check
			//last row, last column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1)) - 1]);
			//last row, this column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1))]);
			//last row, next column
			CheckNode(Status[(gl_InstanceID - int(Dimensions - 1) + 1)]);
			//this row, last column
			CheckNode(Status[gl_InstanceID - 1]);
			//this row, next column
			CheckNode(Status[gl_InstanceID + 1]);
			//next row, last column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1)) - 1]);
			//next row, this column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1))]);
			//next row, next column
			CheckNode(Status[(gl_InstanceID + int(Dimensions - 1)) + 1]);
		}
	}

	if (NeighborCount < 2 && Status[gl_InstanceID] == 1)
	{
		Status[gl_InstanceID] = 2;
	}

	else if (NeighborCount >= 2 && Status[gl_InstanceID] == 1)
	{
		if (NeighborCount > 3)
		{
			Status[gl_InstanceID] = 2;
		}

		else
		{
			Status[gl_InstanceID] = 1;
		}
	}

	else if (NeighborCount == 3 && Status[gl_InstanceID] == 2)
	{
		Status[gl_InstanceID] = 1;
	}
}*/

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = UV;// / Resolution;

	//oooooh right this uses instanced rendering.
	float XResult = 0;
	float YResult = modf(gl_InstanceID / Dimensions.x, XResult); 
	outBlock.position.x = outBlock.position.x + (XResult * ((resolution.x / Dimensions.x) / resolution.x)) * 2;
	outBlock.position.y = outBlock.position.y + YResult * 2;

	/*
	need to use instanceId to figure out where in the grid the instance belongs
	instance that is less than Dimension is in the first row.
	*/
	/*if(gl_VertexID == 0)
	{
		Tick();
	}*/

	if(Status[gl_InstanceID] == 0)
	{
		outBlock.color = vec4(0, 0, 1, 1);//EmptyColor;
	}

	if(Status[gl_InstanceID] == 1)
	{
		outBlock.color = vec4(0, 1, 0, 1);
		//AliveColor;
	}

	if(Status[gl_InstanceID] == 2)
	{
		outBlock.color = vec4(1, 0, 0, 1);
		//DeadColor;
	}

	gl_Position = outBlock.position;
}