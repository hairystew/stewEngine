#pragma once



#include "device.h"
#include "pipeline.h"
#include "swapchain.h"
#include "window.h"
#include "gameobject.h"

#include <memory>
#include <vector>
#include <chrono>


class Application
{

public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 800;





	static struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		float time;
	};


	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void run();

private:

	struct GPUCameraData {
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::mat4 viewproj;
	};

	struct Material {
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
	};



	void loadGameObjects();//this shouldn't be in this class
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	VkImageView createImageView(VkImage image, VkFormat format);
	void updateGameObjects();
	void createPipelineLayout();
	void createDescriptorSetLayout();
	void createPipeline();
	void createCommandBuffers();
	void freeCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
	void renderGameObjects(VkCommandBuffer commandBuffer);
	void createUniformBuffers();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void createDescriptorSets();
	int getImageCount() { return swapchain->imageCount(); }
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	//Material* createMaterial(std::unique_ptr<Pipeline> pipeline, VkPipelineLayout layout, const std::string& name);
	//Material* getMaterial(const std::string& name);
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	float timestep;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	UniformBufferObject cam;
	Window window{ WIDTH, HEIGHT, "Window Dingus" };
	Device device{ window };
	std::unique_ptr<SwapChain> swapchain;
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkSampler textureSampler;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkImageView textureImageView;



	std::vector<GameObject> gameObjects;
	std::unordered_map<std::string, Model> models;




};

