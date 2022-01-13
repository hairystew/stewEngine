#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "simplexNoise.h"
#include "gameobject.h"
#include "world.h"
#include "model.h"
#include "glm/glm.hpp"

#define CHUNK_SIZE 32
#define RENDER_SIZE 12

class Chunk : public  GameObject
{
public:

	Chunk() : GameObject() {
		gameType = GameChunk;
		noise = SimplexNoise();
	}
	~Chunk() {
		//vertices.clear();
	}
	//std::unique_ptr<World> world = nullptr;
	std::string path;
	uint8_t chunkData[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]{};
	void createModel(Device& device);
	glm::ivec3 chunkLoc{};
	bool isEmpty = false;

	//some wack copy constructor shit
	Chunk(const Chunk&) = delete;
	Chunk& operator=(const Chunk&) = delete;
	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;
	Model::Builder chunkModelData;
	std::size_t hash;
private:
	void loadChunkData(std::ifstream& file);
	void writeChunkData();
	void generateChunkData();
	float getSimplexNoise(glm::vec3 worldPos);
	void createMesh();
	SimplexNoise noise;
	std::vector<Model::Vertex> vertices;
	const glm::vec3 indi[8] = {
		glm::vec3(0,0,0), //0
		glm::vec3(1,0,0), //1
		glm::vec3(0,1,0), //2
		glm::vec3(1,1,0), //3
		glm::vec3(0,0,1), //4
		glm::vec3(1,0,1), //5
		glm::vec3(0,1,1), //6
		glm::vec3(1,1,1), //7
	};

	const glm::vec2 texIndi[8] = {
		glm::vec2(0,0), //top left				(0)
		glm::vec2(0,1), //bottom left			(1)
		glm::vec2(1.f / 3,0), // top third		(2)
		glm::vec2(1.f / 3,1), // bottom third	(3)
		glm::vec2(2.f / 3,0), // top 2 third	(4)
		glm::vec2(2.f / 3,1), // bottom 2 third (5)
		glm::vec2(1,0), // top right			(6)
		glm::vec2(1,1), // bottom right			(7)
	};

};

