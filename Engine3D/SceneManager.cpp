#include "stdafx.h"
#include "SceneManager.h"

#include "CameraInput.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

SceneManager::SceneManager(int width, int height)
{
   const float FoVy = 45.f;
   const float zNear = 0.1f;
   const float zFar = 500.f;
   const glm::vec3 position(0, 0, 0);
   const glm::vec3 worldUp(0, 1, 0);
   // Create camera
   spCamera.reset(new Camera(FoVy, (float)width, (float)height, zNear, zFar, position, worldUp));

   // Init camera input with camera
   spCameraInput.reset(new CameraInput(spCamera.get()));

   InitResources();
}

void SceneManager::InitResources()
{
   // Create a shader program for drawing face polygon with the color
   {
      std::string vertexPath   = RESOURCE_PATH::SHADERS + "MVP.VS.glsl";
      std::string fragmentPath = RESOURCE_PATH::SHADERS + "Simplest.FS.glsl";

      shaders["Simple"].reset(new Shader(vertexPath.c_str(), fragmentPath.c_str()));
   }
}


Camera* SceneManager::GetSceneCamera() const
{
   return spCamera.get();
}
InputController* SceneManager::GetCameraInput() const
{
   return spCameraInput.get();
}

void SceneManager::AddMesh(const char* meshName, std::vector<glm::vec3> positions, std::vector<unsigned int> indices)
{
   std::vector<Vertex> vertices;
   for (std::vector<glm::vec3>::iterator it = positions.begin(); it != positions.end(); it++) {
      Vertex vertex;
      vertex.Position = *it;
      vertices.push_back(vertex);
   }
   meshes[meshName].reset(new Mesh(vertices, indices, std::vector<STexture>(), true));
}

void SceneManager::AddShader(const char* shaderName, const char* vertexPath, const char* fragmentPath)
{
   shaders[shaderName].reset( new Shader(vertexPath, fragmentPath) );
}

const Mesh* SceneManager::GetMesh(const char* meshName) const
{
   std::unordered_map<std::string, std::unique_ptr<Mesh>>::const_iterator it = meshes.find(meshName);

   if (it == meshes.end()) {
      return nullptr;
   }
   
   return it->second.get();
}

void SceneManager::RenderMesh(const Mesh& mesh, glm::vec3 position, glm::vec3 scale)
{
   RenderMesh(mesh, *shaders["Simple"].get(), position, scale);
}

void SceneManager::RenderMesh(const Mesh& mesh, const Shader& shader, 
                              const glm::vec3& position, const glm::vec3& scale)
{
   glm::mat4 modelMatrix(1);
   modelMatrix = glm::translate(modelMatrix, position);
   modelMatrix = glm::scale(modelMatrix, scale);

   RenderMesh(mesh, shader, modelMatrix);
}

void SceneManager::RenderMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& modelMatrix)
{
   if (!shader.GetID()) {
      return;
   }

   // render an object using the specified shader and the specified model transformation
   shader.Use();
   glUniformMatrix4fv(shader.loc_view_matrix, 1, GL_FALSE, glm::value_ptr(spCamera->GetViewMatrix()));
   glUniformMatrix4fv(shader.loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(spCamera->GetProjectionMatrix()));
   glUniformMatrix4fv(shader.loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
   
   mesh.Draw(shader);
}