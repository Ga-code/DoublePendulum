#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class shader
{
    public:
        unsigned int ID;
        shader(const char* vertexPath, const char* fragmentPath)
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vshaderStream, fshaderStream;
                vshaderStream << vShaderFile.rdbuf();
                fshaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vshaderStream.str();
                fragmentCode = fshaderStream.str();
            }
            catch(std::ifstream::failure e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(vertex);
            glCompileShader(fragment);
            ID = glCreateProgram();
            glAttachShader(ID,vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            glDeleteShader(vertex);
            glDeleteShader(fragment);


        }
        void use()
        {
            glUseProgram(ID);
        }
        void setBool(const std::string &name, bool value) const 
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const 
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const 
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setMatrix4(const std::string &name, glm::mat4 value)
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
};
#endif