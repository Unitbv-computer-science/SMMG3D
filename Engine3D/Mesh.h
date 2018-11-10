#pragma once

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

struct Texture
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
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

   // render the mesh
   void Draw(const Shader& shader) const;
private:
   /*  Render data  */
   unsigned int VBO, EBO;

   /*  Functions    */
   // initializes all the buffer objects/arrays
   void SetupMesh();

private:
   /*  Mesh Data  */
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<Texture> textures;
   unsigned int VAO;
};

