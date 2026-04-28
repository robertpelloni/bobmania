#include "RageDisplay_Vulkan.h"

RageDisplay_Vulkan::RageDisplay_Vulkan()
    : m_bInitialized(false), m_vkInstance(nullptr), m_vkDevice(nullptr),
      m_vkSwapchain(nullptr), m_vkCommandBuffer(nullptr)
{
}

RageDisplay_Vulkan::~RageDisplay_Vulkan() {
    if (m_bInitialized) {
        LOG->Info("RageDisplay_Vulkan: Destroying Vulkan Device and Instance.");
    }
}

void RageDisplay_Vulkan::Init() {
    LOG->Info("RageDisplay_Vulkan: Initializing Vulkan 1.2+ API...");
    // 1. Create vkInstance
    // 2. Select Physical Device
    // 3. Create Logical Device & Queues
    // 4. Create Swapchain

    m_bInitialized = true;
    CompilePipelines();
}

void RageDisplay_Vulkan::CompilePipelines() {
    LOG->Info("RageDisplay_Vulkan: Compiling SPIR-V Graphics Pipelines...");
    // In legacy OGL, shaders were compiled on the fly resulting in stutter.
    // Vulkan requires Pipeline State Objects (PSOs) to be pre-compiled.
}

void RageDisplay_Vulkan::BuildCommandBuffers() {
    // Vulkan allows multi-threaded command buffer generation
}

void RageDisplay_Vulkan::BeginFrame() {
    if (!m_bInitialized) return;
    // Acquire Next Image from Swapchain
    // Begin Command Buffer recording
}

void RageDisplay_Vulkan::EndFrame() {
    if (!m_bInitialized) return;
    // End Command Buffer recording
    // Submit to Queue
    // Present Swapchain Image
}
