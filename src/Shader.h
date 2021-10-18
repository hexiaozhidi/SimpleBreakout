#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader {
public:
    Shader();

    Shader(const char *vShaderCode, const char *fShaderCode, const char *gShaderCode = nullptr);

    const Shader &use() const;

    void clear() const;

    const Shader &setBool(const std::string &name, const bool &value) const;
    const Shader &setBool(const std::string &name, const std::vector<bool> &values) const;

    const Shader &setInt(const std::string &name, const int &value) const;
    const Shader &setInt(const std::string &name, const std::vector<int> &values) const;

    const Shader &setFloat(const std::string &name, const float &value) const;
    const Shader &setFloat(const std::string &name, const std::vector<float> &values) const;

    const Shader &setVec2(const std::string &name, const glm::vec2 &value) const;
    const Shader &setVec2(const std::string &name, const float &x, const float &y) const;
    const Shader &setVec2(const std::string &name, const std::vector<glm::vec2> &values) const;

    const Shader &setVec3(const std::string &name, const glm::vec3 &value) const;
    const Shader &setVec3(const std::string &name, const float &x, const float &y, const float &z) const;
    const Shader &setVec3(const std::string &name, const std::vector<glm::vec3> &values) const;

    const Shader &setVec4(const std::string &name, const glm::vec4 &value) const;
    const Shader &setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w) const;
    const Shader &setVec4(const std::string &name, const std::vector<glm::vec4> &values) const;

    const Shader &setMat2(const std::string &name, const glm::mat2 &value) const;
    const Shader &setMat2(const std::string &name, const std::vector<glm::mat2> &values) const;

    const Shader &setMat3(const std::string &name, const glm::mat3 &value) const;
    const Shader &setMat3(const std::string &name, const std::vector<glm::mat3> &values) const;

    const Shader &setMat4(const std::string &name, const glm::mat4 &value) const;
    const Shader &setMat4(const std::string &name, const std::vector<glm::mat4> &values) const;

private:
    unsigned int id;

    void checkCompileErrors(const unsigned int &id, const std::string &type) const;
};

#endif