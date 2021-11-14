#include "model.h"
#include <iostream>
#include <exception>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>



//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

/*The goal here is to create, store, and handle the vertex buffers, index buffers, uniform buffers, command buffers, and descriptor sets on a per
object basis. The application will loop through objects, pushing them to be rendered with the next frame. When you get this working
clear the functionality from the application class to reduce clutter and confusion plz - stew*/

template <>
struct std::hash<Model::Vertex>;




template<> struct std::hash<Model::Vertex> {
	size_t operator()(Model::Vertex const& vertex) const {
		return ((std::hash<glm::vec3>()(vertex.position) ^
			(std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
			(std::hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};






Model::Model(Device& device, Model::Builder& builder) : device{ device } {
	createVertexBuffers(builder.vertices);
	createIndexBuffer(builder.indices);
	builder.vertices.clear();
	builder.indices.clear();
}




Model::~Model() {

	vkDestroyBuffer(device.device(), indexBuffer, nullptr);
	vkFreeMemory(device.device(), indexBufferMemory, nullptr);

	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);




}

void Model::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Model::draw(VkCommandBuffer commandBuffer) {
	//vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);

	vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0, 0, 0);
}




/*
bool Model::loadFrom
(const char* filename)
{
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename);
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.position = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};


			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 0.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}

	createVertexBuffers(vertices);
	createIndexBuffer(indices);


}
*/


std::unique_ptr<Model> Model::createModelFromFile(
	Device& device, const std::string& filepath) {
	Builder builder{};
	builder.loadModel(filepath);
	return std::make_unique<Model>(device, builder);
}


void Model::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3 && "Vertex count must be at least 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		stagingBuffer, 
		stagingBufferMemory);


	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device.device(), stagingBufferMemory);

	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		vertexBuffer, 
		vertexBufferMemory);

	//probably should use a staging buffer, need to check performance
	device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);

}

void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
{

	indicesCount = static_cast<uint32_t>(indices.size());
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		stagingBuffer, 
		stagingBufferMemory);

	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device.device(), stagingBufferMemory);

	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		indexBuffer, 
		indexBufferMemory);

	device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);

}

//std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions()
//{
//	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
//	bindingDescriptions[0].binding = 0;
//	bindingDescriptions[0].stride = sizeof(Vertex);
//	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//	return bindingDescriptions;
//}
//
//std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions()
//{
//	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
//
//	//Position
//	attributeDescriptions[0].binding = 0;
//	attributeDescriptions[0].location = 0;
//	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//	attributeDescriptions[0].offset = offsetof(Vertex, position);;
//	//Color
//	attributeDescriptions[1].binding = 0;
//	attributeDescriptions[1].location = 1;
//	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//	attributeDescriptions[1].offset = offsetof(Vertex, color);
//
//	return attributeDescriptions;
//}


std::vector<VkVertexInputBindingDescription> Model::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

	//Position
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);;
	//Color
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

void Model::Builder::loadModel(const std::string& filepath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				auto colorIndex = 3 * index.vertex_index + 2;
				if (colorIndex < attrib.colors.size()) {
					vertex.color = {
						attrib.colors[colorIndex - 2],
						attrib.colors[colorIndex - 1],
						attrib.colors[colorIndex - 0],
					};
				}
				else {
					vertex.color = { 1.f, 1.f, 1.f };  // set default color
				}
			}

			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if (index.texcoord_index >= 0) {
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1. - attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void Model::Builder::buildModel(std::vector<Vertex>& inVertices)
{
	vertices.clear();
	indices.clear();
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	int iter = 0;
	for (const auto& vertex : inVertices) {

		if (uniqueVertices.count(vertex) == 0) {
			uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
			vertices.push_back(vertex);
		}
		indices.push_back(uniqueVertices[vertex]);
	}

}
