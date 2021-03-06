#pragma once

class Shader
{
public:
   // constructor generates the shader on the fly
   // ------------------------------------------------------------------------
   Shader(const char* resourcePath, const char* vertexFileName, const char* fragmentFileName);
   Shader(const char* vertexPath, const char* fragmentPath);

   ~Shader();

   void GetUniforms();
   
   // activate the shader
   // ------------------------------------------------------------------------
   void Use() const;

   unsigned int GetID() const { return ID; }

   // MVP
   unsigned int loc_model_matrix;
   unsigned int loc_view_matrix;
   unsigned int loc_projection_matrix;

   // utility uniform functions
   void SetBool(const std::string &name, bool value) const;
   void SetInt(const std::string &name, int value) const;
   void SetFloat(const std::string &name, float value) const;
   void SetVec2(const std::string &name, const glm::vec2 &value) const;
   void SetVec2(const std::string &name, float x, float y) const;
   void SetVec3(const std::string &name, const glm::vec3 &value) const;
   void SetVec3(const std::string &name, float x, float y, float z) const;
   void GetVec3(const std::string &name, glm::vec3 &value) const;
   void SetVec4(const std::string &name, const glm::vec4 &value) const;
   void SetVec4(const std::string &name, float x, float y, float z, float w) const;
   void SetMat2(const std::string &name, const glm::mat2 &mat) const;
   void SetMat3(const std::string &name, const glm::mat3 &mat) const;
   void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
   void Init(const char* vertexPath, const char* fragmentPath);

   // utility function for checking shader compilation/linking errors.
   // ------------------------------------------------------------------------
   void CheckCompileErrors(unsigned int shader, std::string type);
private:
   unsigned int ID;
};

