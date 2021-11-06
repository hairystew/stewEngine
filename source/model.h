#pragma once

#include "device.h"
#include "swapchain.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include <vector>
#include <array>
#include <memory>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"



class Model {

public:

	struct PushConstants {
		glm::vec4 data;
		glm::mat4 renderMatrix;

	};

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};


	




	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
		glm::vec3 normal;
		//static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		//static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		bool operator==(const Model::Vertex& other) const {
			return position == other.position && color == other.color && texCoord == other.texCoord;
		}

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};


	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		void loadModel(const std::string& filepath);
	};

	



	Model(Device& device, const Model::Builder& builder);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);




	UniformBufferObject ubo;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	static int imagecount;
	VkDeviceMemory textureImageMemory;
	static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	static std::unique_ptr<Model> createModelFromFile(Device& device, const std::string& filepath);





private:
	void createVertexBuffers(const std::vector<Vertex>& verticies);
	void createIndexBuffer(const std::vector<uint32_t> indices);
	Device &device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkDescriptorPool descriptorPool;
	VkPipelineLayout pipelineLayout;
	uint32_t vertexCount;
	uint32_t indicesCount;
};

