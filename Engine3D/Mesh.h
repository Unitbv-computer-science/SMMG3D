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

   // color
   glm::vec4 Color;

   Vertex() {}
   Vertex(
      float px, float py, float pz,
      float nx, float ny, float nz,
      float tx, float ty, float tz,
      float u, float v) :
      Position(px, py, pz),
      Normal(nx, ny, nz),
      Tangent(tx, ty, tz),
      Bitangent(tx, ty, tz),
      Color(0.0, 0.0, 0.0, 1.0),
      TexCoords(u, v) {}
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
private:
   friend class GeometryGenerator;
   friend class TerrainGrid;
   Mesh();

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

