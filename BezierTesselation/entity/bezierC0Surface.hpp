#pragma once
#include <memory>
#include <vector>
#include <vector/vector.hpp>
#include "mesh\mesh.hpp"

class ShaderManager;

class BezierC0Surface 
{
	std::shared_ptr<Mesh> mesh, polynomialMesh;
	std::vector<Vector4<float>> points;

	int patchesU;
	int patchesV;

	bool drawPolynomial;

	int subdivU;
	int subdivV;

	void setPolynomialMesh(const std::vector<float>& vectices, int sizeX, int sizeZ);
public:
	BezierC0Surface(const std::vector<Vector4<float>>& points, int patchesX, int patchesZ);
	~BezierC0Surface();
	virtual void updateMesh();
	virtual void draw(const ShaderManager& shaderManager);
};