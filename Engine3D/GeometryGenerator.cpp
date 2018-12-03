#include "stdafx.h"
#include "GeometryGenerator.h"
#include "PerlinNoise.h"

#include <algorithm>
#include <time.h> 

GeometryGenerator::GeometryGenerator()
{
   srand((unsigned int)time(NULL));
   int seed = 1;
   _spPerlinNoise.reset( new PerlinNoise(seed));
}


GeometryGenerator::~GeometryGenerator()
{
}

Mesh GeometryGenerator::CreateBox(float width, float height, float depth, unsigned int numSubdivisions)
{
   Mesh meshData;

   //
   // Create the vertices.
   //

   Vertex v[24];

   float w2 = 0.5f*width;
   float h2 = 0.5f*height;
   float d2 = 0.5f*depth;

   // Fill in the front face vertex data.
   v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
   v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
   v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

   // Fill in the back face vertex data.
   v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
   v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
   v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

   // Fill in the top face vertex data.
   v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
   v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
   v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

   // Fill in the bottom face vertex data.
   v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
   v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
   v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

   // Fill in the left face vertex data.
   v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
   v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
   v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
   v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

   // Fill in the right face vertex data.
   v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
   v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
   v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
   v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

   meshData.vertices.assign(&v[0], &v[24]);

   //
   // Create the indices.
   //

   unsigned int i[36];

   // Fill in the front face index data
   i[0] = 0; i[1] = 1; i[2] = 2;
   i[3] = 0; i[4] = 2; i[5] = 3;

   // Fill in the back face index data
   i[6] = 4; i[7] = 5; i[8] = 6;
   i[9] = 4; i[10] = 6; i[11] = 7;

   // Fill in the top face index data
   i[12] = 8; i[13] = 9; i[14] = 10;
   i[15] = 8; i[16] = 10; i[17] = 11;

   // Fill in the bottom face index data
   i[18] = 12; i[19] = 13; i[20] = 14;
   i[21] = 12; i[22] = 14; i[23] = 15;

   // Fill in the left face index data
   i[24] = 16; i[25] = 17; i[26] = 18;
   i[27] = 16; i[28] = 18; i[29] = 19;

   // Fill in the right face index data
   i[30] = 20; i[31] = 21; i[32] = 22;
   i[33] = 20; i[34] = 22; i[35] = 23;

   meshData.indices.assign(&i[0], &i[36]);

   // Put a cap on the number of subdivisions.
   numSubdivisions = std::min<unsigned int>(numSubdivisions, 6u);

   for (unsigned int i = 0; i < numSubdivisions; ++i)
      Subdivide(meshData);

   meshData.SetupMesh();

   return meshData;
}

Mesh GeometryGenerator::CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount)
{
   Mesh meshData;

   //
   // Compute the vertices stating at the top pole and moving down the stacks.
   //

   // Poles: note that there will be texture coordinate distortion as there is
   // not a unique point on the texture map to assign to the pole when mapping
   // a rectangular texture onto a sphere.
   Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

   meshData.vertices.push_back(topVertex);

   float phiStep = XM_PI / stackCount;
   float thetaStep = 2.0f*XM_PI / sliceCount;

   // Compute vertices for each stack ring (do not count the poles as rings).
   for (unsigned int i = 1; i <= stackCount - 1; ++i)
   {
      float phi = i * phiStep;

      // vertices of ring.
      for (unsigned int j = 0; j <= sliceCount; ++j)
      {
         float theta = j * thetaStep;

         Vertex v;

         // spherical to cartesian
         v.Position.x = radius * sinf(phi)*cosf(theta);
         v.Position.y = radius * cosf(phi);
         v.Position.z = radius * sinf(phi)*sinf(theta);

         // Partial derivative of P with respect to theta
         v.Tangent.x = -radius * sinf(phi)*sinf(theta);
         v.Tangent.y = 0.0f;
         v.Tangent.z = +radius * sinf(phi)*cosf(theta);

         v.Tangent = glm::normalize(v.Tangent);
         v.Normal = glm::normalize(v.Position);

         v.TexCoords.x = theta / XM_2PI;
         v.TexCoords.y = phi / XM_PI;

         meshData.vertices.push_back(v);
      }
   }

   meshData.vertices.push_back(bottomVertex);

   //
   // Compute indices for top stack.  The top stack was written first to the vertex buffer
   // and connects the top pole to the first ring.
   //

   for (unsigned int i = 1; i <= sliceCount; ++i)
   {
      meshData.indices.push_back(0);
      meshData.indices.push_back(i + 1);
      meshData.indices.push_back(i);
   }

   //
   // Compute indices for inner stacks (not connected to poles).
   //

   // Offset the indices to the index of the first vertex in the first ring.
   // This is just skipping the top pole vertex.
   unsigned int baseIndex = 1;
   unsigned int ringVertexCount = sliceCount + 1;
   for (unsigned int i = 0; i < stackCount - 2; ++i)
   {
      for (unsigned int j = 0; j < sliceCount; ++j)
      {
         meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
         meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
         meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

         meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
         meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
         meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
      }
   }

   //
   // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
   // and connects the bottom pole to the bottom ring.
   //

   // South pole vertex was added last.
   unsigned int southPoleIndex = (unsigned int)meshData.vertices.size() - 1;

   // Offset the indices to the index of the first vertex in the last ring.
   baseIndex = southPoleIndex - ringVertexCount;

   for (unsigned int i = 0; i < sliceCount; ++i)
   {
      meshData.indices.push_back(southPoleIndex);
      meshData.indices.push_back(baseIndex + i);
      meshData.indices.push_back(baseIndex + i + 1);
   }

   meshData.ComputeNormals();
   meshData.SetupMesh();

   return meshData;
}

void GeometryGenerator::Subdivide(Mesh& meshData)
{
   // Save a copy of the input geometry.
   Mesh inputCopy = meshData;


   meshData.vertices.resize(0);
   meshData.indices.resize(0);

   //       v1
   //       *
   //      / \
   //     /   \
	//  m0*-----*m1
   //   / \   / \
	//  /   \ /   \
	// *-----*-----*
   // v0    m2     v2

   unsigned int numTris = (unsigned int)inputCopy.indices.size() / 3;
   for (unsigned int i = 0; i < numTris; ++i)
   {
      Vertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
      Vertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
      Vertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

      //
      // Generate the midpoints.
      //

      Vertex m0 = MidPoint(v0, v1);
      Vertex m1 = MidPoint(v1, v2);
      Vertex m2 = MidPoint(v0, v2);

      //
      // Add new geometry.
      //

      meshData.vertices.push_back(v0); // 0
      meshData.vertices.push_back(v1); // 1
      meshData.vertices.push_back(v2); // 2
      meshData.vertices.push_back(m0); // 3
      meshData.vertices.push_back(m1); // 4
      meshData.vertices.push_back(m2); // 5

      meshData.indices.push_back(i * 6 + 0);
      meshData.indices.push_back(i * 6 + 3);
      meshData.indices.push_back(i * 6 + 5);

      meshData.indices.push_back(i * 6 + 3);
      meshData.indices.push_back(i * 6 + 4);
      meshData.indices.push_back(i * 6 + 5);

      meshData.indices.push_back(i * 6 + 5);
      meshData.indices.push_back(i * 6 + 4);
      meshData.indices.push_back(i * 6 + 2);

      meshData.indices.push_back(i * 6 + 3);
      meshData.indices.push_back(i * 6 + 1);
      meshData.indices.push_back(i * 6 + 4);
   }
}

Vertex GeometryGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
{
   glm::vec3 p0 = v0.Position;
   glm::vec3 p1 = v1.Position;

   glm::vec3 n0 = v0.Normal;
   glm::vec3 n1 = v1.Normal;

   glm::vec3 tan0 = v0.Tangent;
   glm::vec3 tan1 = v1.Tangent;

   glm::vec2 tex0 = v0.TexCoords;
   glm::vec2 tex1 = v1.TexCoords;

   // Compute the midpoints of all the attributes.  Vectors need to be normalized
   // since linear interpolating can make them not unit length.
   Vertex v;
   v.Position = 0.5f*(p0 + p1);
   v.Normal = glm::normalize(0.5f*(n0 + n1));
   v.Tangent = glm::normalize(0.5f*(tan0 + tan1));
   v.TexCoords = 0.5f*(tex0 + tex1);

   return v;
}

Mesh GeometryGenerator::CreateGrid(float width, float depth, unsigned int m, unsigned int n)
{
   Mesh meshData;

   unsigned int vertexCount = m * n;
   unsigned int faceCount = (m - 1)*(n - 1) * 2;

   //
   // Create the vertices.
   //
   float halfWidth = 0.5f*width;
   float halfDepth = 0.5f*depth;

   float dx = width / (n - 1);
   float dz = depth / (m - 1);

   float du = 1.0f / (n - 1);
   float dv = 1.0f / (m - 1);

   meshData.vertices.resize(vertexCount);
   for (unsigned int i = 0; i < m; ++i)
   {
      float z = halfDepth - i * dz;
      for (unsigned int j = 0; j < n; ++j)
      {
         float x = -halfWidth + j * dx;

         meshData.vertices[i*n + j].Position = glm::vec3(x, 0.0f, z);
         meshData.vertices[i*n + j].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
         meshData.vertices[i*n + j].Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

         // Stretch texture over hillGrid.
         meshData.vertices[i*n + j].TexCoords.x = j * du;
         meshData.vertices[i*n + j].TexCoords.y = i * dv;
      }
   }

   //
   // Create the indices.
   //

   meshData.indices.resize(faceCount * 3); // 3 indices per face

   // Iterate over each quad and compute indices.
   unsigned int k = 0;
   for (unsigned int i = 0; i < m - 1; ++i)
   {
      for (unsigned int j = 0; j < n - 1; ++j)
      {
         meshData.indices[k] = i * n + j;
         meshData.indices[k + 1] = i * n + j + 1;
         meshData.indices[k + 2] = (i + 1)*n + j;

         meshData.indices[k + 3] = (i + 1)*n + j;
         meshData.indices[k + 4] = i * n + j + 1;
         meshData.indices[k + 5] = (i + 1)*n + j + 1;

         k += 6; // next quad
      }
   }

   meshData.SetupMesh();

   return meshData;
}

Mesh GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth)
{
   Mesh meshData;

   meshData.vertices.resize(4);
   meshData.indices.resize(6);

   // Position coordinates specified in NDC space.
   meshData.vertices[0] = Vertex(
      x, y - h, depth,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f);

   meshData.vertices[1] = Vertex(
      x, y, depth,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f);

   meshData.vertices[2] = Vertex(
      x + w, y, depth,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f);

   meshData.vertices[3] = Vertex(
      x + w, y - h, depth,
      0.0f, 0.0f, -1.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f);

   meshData.indices[0] = 0;
   meshData.indices[1] = 1;
   meshData.indices[2] = 2;

   meshData.indices[3] = 0;
   meshData.indices[4] = 2;
   meshData.indices[5] = 3;

   meshData.ComputeNormals();
   meshData.SetupMesh();

   return meshData;
}

Mesh GeometryGenerator::CreateHill(float width, float depth, unsigned int m, unsigned int n)
{
   Mesh hillGrid = CreateGrid(width, depth, m, n);
   for (size_t i = 0; i < hillGrid.vertices.size(); ++i)
   {
      hillGrid.vertices[i].Position.y = 
         GetHillsHeight(hillGrid.vertices[i].Position.x, hillGrid.vertices[i].Position.z);
      hillGrid.vertices[i].Normal = 
         GetHillsNormal(hillGrid.vertices[i].Position.x, hillGrid.vertices[i].Position.z);

      // TODO: check elevations
      hillGrid.vertices[i].Color = GetColor(hillGrid.vertices[i].Position.y);
   }

   hillGrid.SetupMesh();

   return hillGrid;
}

Mesh GeometryGenerator::CreatePerlinHill(float width, float depth, unsigned int m, unsigned int n)
{
   Mesh hillGrid = CreateGrid(width, depth, m, n);

   for (size_t i = 0; i < hillGrid.vertices.size(); ++i)
   {
      hillGrid.vertices[i].Position.y = GetlElevation(EElevationType::EPerlinNoise, hillGrid.vertices[i].Position.x,
         hillGrid.vertices[i].Position.y, hillGrid.vertices[i].Position.z);

      // TODO: Check elevations 
      hillGrid.vertices[i].Color = GetColor(hillGrid.vertices[i].Position.y);
   }

   hillGrid.ComputeNormals();
   hillGrid.SetupMesh();

   return hillGrid;
}

float GeometryGenerator::GetlElevation(EElevationType elevationType, double x, double y, double z,
   float minElevation, float maxElevation) const
{
   switch (elevationType) {
   case EFlat:
      return minElevation;
   case EHill:
      return (float)Lerp(GetHillsHeight((float)x, (float)z), minElevation, maxElevation);
   case EPerlinNoise:
      return (float)Lerp(_spPerlinNoise->Noise(x, y, z), minElevation, maxElevation);
   }

   return 0.0;
}

glm::vec4 GeometryGenerator::GetColor(float elevation) const
{
   glm::vec4 color;

   // Color the vertex based on its height.
   if (elevation < -10.0f)
   {
      // Sandy beach color.
      color = glm::vec4(1.0f, 0.96f, 0.62f, 1.0f);
   }
   else if (elevation < 5.0f)
   {
      // Light yellow-green.
      color = glm::vec4(0.48f, 0.77f, 0.46f, 1.0f);
   }
   else if (elevation < 12.0f)
   {
      // Dark yellow-green.
      color = glm::vec4(0.1f, 0.48f, 0.19f, 1.0f);
   }
   else if (elevation < 20.0f)
   {
      // Dark brown.
      color = glm::vec4(0.45f, 0.39f, 0.34f, 1.0f);
   }
   else
   {
      // White snow.
      color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
   }

   return color;
}

float GeometryGenerator::GetHillsHeight(float x, float z) const
{
   return 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
}

double GeometryGenerator::Lerp(double t, double a, double b) const
{
   return a + t * (b - a);
}

glm::vec3 GeometryGenerator::GetHillsNormal(float x, float z) const
{
   // n = (-df/dx, 1, -df/dz)
   glm::vec3 n(
      -0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
      1.0f,
      -0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));

   return glm::normalize(n);
}