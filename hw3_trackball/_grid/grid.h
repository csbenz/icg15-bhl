#pragma once
#include "icg_common.h"

//#define USE_WIREFRAME
#define GRID_SIZE 100

class Grid{
protected:
	GLuint _vao;          ///< vertex array object
	GLuint _vbo_position; ///< memory buffer for positions
	GLuint _vbo_index;    ///< memory buffer for indice
	GLuint _pid;          ///< GLSL shader program ID
	GLuint _tex;          ///< Texture ID
	GLuint _num_indices;  ///< number of vertices to render

public:    
	void init()
	{
		// Compile the shaders
		_pid = opengp::load_shaders("grid_vshader.glsl", "grid_fshader.glsl");
		if(!_pid) exit(EXIT_FAILURE);       
		glUseProgram(_pid);

		// Vertex one vertex Array
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Vertex coordinates and indices
		{
			std::vector<GLfloat> vertices;
			std::vector<GLuint> indices;
#if 1
			// TODO 5: Make a triangle grid with dimension 100x100.
			// Always two subsequent entries in 'vertices' form a 2D vertex position.
			const int grid_dim = GRID_SIZE;

			for (int height = 0; height <= grid_dim; ++height) {
				for (int width = 0; width <= grid_dim; ++width) {
					/* scaling width and height to fit on the canvas */
					const float x_vertex = (2.0 * ((float) width / (float) grid_dim)) - 1.0;
					const float y_vertex = (-2.0 * ((float) height / (float) grid_dim)) + 1.0;

					/* push them as vertices */
					vertices.push_back(x_vertex);
					vertices.push_back(y_vertex);
				}
			}

			/* forming triangles */
			for (int height = 0; height < grid_dim; ++height) {
				for (int width = 0; width < grid_dim; ++width) {
					/* getting the indices */
					const int topLeft_vertex = (height * (grid_dim + 1)) + width;
					const int topRight_vertex = topLeft_vertex + 1;
					const int bottomLeft_vertex = topLeft_vertex + (grid_dim + 1);
					const int bottomRight_vertex = bottomLeft_vertex + 1;

					/* pushing */
					indices.push_back(topLeft_vertex);
					indices.push_back(topRight_vertex);
					indices.push_back(bottomLeft_vertex);
					indices.push_back(topRight_vertex);
					indices.push_back(bottomRight_vertex);
					indices.push_back(bottomLeft_vertex);
				}
			}

#else
			// The given code below are the vertices for a simple quad.
			// Your grid should have the same dimension as that quad, i.e.,
			// reach from [-1, -1] to [1, 1].

			// Vertex position of the triangles.
			vertices.push_back(-1.0f); vertices.push_back( 1.0f);
			vertices.push_back( 1.0f); vertices.push_back( 1.0f);
			vertices.push_back( 1.0f); vertices.push_back(-1.0f);
			vertices.push_back(-1.0f); vertices.push_back(-1.0f);

			// And indices.
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(3);
			indices.push_back(2);
#endif
			_num_indices = indices.size();

			// position buffer
			glGenBuffers(1, &_vbo_position);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo_position);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

			// vertex indices
			glGenBuffers(1, &_vbo_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			// position shader attribute
			GLuint loc_position = glGetAttribLocation(_pid, "position");
			glEnableVertexAttribArray(loc_position);
			glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
		}

#ifndef USE_WIREFRAME
		// Load texture
		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);
		glfwLoadTexture2D("grid_texture.tga", 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Texture uniforms
		GLuint tex_id = glGetUniformLocation(_pid, "tex");
		glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
#endif

		// to avoid the current object being polluted
		glBindVertexArray(0);
	}

	void cleanup()
	{
		glDeleteBuffers(1, &_vbo_position);
		glDeleteBuffers(1, &_vbo_index);
		glDeleteVertexArrays(1, &_vao);
		glDeleteProgram(_pid);
		glDeleteTextures(1, &_tex);
	}

	void draw(const mat4& model, const mat4& view, const mat4& projection, float time)
	{
		glUseProgram(_pid);
		glBindVertexArray(_vao);
#ifndef USE_WIREFRAME
		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _tex);
#endif

		// Setup MVP
		mat4 MVP = projection*view*model;
		GLuint MVP_id = glGetUniformLocation(_pid, "mvp");
		glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());

		// Pass the current time stamp to the shader.
		glUniform1f(glGetUniformLocation(_pid, "time"), time);

		// Draw
#ifdef USE_WIREFRAME
		// TODO 5: For debugging it can be helpful to draw only the wireframe.
		// You can do that by uncommenting the next line.
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
		// TODO 5: Depending on how you set up your vertex index buffer, you
		// might have to change GL_TRIANGLE_STRIP to GL_TRIANGLES.
		glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);        
		glUseProgram(0);
	}
};