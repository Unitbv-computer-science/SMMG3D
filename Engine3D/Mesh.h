#pragma once

#include <vector>

struct Vertex
{
   // position
   glm::vec3 Position;
   // normal
   glm::vec3 Normal;
   // texCoords
   glm::vec2 TexCoords;
   // tangent
   glm::vec3 Tangent;
   // bitangent
   glm::vec3 Bitangent;
};

struct STexture
{
   unsigned int id;
   std::string type;
   std::string path;
};

class Shader;

class Mesh
{
public:
   /*  Functions  */
   // constructor
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
      std::vector<STexture> textures, bool bComputeNormals);

   Mesh(const size_t nVertices, const GLfloat pVertices[],
      const size_t nIndices, const unsigned int pIndices[]);

   // render the mesh
   void Draw(const Shader& shader) const;
private:
   /*  Render data  */
   unsigned int VBO, EBO;

   /*  Functions    */
   void Init(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
      std::vector<STexture> textures, bool bComputeNormals);

   // initializes all the buffer objects/arrays
   void SetupMesh();

   void ComputeNormals();

private:
   /*  Mesh Data  */
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<STexture> textures;
   unsigned int VAO;
};

