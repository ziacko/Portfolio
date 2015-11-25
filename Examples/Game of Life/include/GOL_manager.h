#pragma once
enum cellState_t
{
	EMPTY = 0,
	ALIVE,
	DEAD
};

struct GOL_SettingsBuffer_t
{
	glm::vec4		aliveColor;
	glm::vec4		deadColor;
	glm::vec4		emptyColor;
	GLfloat			dimensions;


	GOL_SettingsBuffer_t(GLfloat dimensions,
		glm::vec4 aliveColor = glm::vec4(0, 1, 0, 1),
		glm::vec4 deadColor = glm::vec4(1, 0, 0, 1),
		glm::vec4 emptyColor = glm::vec4(0, 0, 1, 1))
	{
		this->dimensions = dimensions;
		this->aliveColor = aliveColor;
		this->deadColor = deadColor;
		this->emptyColor = emptyColor;
	}
};

class GOL_manager
{
public:

	GLfloat							tickDelay;
	GLfloat							currentTickDelay;
	GLuint							randomSeed;

	GLuint							programGLID;
	
	GLuint							defaultSettingsHandle;
	GLuint							defaultUniformBufferHandle;
	defaultUniformBuffer_t*			defaultUniformBuffer;

	GLuint							GOLsettingsHandle;
	GLuint							GOLsettingsBufferHandle;
	GOL_SettingsBuffer_t*			GOLsettingsBuffer;

	GLuint							vertexBuffer;
	GLuint							vertexArray;	
	
	GLuint							GOLstatusBufferHandle;
	GLuint							GOLstatusHandle;	

	std::vector< cellState_t >			cells;

	GOL_manager(camera* GOL_camera, GLuint program, GLuint dimensions = 100, GLfloat tickDelay = 0.0f,
		GLuint randomSeed = 1337 /* lol */, GLuint probability = 60 /* 100 */)
	{
		this->programGLID = program;
		this->defaultUniformBuffer = new defaultUniformBuffer_t(GOL_camera->projection,
			GOL_camera->view, GOL_camera->translation, GOL_camera->resolution);

		this->GOLsettingsBuffer = new GOL_SettingsBuffer_t(dimensions);

		std::srand(randomSeed);

		for (GLuint iter = 0; iter < (dimensions * dimensions); iter++)
		{
			GLuint randomValue = std::rand() % 100;
			cells.push_back((cellState_t)(randomValue < probability));
		}

		GenerateBuffers();

		glEnable(GL_DEPTH_TEST);
	}

	void Draw( GLvoid )
	{
		glUseProgram(programGLID);
		glBindVertexArray(vertexArray);
		glDrawArraysInstanced( GL_QUADS, 0, 4, ( GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions ) );
	}

	void CheckNode( GLuint currentState, GLuint& neighborCount, GLuint& deadNeighborCount )
	{
		switch ( currentState )
		{
		case ALIVE:
		{
			neighborCount++;
			break;
		}
		case DEAD:
		{
			deadNeighborCount++;
			break;
		}

		case EMPTY:
		{
			break;
		}
		}
	}

	void TickOver( GLvoid )
	{
		for ( GLuint iter = 0; iter < ( GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions ); iter++ )
		{
			GLuint column = iter % (GLuint)GOLsettingsBuffer->dimensions;

			GLuint row = ( iter / ( GLuint )GOLsettingsBuffer->dimensions );

			if ( cells[ iter ] == EMPTY )
			{
				continue;
			}

			GLuint neighborCount = 0;
			GLuint deadNeighborCount = 0;
			GLuint dimension = GOLsettingsBuffer->dimensions - 1;

			if ( iter < ( GLuint )GOLsettingsBuffer->dimensions )
			{
				if ( iter % ( GLuint )GOLsettingsBuffer->dimensions == 0 )
				{
					CheckNode( cells[ iter + GOLsettingsBuffer->dimensions ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + 1 ], neighborCount, deadNeighborCount );
				}

				else if ( iter % dimension == 0 )
				{
					CheckNode( cells[ ( iter + dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - 1 ], neighborCount, deadNeighborCount );
				}

				else
				{
					CheckNode( cells[ iter - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter + dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + GOLsettingsBuffer->dimensions ], neighborCount, deadNeighborCount );
				}

			}

			else if ( iter > ( ( ( GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions ) - GOLsettingsBuffer->dimensions ) - 1 )
				&& iter < ( ( GOLsettingsBuffer->dimensions* GOLsettingsBuffer->dimensions ) - 1 ) )
			{
				if ( ( iter % ( GLuint )GOLsettingsBuffer->dimensions ) == 0 )
				{
					CheckNode( cells[ iter - GOLsettingsBuffer->dimensions ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + 1 ], neighborCount, deadNeighborCount );
				}

				else if ( iter % dimension == 0 )
				{
					CheckNode( cells[ iter - dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter - dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - 1 ], neighborCount, deadNeighborCount );
				}

				else
				{
					CheckNode( cells[ iter - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter - dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - GOLsettingsBuffer->dimensions ], neighborCount, deadNeighborCount );
				}
			}

			else
			{
				if ( iter < ( GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions ) - 1 )
				{
					CheckNode( cells[ ( iter - dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter - dimension ) ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - GOLsettingsBuffer->dimensions ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter + dimension ) - 1 ], neighborCount, deadNeighborCount );

					CheckNode( cells[ iter + dimension ], neighborCount, deadNeighborCount );

					CheckNode( cells[ ( iter + dimension ) + 1 ], neighborCount, deadNeighborCount );

				}
			}

			if ( neighborCount < 2 && cells[ iter ] == ALIVE )
			{
				cells[ iter ] = DEAD;
			}

			else if ( neighborCount >= 2 && cells[ iter ] == ALIVE )
			{
				if ( neighborCount > 3 )
				{
					cells[ iter ] = DEAD;
				}

				else
				{
					cells[ iter ] = ALIVE;
				}
			}

			else if ( neighborCount == 3 && cells[ iter ] == DEAD )
			{
				cells[ iter ] = ALIVE;
			}
		}

	}

	void Update( GLdouble deltaTime )
	{
		if ( currentTickDelay < tickDelay )
		{
			currentTickDelay += deltaTime;
		}

		else
		{
			TickOver( );

			glBufferData(GL_SHADER_STORAGE_BUFFER, (GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions) * sizeof(GLuint), cells.data(), GL_DYNAMIC_DRAW);
			currentTickDelay = 0.0f;
		}
	}

	void SetupUniforms( GLvoid )
	{
		this->defaultSettingsHandle = glGetUniformBlockIndex( this->programGLID, "defaultSettings" );
		this->GOLsettingsHandle = glGetUniformBlockIndex( this->programGLID, "GOLsettings" );
		this->GOLstatusHandle = glGetProgramResourceIndex( this->programGLID, GL_SHADER_STORAGE_BLOCK, "GOLstatus" );
	}

	void GenerateBuffers( GLvoid )
	{
		SetupUniforms( );

		float cellWidth = defaultUniformBuffer->width / GOLsettingsBuffer->dimensions;
		float cellHeight = defaultUniformBuffer->height / GOLsettingsBuffer->dimensions;

		float cellQuad[ ] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			cellWidth, 0.0f, 1.0f, 1.0f,
			cellWidth, cellHeight, 1.0f, 1.0f,
			0.0f, cellHeight, 1.0f, 1.0f
		};

		glGenBuffers( 1, &vertexBuffer );
		glGenBuffers( 1, &defaultUniformBufferHandle );
		glGenBuffers( 1, &GOLsettingsBufferHandle );
		glGenBuffers( 1, &GOLstatusBufferHandle );

		glBindBuffer( GL_UNIFORM_BUFFER, defaultUniformBufferHandle );
		glBufferData( GL_UNIFORM_BUFFER, sizeof( defaultUniformBuffer_t ), defaultUniformBuffer, GL_STATIC_DRAW );
		glBindBufferBase( GL_UNIFORM_BUFFER, this->defaultSettingsHandle, this->defaultUniformBufferHandle );

		glBindBuffer( GL_UNIFORM_BUFFER, GOLsettingsBufferHandle );
		glBufferData( GL_UNIFORM_BUFFER, sizeof( GOL_SettingsBuffer_t ), GOLsettingsBuffer, GL_STATIC_DRAW );
		glBindBufferBase( GL_UNIFORM_BUFFER, this->GOLsettingsHandle, this->GOLsettingsBufferHandle );

		glBindBuffer( GL_SHADER_STORAGE_BUFFER, GOLstatusBufferHandle );
		glBufferData( GL_SHADER_STORAGE_BUFFER, ( GOLsettingsBuffer->dimensions * GOLsettingsBuffer->dimensions ) * sizeof( GLuint ), cells.data( ), GL_DYNAMIC_DRAW );
		glBindBufferBase( GL_SHADER_STORAGE_BUFFER, this->GOLstatusHandle, this->GOLstatusBufferHandle );

		glGenVertexArrays( 1, &vertexArray );

		glBindVertexArray( vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 16, cellQuad, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );

		glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, ( char* )0 );
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, sizeof(float) * 4);

		glBindVertexArray( vertexArray );

		glUseProgram( this->programGLID );
	}
};