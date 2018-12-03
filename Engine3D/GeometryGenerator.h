#pragma once

#include "Mesh.h"
#include "PerlinNoise.h"

#define MinElevation -10.f
#define MaxElevation 30.f

class GeometryGenerator
{
public:
   enum EElevationType
   {
      EFlat,
      EHill,
      EPerlinNoise
   };

public:
   GeometryGenerator();
   ~GeometryGenerator();

   Mesh CreateBox(float width, float height, float depth, unsigned int numSubdivisions);
   Mesh CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount);
   
   Mesh CreateGrid(float width, float depth, unsigned int m, unsigned int n);
   Mesh CreateHill(float width, float depth, unsigned int m, unsigned int n);
   Mesh CreatePerlinHill(float width, float depth, unsigned int m, unsigned int n);

   Mesh CreateQuad(float x, float y, float w, float h, float depth);
private:
   float GetlElevation(EElevationType elevationType, double x, double y, double z,
      float minElevation = MinElevation, float maxElevation = MaxElevation) const;

   glm::vec4 GetColor(float elevation) const;
   glm::vec3 GetHillsNormal(float x, float z) const;
   float GetHillsHeight(float x, float z) const;

   double Lerp(double t, double a, double b) const;

   void Subdivide(Mesh& meshData);
   Vertex MidPoint(const Vertex& v0, const Vertex& v1);

private:
   std::unique_ptr<PerlinNoise> _spPerlinNoise;
};

