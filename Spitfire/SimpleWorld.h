#pragma once
#include"Utilities.hpp"
#include"LoadShaders.hpp"
#include "Lighting.h"
#include <vector>
#include <unordered_map>
#include <sstream>

#include"btBulletDynamicsCommon.h"

/*
* To create a new terrain use this function to add a new configuration to the file
* SimpleWorld::CreateNewTerrainConfig("Huge", 1000, 1, -1);
*/


//Different sky boxes
enum SkySetting { Noon, Cloudy, Sunset, Dusk, None };

const std::unordered_map<SkySetting, std::string> SkyboxFolders{
	{SkySetting::Noon, "Noon"}, {SkySetting::Cloudy, "Cloudy"},
	{SkySetting::Sunset, "Sunset"}, {SkySetting::Dusk, "Dusk"} };

class SimpleWorld
{
public:
	SimpleWorld(bool createGround, SkySetting sky, LightNode* _lightNodes);
	SimpleWorld(std::string terrainName, SkySetting sky, LightNode* _lightNodes);
	~SimpleWorld();

	static void CreateNewTerrainConfig(std::string terrainName, float width, float vertDensity, int maxVerts);

	void DrawWorld(const glm::mat4& projection, const glm::mat4& view);

private:
	void DrawSky(const glm::mat4& projection, const glm::mat4& view);
	void DrawTerrain(const glm::mat4& projection, const glm::mat4& view);

	void GenerateTerrain(std::string terrainConfigName);
	void FillVerts(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& tris, float width, float vertDensity, int maxVerts);

	void CalculateVertSpacing(int maxVerts, float width, float& vertSpacing);
	void GetTerrainConfig(std::string name, float& width, float& density, int& maxVerts);

	void GenerateSkyBox();		
	unsigned int LoadCubeMap(std::vector<std::string> faces);

	//the bot texture doesnt work so just using top one
	std::vector<std::string> SkyMapFiles = { "/Right.bmp", "/Left.bmp", "/Top.bmp", "/top.bmp", "/Front.bmp", "/Back.bmp" };

	int groundShaderProgam{};
	int skyShaderProgram{};

	GLuint terrainVAO{};
	GLuint terrainElementBufId{};
	int triCount{};

	GLuint skyBoxVAO{};
	GLuint skyTextures{};

	LightNode* lightNodes{};
};

