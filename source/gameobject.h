#pragma once

#include "model.h"
#include <memory>





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

	static GameObject createGameObject() {
		static id_t currentId = 0;
		return GameObject{ currentId++ };
	}

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;


	std::shared_ptr<Model> model{};
	glm::mat4 transformMatrix{ 1.f }; 



	glm::vec3 color{};
	UniformBufferObject ubo{};
	id_t GetId() { return id; }

private:
	GameObject(id_t objId) : id{ objId } {}


	id_t id;

};

