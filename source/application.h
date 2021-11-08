#pragma once



#include "device.h"
#include "pipeline.h"
#include "swapchain.h"
#include "window.h"
#include "gameobject.h"
#include "chunk.h"

#include <math.h>
#include <memory>
#include <vector>
#include <chrono>

#define MOVEMENT_SPEED 50.0f
#define MOUSE_SENSITIVITY 0.001f

class Application
{

public:
	static constexpr int WIDTH = 1920;
	static constexpr int HEIGHT = 1080;





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

	struct Camera {
		float pitch = 0.f;
		float yaw = 0.f;
		glm::vec3 forward{ 1., 0., 0. };
		glm::vec3 up{ 0., 0., 1. };
		glm::vec3 position{0., 0., 0. };
		glm::quat orientation{};
		glm::mat4 lookMat{};
		glm::vec2 mousePos{};

		void update(float deltaTime) {
			float delPitch = (Window::inputs.mouseY - mousePos.y) * MOUSE_SENSITIVITY;
			float delYaw = (Window::inputs.mouseX - mousePos.x) * MOUSE_SENSITIVITY;
			pitch += delPitch;
			yaw += delYaw;

			glm::vec3 noVert = glm::normalize(glm::vec3(forward.x, forward.y, 0.f));
			if ((forward.z < -.99f && pitch < 0) || (forward.z > .99f && pitch > 0))
				pitch = 0.0f;

			forward = glm::angleAxis(-delPitch, glm::cross(noVert, up)) * forward;
			forward = glm::angleAxis(-delYaw, up) * forward;
			//std::cout  << "      m_forward: " << m_forward.x << ", " << m_forward.y << ", " << m_forward.z << std::endl;
			//move camera position in direction of forward vector
			
			glm::vec3 vForward = glm::normalize(glm::vec3(forward.x, forward.y, 0.f)) * deltaTime * MOVEMENT_SPEED;
			if (Window::inputs.isForward)
				position += vForward;
			if(Window::inputs.isBack)
				position -= vForward;
			if(Window::inputs.isLeft)
				position -= glm::cross(vForward, up);
			if(Window::inputs.isRight)
				position += glm::cross(vForward, up);
			if (Window::inputs.isUp)
				position += up * deltaTime * MOVEMENT_SPEED;
			if (Window::inputs.isDown)
				position -= up * deltaTime * MOVEMENT_SPEED;
			mousePos = glm::vec2(Window::inputs.mouseX, Window::inputs.mouseY);
			lookMat = (glm::lookAt(position, position + forward, up));
		}
	}camera;

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
	float timeStep;
	float time;
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



	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::unordered_map<std::string, Model> models;




};

