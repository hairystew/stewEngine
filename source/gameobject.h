#pragma once

#include "model.h"
#include <memory>



enum GameType {
	GameMob,
	GameChunk,
};

class GameObject
{
public:
	using id_t = unsigned int;

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		float time;
	};

	static GameObject create() {
		return GameObject{ currentId++ };
	}
	static id_t currentId;

	GameObject() {
		id = currentId++;
	}

	~GameObject() {}

	//some wack copy constructor shit
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	std::shared_ptr<Model> model{};
	glm::mat4 transformMatrix{ 1.f }; 
	glm::vec3 pos{};
	glm::vec3 color{};
	UniformBufferObject ubo{};
	id_t GetId() { return id; }
	int gameType;
private:
	GameObject(id_t objId) : id{ objId } {}
	id_t id;

};
