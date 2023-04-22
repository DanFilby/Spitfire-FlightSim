#include "SimpleWorld.h"
#include "PerlinNoise.hpp"
#include "stb_image.h"

//creates the world witht the default ground and given sky
SimpleWorld::SimpleWorld(bool createGround, SkySetting sky, LightNode* _lightNodes)
{
	if (createGround) {
		groundShaderProgam = LoadShaders("GroundVert.glsl", "GroundFrag.glsl");
		GenerateTerrain("Default");
	}
	if (sky != SkySetting::None) {
		skyShaderProgram = LoadShaders("SkyVert.glsl", "SkyFrag.glsl");

		for (std::string & file : SkyMapFiles) {
			file = "../Assets/SkyBoxes/" + SkyboxFolders.at(sky) + file;
		}

		GenerateSkyBox();
	}

	lightNodes = _lightNodes;

}

//creates the world with a previously created ground to minimise startup time
SimpleWorld::SimpleWorld(std::string terrainName, SkySetting sky, LightNode* _lightNodes)
{
	groundShaderProgam = LoadShaders("GroundVert.glsl", "GroundFrag.glsl");
	GenerateTerrain(terrainName);
	
	if (sky != SkySetting::None) {
		skyShaderProgram = LoadShaders("SkyVert.glsl", "SkyFrag.glsl");

		for (std::string& file : SkyMapFiles) {
			file = "../Assets/SkyBoxes/" + SkyboxFolders.at(sky) + file;
		}

		GenerateSkyBox();
	}

	lightNodes = _lightNodes;


}

SimpleWorld::~SimpleWorld()
{
	Utilities::ClearVAO(terrainVAO);
}


void SimpleWorld::CreateNewTerrainConfig(std::string terrainName, float width, float vertDensity, int maxVerts)
{
	std::string fileLoc = "Generated/TerrainConfigs.dat";

	std::stringstream oldTerrainConfigs = std::stringstream();
	std::stringstream terrainConfigs = std::stringstream();

	std::ifstream configFile;
	configFile.open(fileLoc);
	oldTerrainConfigs << configFile.rdbuf();
	configFile.close();

	//TODO: extract this to function that allows for new variables to be added to terrain gen
	//remove an old config if same name
	while (!oldTerrainConfigs.eof()) {
		std::string currName;
		float currWidth, currDensity;
		int currMaxVerts;

		//read the values
		oldTerrainConfigs >> currName >> currWidth >> currDensity >> currMaxVerts;

		//add to the new config file if not the same name as given
		if (currName != terrainName && !currName.empty()) {
			terrainConfigs << currName << '\t' << currWidth << '\t' << currDensity << '\t' << currMaxVerts << '\n';
		}	
	}

	//add new config to file
	terrainConfigs << terrainName << '\t' << width << '\t' << vertDensity << '\t' << maxVerts << '\n';

	std::ofstream outfile;
	outfile.open(fileLoc, std::ios::out);
	outfile << terrainConfigs.str();
	outfile.close();
}


void SimpleWorld::DrawWorld(const glm::mat4& projection, const glm::mat4& view)
{
	DrawSky(projection, view);
	DrawTerrain(projection,view);
}

void SimpleWorld::DrawTerrain(const glm::mat4& projection, const glm::mat4& view)
{
	glUseProgram(groundShaderProgam);

	glm::mat4 model = glm::mat4(1);
	glm::mat4 mvp = projection * view * model;

	glUniformMatrix4fv(glGetUniformLocation(groundShaderProgam, "MVP"), 1, GL_FALSE, &mvp[0][0]);

	glUniform3fv(glGetUniformLocation(groundShaderProgam, "lightColour"), 1, &lightNodes[0].lightColour[0]);

	glUniform1f(glGetUniformLocation(groundShaderProgam, "lightPower"), lightNodes[0].lightPower);

	glUniform3fv(glGetUniformLocation(groundShaderProgam, "LightPosition_worldspace"), 1, &lightNodes[0].lightPosition[0]);

	glBindVertexArray(terrainVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainElementBufId);
	glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}


void SimpleWorld::DrawSky(const glm::mat4& projection, const glm::mat4& view)
{
	glm::mat4 view2 = glm::mat4(glm::mat3(view));

	glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_LEQUAL);

	glUseProgram(skyShaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(skyShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyShaderProgram, "view"), 1, GL_FALSE, &view2[0][0]);

	glBindVertexArray(skyBoxVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTextures);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//glDepthMask(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void SimpleWorld::GenerateTerrain(std::string terrainConfigName)
{
	float width, vertDensity;
	int maxVerts;

	GetTerrainConfig(terrainConfigName, width, vertDensity, maxVerts);

	std::vector<glm::vec3> vertices{};
	std::vector<unsigned int> tris{};
	
	float radius = width / 2.0f;
	
	FillVerts(vertices, tris, width, vertDensity, maxVerts);
	std::vector<glm::vec3> normals = Utilities::CalculateNormals(vertices, tris);

	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &terrainElementBufId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainElementBufId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(unsigned int), &tris[0], GL_STATIC_DRAW);

}

void SimpleWorld::FillVerts(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& tris, float width, float vertDensity, int maxVerts)
{
	siv::PerlinNoise::seed_type seed = 1000u;
	siv::PerlinNoise perlin{ seed };

	//how far apart each vertex will be
	float vertSpacing = 1.0f / vertDensity;

	//check that the total vert count is within the limit, if not calculate a new one
	if (maxVerts > 0 && ((width / vertSpacing + 1) * width / vertSpacing) > maxVerts) {
		//decrease density till vert count is low enough
		for (size_t density = vertDensity; density > 0; density--)
		{
			//calculate the vert count with the new density
			float vertCount = width * width * density * density + width * density;

			vertDensity = density;
			vertSpacing = 1.0f / vertDensity;

			if (vertCount <= maxVerts) {	
				break;
			}
		}
	}

	float xPos = 0, yPos = 0;

	float heightModifier = 5 * vertSpacing;

	//counter for how many verts will be in a row
	int vertsInRow = width / vertSpacing;

	for (int x = 0; x < (width / vertSpacing) + 1; x++)
	{
		for (int y = 0; y < (width / vertSpacing); y++)
		{
			float xDist = x / (float)vertsInRow;
			float yDist = y / (float)vertsInRow;

			//use perlin noise to create random height
			float height = -10 + 0.2 * perlin.noise2D_01(xDist * 20, yDist * 20) + 10 * perlin.noise2D_01(xDist * 4, yDist * 4);
			
			vertices.push_back(glm::vec3(vertSpacing * x - width / 2.0, height , vertSpacing * y - width / 2.0));
		}
	}

	//create square from two tris at each vertex except last row
	for (int i = 0; i < vertices.size() - vertsInRow - 1; i++)
	{
		if (i % vertsInRow == vertsInRow - 1) {
			continue;
		}

		tris.push_back(i);
		tris.push_back(i + 1); 
		tris.push_back(vertsInRow + i);

		tris.push_back(vertsInRow + i);
		tris.push_back(i + 1);
		tris.push_back(vertsInRow + i + 1);
	}

	triCount = tris.size();
}

//TODO: use this to get maximum amount of verts into terrain, gen function bugs if given a
//non whole number for vert density, this gives exact vert spacing but need to fix tri first
void SimpleWorld::CalculateVertSpacing(int maxVerts, float width, float& vertSpacing)
{
	if (maxVerts > 0 && ((width / vertSpacing + 1) * width / vertSpacing) > maxVerts) {
		float z = width * width;
		vertSpacing = (-width - sqrt(z + 4 * z * maxVerts)) / (-2 * maxVerts);
	}
}

void SimpleWorld::GetTerrainConfig(std::string name, float& width, float& density, int& maxVerts)
{
	std::string fileLoc = "Generated/TerrainConfigs.dat";

	std::stringstream terrainConfigs = std::stringstream();

	//open file
	std::ifstream configFile;
	configFile.open(fileLoc);

	//read file into string stream
	terrainConfigs << configFile.rdbuf();

	configFile.close();

	while (!terrainConfigs.eof()) {
		std::string currName;
		float currWidth, currDensity;
		int currMaxVerts;

		//read the values
		terrainConfigs >> currName >> currWidth >> currDensity >> currMaxVerts;

		//if the name matches assign the values 
		if (currName == name) {
			width = currWidth;
			density = currDensity;
			maxVerts = currMaxVerts;
			return;
		}
		//assign the values if its the default, will be overwritten if the name matches 
		else if (currName == "Default") {
			width = currWidth;
			density = currDensity;
			maxVerts = currMaxVerts;
		}
	}
}

void SimpleWorld::GenerateSkyBox()
{
	glGenVertexArrays(1, &skyBoxVAO);
	glBindVertexArray(skyBoxVAO);

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyTextures = LoadCubeMap(SkyMapFiles);
	glUniform1i(glGetUniformLocation(skyShaderProgram, "skybox"), 0);
}

unsigned int SimpleWorld::LoadCubeMap(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(false);

	glUseProgram(skyShaderProgram);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		GLenum format{};

		if (nrChannels == 3){
			format = GL_RGB;
		}
		else if (nrChannels == 4){
			format = GL_RGBA;
		}
		
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glUniform1i(glGetUniformLocation(skyShaderProgram, "skybox"), 0);


	return textureID;
}


