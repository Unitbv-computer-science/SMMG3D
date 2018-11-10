#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

class InputController;
class Camera;
class CameraInput;
class Mesh;
class Shader;

class SceneManager
{
public:
   SceneManager(int width, int height);

   virtual Camera* GetSceneCamera() const final;
   virtual InputController* GetCameraInput() const final;

   void AddMesh(const char* meshName, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
   void AddShader(const char* shaderName, const char* vertexPath, const char* fragmentPath);

   const Mesh* GetMesh(const char* meshName) const;

   void RenderMesh(const Mesh& mesh, glm::vec3 position, glm::vec3 scale);
   void RenderMesh(const Mesh& mesh, const Shader& shader, const glm::vec3& position, const glm::vec3& scale);
   void RenderMesh(const Mesh& mesh, const Shader& shader, const glm::mat4 & modelMatrix);

private:
   void InitResources();

private:
   std::unique_ptr<Camera> spCamera;
   std::unique_ptr<CameraInput> spCameraInput;

   std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
   std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};

