#pragma once

#include "device.h"

#include <string>
#include <vector>

struct pipelineConfigInfo {

	pipelineConfigInfo(const pipelineConfigInfo&) = delete;
	pipelineConfigInfo& operator=(const pipelineConfigInfo&) = delete;

	
	VkDescriptorSetLayoutBinding uboLayoutBinding;
	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;

};

class Pipeline
{
public:
	Pipeline(
		Device& Vdevice, 
		const std::string& vertFilepath, 
		const std::string& fragFilepath, 
		const pipelineConfigInfo& configInfo);
	~Pipeline();

	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;


	VkPipeline getPipeline() { return graphicsPipeline; };
	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(pipelineConfigInfo& configInfo);

private:
	static std::vector<char> readFile(const std::string& filepath);



	void createGraphicsPipeline(
		const std::string& vertFilepath, 
		const std::string& fragFilepath, 
		const pipelineConfigInfo& configInfo);

	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	Device& device;
	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;


};

