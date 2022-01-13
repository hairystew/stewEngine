#include "chunk.h"

void Chunk::createModel(Device& device)
{
	std::hash<std::string> hashFunc;
	hash = hashFunc(std::to_string(chunkLoc.x) + std::to_string(chunkLoc.y) + std::to_string(chunkLoc.z));
	pos = (glm::vec3)chunkLoc * (float)CHUNK_SIZE;
	//path = "/worlds/" + world->fileName + "/" + (char)pos.x + "x" + (char)pos.y + "y" + (char)pos.z + "z.chunk";
	path = "../worlds/newWorld/chunks/" + std::to_string(chunkLoc.x) + "x" + std::to_string(chunkLoc.y) + "y" + std::to_string(chunkLoc.z) + "z.chunk";
	std::ifstream file;
	file.open(path, std::ios::out | std::ios::binary);
	if (!file){
		generateChunkData();
		writeChunkData();
		file.open(path, std::ios::out | std::ios::binary);
	}

	loadChunkData(file);
	createMesh();
	chunkModelData.buildModel(vertices);
	model = std::make_unique<Model>(device, chunkModelData);

	transformMatrix = glm::translate(glm::mat4{ 1.0 }, glm::vec3(chunkLoc.x, chunkLoc.y, chunkLoc.z)) * glm::scale(glm::mat4{ CHUNK_SIZE }, glm::vec3(1., 1., 1.));
}

void Chunk::loadChunkData(std::ifstream& file)
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				file.read((char*)&chunkData[i][j][k], sizeof(uint8_t));
			}
		}
	}
	file.close();
}

void Chunk::writeChunkData()
{
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (!file)
		std::cout << "SHIT BROKEY!\n";
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				file.write((char*)&chunkData[i][j][k], sizeof(uint8_t));
			}
		}
	}
	file.close();
}

void Chunk::generateChunkData()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
					 //int val = static_cast<uint8_t>(32 * ((noise.noise((chunkLoc.x + i), (chunkLoc.y + j), (chunkLoc.z + k)) + 1)/2));
					 //chunkData[i][j][k] = val;
					double val = noise.noise(((chunkLoc.x * (double)CHUNK_SIZE) + i) / 64., ((chunkLoc.y * (double)CHUNK_SIZE) + j) / 64., ((chunkLoc.z * (double)CHUNK_SIZE) + k) / 64.);
					chunkData[i][j][k] = (val > 0.3) ? 1 : 0;
			}
		}
	}
}

float Chunk::getSimplexNoise(glm::vec3 worldPos)
{



	return 0.0f;
}

void Chunk::createMesh()
{


	//take 3D array of block IDs

	//loop through array and add face vertices and tex coords to buffer if next to air
	vertices.clear();
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			for (int k = 0; k < CHUNK_SIZE; k++)
			{

				//check for air on -x face
				if ((i == 0 && chunkData[i][j][k] != 0) || (i != 0 && chunkData[i - 1][j][k] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3] });
					vertices.push_back(Model::Vertex{ indi[6] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4] });
					vertices.push_back(Model::Vertex{ indi[2] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2] });
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3] });
					vertices.push_back(Model::Vertex{ indi[4] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5] });
					vertices.push_back(Model::Vertex{ indi[6] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4] });
				}

				//check for air on +x face
				if ((i == CHUNK_SIZE - 1 && chunkData[i][j][k] != 0) || (i != CHUNK_SIZE - 1 && chunkData[i + 1][j][k] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[1] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5]});
					vertices.push_back(Model::Vertex{ indi[3] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4]});
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2]});
					vertices.push_back(Model::Vertex{ indi[1] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5]});
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2]});
					vertices.push_back(Model::Vertex{ indi[5] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3]});
				}

				//check for air on -y face
				if ((j == 0 && chunkData[i][j][k] != 0) || (j != 0 && chunkData[i][j - 1][k] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[6]});
					vertices.push_back(Model::Vertex{ indi[5] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5]});
					vertices.push_back(Model::Vertex{ indi[4] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[7]});
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[6]});
					vertices.push_back(Model::Vertex{ indi[1] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4]});
					vertices.push_back(Model::Vertex{ indi[5] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5]});
				}

				//check for air on +y face
				if ((j == CHUNK_SIZE - 1 && chunkData[i][j][k] != 0) || (j != CHUNK_SIZE - 1 && chunkData[i][j + 1][k] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[2] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3] });
					vertices.push_back(Model::Vertex{ indi[6] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2] });
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[0] });
					vertices.push_back(Model::Vertex{ indi[2] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3] });
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[0] });
					vertices.push_back(Model::Vertex{ indi[3] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[1] });
				}

				//check for air on -z face
				if ((k == 0 && chunkData[i][j][k] != 0) || (k != 0 && chunkData[i][j][k - 1] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5] });
					vertices.push_back(Model::Vertex{ indi[2] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4] });
					vertices.push_back(Model::Vertex{ indi[3] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2] });
					vertices.push_back(Model::Vertex{ indi[0] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5] });
					vertices.push_back(Model::Vertex{ indi[3] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2] });
					vertices.push_back(Model::Vertex{ indi[1] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3]});
				}

				//check for air on +z face
				if ((k == CHUNK_SIZE - 1 && chunkData[i][j][k] != 0) || (k != CHUNK_SIZE - 1 && chunkData[i][j][k + 1] == 0 && chunkData[i][j][k] != 0)) {
					vertices.push_back(Model::Vertex{ indi[4] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3]});
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4]});
					vertices.push_back(Model::Vertex{ indi[6] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[2]});
					vertices.push_back(Model::Vertex{ indi[4] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[3]});
					vertices.push_back(Model::Vertex{ indi[5] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[5]});
					vertices.push_back(Model::Vertex{ indi[7] + glm::vec3(i + CHUNK_SIZE * chunkLoc.x, j + CHUNK_SIZE * chunkLoc.y, k + CHUNK_SIZE * chunkLoc.z), texIndi[4]});
				}
			}
		}
	}




}

