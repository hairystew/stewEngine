#include "application.h"




#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>






struct SimplePushConstantData {
	glm::vec4 data;
	glm::mat4 render_matrix;

};

Application::Application() {
	createDescriptorSetLayout();
	createPipelineLayout();
	recreateSwapChain();

	createTextureImage();
	createTextureImageView();

	createTextureSampler();

	loadGameObjects();
	createUniformBuffers();
	createDescriptorPool();
	createCommandBuffers();
	createDescriptorSets();
}

Application::~Application() {

	vkDestroyInstance(instance, nullptr);

	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);

	vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);

	for (size_t i = 0; i < swapchain->imageCount(); i++) {
		vkDestroyBuffer(device.device(), uniformBuffers[i], nullptr);
		vkFreeMemory(device.device(), uniformBuffersMemory[i], nullptr);

	}


	vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);


	vkDestroyImage(device.device(), textureImage, nullptr);
	vkFreeMemory(device.device(), textureImageMemory, nullptr);
	vkDestroySampler(device.device(), textureSampler, nullptr);
	vkDestroyImageView(device.device(), textureImageView, nullptr);

}

void Application::run() {

	time = glfwGetTime();

	while (!window.shouldClose()) {
		glfwPollEvents();
		window.update();
		updateGameObjects();
		float newTime = glfwGetTime();
		timeStep = newTime - time;
		time = newTime;
		drawFrame();
		//std::cout << "Player Position: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << " "
		//std::cout << "Player Looking: " << camera.forward.x << " " << camera.forward.y << " " << camera.forward.z << std::endl;
		//std::cout << "MOUSE DELS: " << camera.pitch << " " << camera.yaw << std::endl;
		//std::cout << 1 / timeStep << std::endl;

	}

	vkDeviceWaitIdle(device.device());

}



void Application::loadGameObjects()
{

	

	
	//std::vector<Model::Vertex> vertices {
	//	{{-0.5f, -0.5f, 0}, {1.0f, 0.0f, 0.0f}},
	//	{{0.5f, -0.5f, 0}, {0.0f, 1.0f, 0.0f}},
	//	{{0.5f, 0.5f, 0}, {0.0f, 0.0f, 1.0f}},
	//	{{-0.5f,0.5f, 0}, {1.0f, 1.0f, 0.0f}},
	//};

	//const std::vector<uint32_t> indices = {
	//0, 1, 2, 2, 3, 0
	//};
	//

	//std::vector<Model::Vertex> vertices1 {
	//	{{-0.5f, -0.5f, 0}, {1.0f, 0.0f, 0.0f}},
	//	{{0.5f, -0.5f, 0}, {0.0f, 1.0f, 0.0f}},
	//	{{0.5f, 0.5f, 0}, {0.0f, 0.0f, 1.0f}},
	//};

	//const std::vector<uint32_t> indices1 = {
	//0, 1, 2
	//};
	//auto model = std::make_shared<Model>(device, vertices, indices);

	//auto  triangle = GameObject::createGameObject();
	//triangle.model = model;
	//
	//auto monkey = std::make_shared<Model>(device, "monkeySmooth.obj");
	//auto monk = GameObject::createGameObject();

	//monk.model = monkey;
	////gameObjects.push_back(std::move(triangle));
	//gameObjects.push_back(std::move(monk));

	//std::vector<Model::Vertex> vertices = {
	//{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	//{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	//{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	//{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	//};


	//std::vector<uint16_t> indices = {
	//0, 1, 2, 2, 3, 0
	//};


		//Chunk chunk;
		//chunk.createModel(device);
		//gameObjects.push_back(std::move(chunk));
	
	//std::shared_ptr<Model> roomModel = Model::createModelFromFile(device, "../source/shrek.obj");
	//std::shared_ptr<Model> roomModel = Model::createModelFromFile(device, "../source/viking_room.obj");
	//auto room = GameObject::create();
	//room.model = roomModel;
	//room.transformMatrix = glm::translate(glm::mat4{ 1.0 }, glm::vec3(-2, 0, 0)) * glm::scale(glm::mat4{ 1.0 }, glm::vec3(1., 1., 1.));
	//gameObjects.push_back(std::move(room));


	/*

	auto monk = std::make_shared<Model>(device, "monkeySmooth.obj");
	auto monkey = GameObject::createGameObject();
	monkey.model = monk;
	//monkey.material = getMaterial("defaultmesh");
	monkey.transformMatrix = glm::translate(glm::mat4{ 1.0 }, glm::vec3(-2, 0, 0)) * glm::scale(glm::mat4{ 1.0 }, glm::vec3(1., 1., 1.));


	gameObjects.push_back(std::move(monkey));



	auto yod = std::make_shared<Model>(device, "Baby_Yoda.obj");
	auto yoddler = GameObject::createGameObject();
	yoddler.model = yod;
	//monkey.material = getMaterial("defaultmesh");
	yoddler.transformMatrix = glm::translate(glm::mat4{ 1.0 }, glm::vec3(2, 0, 0)) * glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.03, 0.03, 0.03));

	gameObjects.push_back(std::move(yoddler));



	for (int x = -20; x <= 20; x++) {
		for (int y = -20; y <= 20; y++) {


			std::vector<Model::Vertex> vertices{
					{{-0.5f, -0.5f, 0}, {1.0f, 0.0f, 0.0f}},
					{{0.5f, -0.5f, 0}, {0.0f, 1.0f, 0.0f}},
					{{0.5f, 0.5f, 0}, {0.0f, 0.0f, 1.0f}},
				};

			const std::vector<uint32_t> indices = {
			0, 1, 2
			};

			auto tri = std::make_shared<Model>(device, vertices, indices);
			auto triangle = GameObject::createGameObject();
			triangle.model = tri;
			//triangle.material = getMaterial("defaultmesh");
			glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, glm::vec3(x, 0, y));
			glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.2, 0.2, 0.2));
			triangle.transformMatrix = translation * scale;

			gameObjects.push_back(std::move(triangle));
		}
	}



	*/
}

void Application::createTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("../source/textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	//stbi_uc* pixels = stbi_load("../source/textures/viking_room.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	
	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	device.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device.device(), stagingBufferMemory);

	stbi_image_free(pixels);

	createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}




void Application::updateGameObjects()
{
	glm::vec3 centerChunk = glm::vec3(std::floor(camera.position.x / (float)CHUNK_SIZE),
							std::floor(camera.position.y / (float)CHUNK_SIZE),
							0);
	


	//these are the chunks that we want to load
	std::vector<glm::ivec3> desiredChunks;
	for (int i = centerChunk.x - (RENDER_SIZE / 4); i < centerChunk.x + (RENDER_SIZE / 4); i++) {
		for (int j = centerChunk.y - (RENDER_SIZE / 4); j < centerChunk.y + (RENDER_SIZE / 4); j++) {
			desiredChunks.push_back(glm::ivec3(i, j, 0));
		}
	}
	
	std::sort(desiredChunks.begin(), desiredChunks.end(), [](glm::ivec3& a, glm::ivec3& b) {
		std::hash<std::string> has;
		return has(std::to_string(a.x) + std::to_string(a.y) + std::to_string(a.z))
			< has(std::to_string(b.x) + std::to_string(b.y) + std::to_string(b.z));
		});



	auto hashFunc = [](glm::ivec3 desChunks) {

		std::hash<std::string> hasher;
		return hasher(std::to_string(desChunks.x) + std::to_string(desChunks.y) + std::to_string(desChunks.z));

	};
	volatile std::size_t hashVal;
	volatile std::size_t desHash;
	for (int k = 0; k < desiredChunks.size(); k++) {

		while (((int)gameChunks.size() - 1) < k || hashFunc(desiredChunks[k]) != gameChunks[k]->hash) {
			if (((int)gameChunks.size() - 1) < k || hashFunc(desiredChunks[k]) < gameChunks[k]->hash) {
				desHash = hashFunc(desiredChunks[k]);
				std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();
				chunk->chunkLoc = desiredChunks[k];
				chunk->createModel(device);
				std::size_t hashVal = chunk->hash;
				auto it = std::upper_bound(gameChunks.cbegin(), gameChunks.cend(), hashVal, [](std::size_t value, const std::unique_ptr<Chunk>& chnk) {
					return value < chnk->hash;
				});
				gameChunks.insert(it, std::move(chunk));
				chunk.reset();
			}
			else {
				gameChunks[k].reset();
				gameChunks.erase(gameChunks.begin() + k);
			}
		}
	}
	for (int l = desiredChunks.size(); l < gameChunks.size(); l++) {
		gameChunks[l].reset();
	}
	gameChunks.erase(gameChunks.begin() + desiredChunks.size(), gameChunks.end());





}

void Application::createPipelineLayout()
{

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);


	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void Application::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}



}

void Application::createPipeline() {
	assert(swapchain != nullptr && "Cannot create pipeline before swapchain");
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");


	pipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = swapchain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(
		device,
		"../source/Shaders/vert.spv",
		"../source/Shaders/frag.spv",
		pipelineConfig);
}

void Application::recreateSwapChain() {
	auto extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(device.device());

	if (swapchain == nullptr) {
		swapchain = std::make_unique<SwapChain>(device, extent);

	}
	else {
		swapchain = std::make_unique<SwapChain>(device, extent, std::move(swapchain));


		if (swapchain->imageCount() != commandBuffers.size()) {
			freeCommandBuffers();
			createUniformBuffers();
			createDescriptorPool();
			createDescriptorSets();
			createCommandBuffers();
		}
	}
	createPipeline();



}

void Application::freeCommandBuffers() {
	vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();

}

void Application::createCommandBuffers() {
	commandBuffers.resize(swapchain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	
}

void Application::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	device.endSingleTimeCommands(commandBuffer);
}

void Application::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);


	device.endSingleTimeCommands(commandBuffer);
}

void Application::recordCommandBuffer(int imageIndex) {



	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}


	


	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapchain->getRenderPass();
	renderPassInfo.framebuffer = swapchain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapchain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapchain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, swapchain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);


	vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);



	renderGameObjects(commandBuffers[imageIndex]);
	


	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer");
	}
}

void Application::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(GPUCameraData);

	uniformBuffers.resize(swapchain->imageCount());
	uniformBuffersMemory.resize(swapchain->imageCount());





	for (size_t i = 0; i < swapchain->imageCount(); i++) {
		device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Application::createDescriptorPool() {

	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchain->imageCount());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchain->imageCount());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapchain->imageCount());

	if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

}
//
//Application::Material* Application::createMaterial(std::unique_ptr<Pipeline> pipeline, VkPipelineLayout layout, const std::string& name)
//{
//	Material mat;
//	mat.pipeline = pipeline->getPipeline();
//	mat.pipelineLayout = layout;
//	materials[name] = mat;
//	return &materials[name];
//}

//Application::Material* Application::getMaterial(const std::string& name)
//{
//	//search for the object, and return nullpointer if not found
//	auto it = materials.find(name);
//	if (it == materials.end()) {
//		return nullptr;
//	}
//	else {
//		return &(*it).second;
//	}
//}

void Application::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(swapchain->imageCount(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchain->imageCount());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapchain->imageCount());
	if (vkAllocateDescriptorSets(device.device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapchain->imageCount(); i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device.device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Application::renderGameObjects(VkCommandBuffer commandBuffer) {


	for (auto& obj : gameChunks) {

		pipeline->bind(commandBuffer);
		SimplePushConstantData push{};

		//model rotation
		glm::mat4 model = obj->transformMatrix;
		//calculate final mesh matrix
		glm::mat4 mesh_matrix = model;

		push.render_matrix = mesh_matrix;



		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj->model->bind(commandBuffer);
		obj->model->draw(commandBuffer);
	}

	for (auto& obj : gameObjects) {

		pipeline->bind(commandBuffer);
		SimplePushConstantData push{};

		//model rotation
		glm::mat4 model = obj.transformMatrix;
		//calculate final mesh matrix
		glm::mat4 mesh_matrix = model;

		push.render_matrix = mesh_matrix;

		

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}

void Application::drawFrame() {

	uint32_t imageIndex;
	auto result = swapchain->acquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}



	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;






	recordCommandBuffer(imageIndex);



	result = swapchain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
		window.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}


	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

}

void Application::updateUniformBuffer(uint32_t currentImage) {
	camera.update(timeStep);
	/*
	//camera view
	//glm::vec3 camPos = { 1.f * glm::sin(.2f * time),1.f,1.f * glm::cos(.2f * time) };
	glm::vec3 camPos = { 1.f , 1.f, 1.5f };
	//std::cout << "x: " << camPos.x << " y: " << camPos.y << " z: " << camPos.z << std::endl;

	//glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	glm::mat4 view = glm::lookAt(camPos, glm::vec3{ 0.f,0.f,0.f }, glm::vec3{ 0.f,0.f,1.f });
	//camera projection
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	projection[1][1] *= -1;


	//fill a GPU camera data struct
	cam.projection = projection;
	cam.view = view;
	cam.viewproj = projection * view;
	//and copy it to the buffer
	*/
	

	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	//ubo.model = glm::rotate(glm::mat4(1.0f), 0 * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.model = glm::mat4(1.f);
	//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = camera.lookMat;
	ubo.proj = glm::perspective(glm::radians(45.0f), swapchain->getSwapChainExtent().width / (float)swapchain->getSwapChainExtent().height, 0.1f, 10000.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(device.device(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device.device(), uniformBuffersMemory[currentImage]);


	
}

void Application::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(device.device(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device.device(), image, &memRequirements);


	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device.device(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device.device(), image, imageMemory, 0);
}

void Application::createTextureImageView() {
	textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);
}

VkImageView Application::createImageView(VkImage image, VkFormat format) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device.device(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}



void Application::createTextureSampler() {
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device.returnPhysicslDevice(), &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}