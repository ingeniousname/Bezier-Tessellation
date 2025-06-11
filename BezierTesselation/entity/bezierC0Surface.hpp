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

	int subdivOuter;
	int subdivInner;

	void setPolynomialMesh(const std::vector<float>& vectices, int sizeX, int sizeZ);
public:
	BezierC0Surface(const std::vector<Vector4<float>>& points, int patchesX, int patchesZ);
	~BezierC0Surface();
	virtual void updateMesh();
	void ShareParameterUI();
	void changeOuterSubdiv(int delta);
	void changeInnerSubdiv(int delta);
	virtual void draw(const ShaderManager& shaderManager);
};