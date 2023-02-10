#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// Shader program ID
	unsigned int ID;

	// class constructor reads and builds the shader
	Shader(const char* vertexSourcePath, const char* fragmentSourcePath)
	{
	    /*
	     *  Retrieve the vertex shader and fragment shader source code from filePath
	     */
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vtxShaderFile;
		std::ifstream fragShaderFile;
		// ensure ifstream objects can throw exceptions
		vtxShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vtxShaderFile.open(vertexSourcePath);
			fragShaderFile.open(fragmentSourcePath);
			std::stringstream vtxShaderStream, fragShaderStream;
			// read file's buffer contents into streams
			vtxShaderStream << vtxShaderFile.rdbuf();
			fragShaderStream << fragShaderFile.rdbuf();
			// close file handlers
			vtxShaderFile.close();
			fragShaderFile.close();
			// convert stream into string
			vertexCode = vtxShaderStream.str();
			fragmentCode = fragShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::SOURCE_FILE_READ_FAILED" << std::endl;
		}
		const char* vtxShaderCode = vertexCode.c_str();
		const char* fragShaderCode = fragmentCode.c_str();
     
		/*
	     *  Compile Shaders
	     */
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// compile vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vtxShaderCode, NULL);
		glCompileShader(vertex);
		// check for errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// compile fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragShaderCode, NULL);
		glCompileShader(fragment);
		// check for errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		/*
		 *  Shader Program
		 */
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// check for linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		/*
		 *  Delete shaders 
		 */
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// activate the shader
	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat2(const std::string& name, float value1, float value2) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
	}

	void setFloat3(const std::string& name, float value1, float value2, float value3) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
	}

	void setFloat4(const std::string& name, float value1, float value2, float value3, float value4) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
	}

	void setMat4(const std::string& name, glm::mat4 matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void destroy()
	{
		glDeleteProgram(ID);
	}
};

#endif