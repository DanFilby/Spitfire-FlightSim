#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iomanip>  // setprecision()
#include <iostream>
#include <string>
#include <vector>
#include<sstream>

#include "PerlinNoise.hpp"

namespace Utilities {
	inline void ClearVAO(GLuint vaoId) {
		glBindVertexArray(vaoId);
		GLint nAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);

		for (int i = 0; i < nAttributes; ++i) {
			GLint vboId = 0;
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
			if (vboId > 0) {
				GLuint uvboId = vboId;
				glDeleteBuffers(1, &uvboId);
			}
		}
		glDeleteVertexArrays(1, &vaoId);
	};

	inline double* GeneratePerlinNoise(int width, float zoom) {

		double* result = new double[width * width];

		double x = 0.0, y = 0.0;

		//value from 0.01 to 0.05 
		double increment = 0.01 + 0.05 * (1 - (zoom - floor(zoom)));

		siv::PerlinNoise::seed_type seed = 1000u;
		siv::PerlinNoise perlin{ seed };

		int index = 0;

		for (size_t i = 0; i < width; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				result[index++] = perlin.noise2D_01(x, y);

				x += increment;
			}
			y += increment;
			x = 0.0;
		}
	}

	inline glm::vec3 CaclulateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		// Uses p2 as a new origin for p1,p3
		glm::vec3 a = p3 - p2;
		glm::vec3 b = p1 - p2;
		glm::vec3 vec = glm::normalize(glm::cross(a, b));
		vec.y = (vec.y + 1.0) / 2.0f;
		return vec;
	}

	inline std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> verts, std::vector<unsigned int> tris) {
		//
		std::vector<glm::vec3> normals = std::vector<glm::vec3>(verts.size());

		for (unsigned int i = 0; i < tris.size(); i += 3) {
			glm::vec3 A = verts[tris[i]];
			glm::vec3 B = verts[tris[i + 1LL]];
			glm::vec3 C = verts[tris[i + 2LL]];
			glm::vec3 normal = CaclulateNormal(A, B, C);
			normals[tris[i]] += normal;
			normals[tris[i + 1LL]] += normal;
			normals[tris[i + 2LL]] += normal;
		}
		// Normalize each normal
		for (unsigned int i = 0; i < normals.size(); i++) {
			normals[i] = glm::normalize(normals[i]);

		}

		return normals;
	}

	template<class InputIterator>
	inline void WriteDat(const char* filename, InputIterator begin, InputIterator end, int precision) {
		std::ofstream f;
		f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		f.open(filename);

		for (; begin != end; ++begin) {
			f << std::setprecision(precision) << *begin << '\t';
		}

	}

	inline void ReadDat(const char* filename ) {
		std::ifstream terrainFile;
		terrainFile.open(filename);

		std::stringstream ss{};
		ss << terrainFile.rdbuf();

		std::vector<float> result;

		while (ss.good())
		{
			std::string substr;
			getline(ss, substr, ',');
			if(!substr.empty())	result.push_back(std::stof(substr));
		}

	}

	inline void GetFloats (const std::vector<glm::vec3> vectors,std::vector<float>& floats) {
		floats = std::vector<float>();

		for (glm::vec3 vec : vectors)
		{
			floats.push_back(vec.x);
			floats.push_back(vec.y);
			floats.push_back(vec.z);
		}
	}

	inline glm::vec3 PointOnSphere(float magnitude)
	{
		std::random_device rd;
		std::mt19937 generator(rd());
		std::normal_distribution<double> distribution(0.0, 5.0);

		glm::vec3 randomPoint = glm::vec3(distribution(generator), distribution(generator), distribution(generator));
		randomPoint = glm::normalize(randomPoint);
		return randomPoint * magnitude;
	}

	inline void PrintVector(glm::vec3 vec)
	{
		printf("%f,%f,%f\n", vec.x, vec.y, vec.z);
	}

	//void GeneratePerlinNoiseImg(int width, float zoom) {

	//	double x = 0.0, y = 0.0;

	//	//value from 0.01 to 0.05 
	//	double increment = 0.01 + 0.05 * (1 - (zoom - floor(zoom)));

	//	siv::PerlinNoise::seed_type seed = 548u;
	//	siv::PerlinNoise perlin{ seed };

	//	uint8_t* pixels = new uint8_t[width * width * 3];
	//	int index = 0;

	//	for (size_t i = 0; i < width; i++)
	//	{
	//		for (size_t j = 0; j < width; j++)
	//		{
	//			uint8_t colour = perlin.noise2D_01(x, y) * 255.0;

	//			pixels[index++] = colour;
	//			pixels[index++] = colour;
	//			pixels[index++] = colour;

	//			x += increment;
	//		}
	//		y += increment;
	//		x = 0.0;
	//	}

	//	//TODO:fix this line
	//	//stbi_write_png("Generated/noise.png", width, width, 3, pixels, width * 3);

	//	delete[] pixels;
	//}

}
