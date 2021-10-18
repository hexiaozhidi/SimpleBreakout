#include "Shader.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : id(0) { }

Shader::Shader(const char *vShaderCode, const char *fShaderCode, const char *gShaderCode) : id(0) {
    id = glCreateProgram();
    unsigned int vertexShader, fragmentShader, geometryShader;

    // compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // compile the geometry shader
    if (gShaderCode) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &gShaderCode, nullptr);
        glCompileShader(geometryShader);
        checkCompileErrors(geometryShader, "GEOMETRY");
    }

    // link the shader program
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    if (gShaderCode)
        glAttachShader(id, geometryShader);

    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (gShaderCode)
        glDeleteShader(geometryShader);
}

const Shader &Shader::use() const {
    glUseProgram(id);
    return *this;
}

void Shader::clear() const {
    glDeleteProgram(id);
}

const Shader &Shader::setBool(const std::string &name, const bool &value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
    return *this;
}

const Shader &Shader::setBool(const std::string &name, const std::vector<bool> &values) const {
    std::vector<int> intValues(values.begin(), values.end());
    glUniform1iv(glGetUniformLocation(id, name.c_str()), intValues.size(), &intValues[0]);
    return *this;
}

const Shader &Shader::setInt(const std::string &name, const int &value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    return *this;
}

const Shader &Shader::setInt(const std::string &name, const std::vector<int> &values) const {
    glUniform1iv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0]);
    return *this;
}

const Shader &Shader::setFloat(const std::string &name, const float &value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    return *this;
}

const Shader &Shader::setFloat(const std::string &name, const std::vector<float> &values) const {
    glUniform1fv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0]);
    return *this;
}

const Shader &Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    return *this;
}

const Shader &Shader::setVec2(const std::string &name, const float &x, const float &y) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    return *this;
}

const Shader &Shader::setVec2(const std::string &name, const std::vector<glm::vec2> &values) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0][0]);
    return *this;
}

const Shader &Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    return *this;
}

const Shader &Shader::setVec3(const std::string &name, const float &x, const float &y, const float &z) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    return *this;
}

const Shader &Shader::setVec3(const std::string &name, const std::vector<glm::vec3> &values) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0][0]);
    return *this;
}

const Shader &Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    return *this;
}

const Shader &Shader::setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    return *this;
}

const Shader &Shader::setVec4(const std::string &name, const std::vector<glm::vec4> &values) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), values.size(), &values[0][0]);
    return *this;
}

const Shader &Shader::setMat2(const std::string &name, const glm::mat2 &value) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    return *this;
}

const Shader &Shader::setMat2(const std::string &name, const std::vector<glm::mat2> &values) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), values.size(), GL_FALSE, &values[0][0][0]);
    return *this;
}

const Shader &Shader::setMat3(const std::string &name, const glm::mat3 &value) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    return *this;
}

const Shader &Shader::setMat3(const std::string &name, const std::vector<glm::mat3> &values) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), values.size(), GL_FALSE, &values[0][0][0]);
    return *this;
}

const Shader &Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    return *this;
}

const Shader &Shader::setMat4(const std::string &name, const std::vector<glm::mat4> &values) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), values.size(), GL_FALSE, &values[0][0][0]);
    return *this;
}

void Shader::checkCompileErrors(const unsigned int &id, const std::string &type) const {
    int success;
    char infoLog[1024];
    if (type == "VERTEX") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "ERROR£ºFailed to compile the vertex shader." << std::endl;
            std::cout << infoLog << std::endl;
        }
    } else if (type == "FRAGMENT") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "ERROR£ºFailed to compile the fragment shader." << std::endl;
            std::cout << infoLog << std::endl;
        }
    } else if (type == "GEOMETRY") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "ERROR£ºFailed to compile the geometry shader." << std::endl;
            std::cout << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "ERROR£ºFailed to link the shader program." << std::endl;
            std::cout << infoLog << std::endl;
        }
    }
}