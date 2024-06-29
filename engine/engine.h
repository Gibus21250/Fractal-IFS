//
// Created by Nathan on 08/06/2024.
//

#ifndef IFS_ENGINE_H
#define IFS_ENGINE_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "vulkan/vulkan.hpp"

#include <glm/glm.hpp>

#include "camera.h"


class Engine {

    template<class type>
    struct optional {
        type value;
        bool hasValue;
    };

    struct QueueFamilyIndices {
        optional<uint32_t> graphicsFamily;
        optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.hasValue && presentFamily.hasValue;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct MeshPushConstants {
        glm::uint nbIteration;
        glm::uint maxInstance;
        glm::uint nbTransformation;
        glm::uint padding;
        glm::mat4 render_matrix;
    };

    struct BufferInfo {
        void* mappedPointer;
        VkBuffer buffer;
        uint32_t size;
    };

public:

    void setCamera(Camera &cam);
    Camera &getCamera();

    void initEngine();
    void run();
    bool framebufferResized = false;
    VkInstance getVkInstance() const {return instance;};
    VkPhysicalDevice getVkPhysicalDevice() const {return physicalDevice;};
    VkDevice getVkDevice() const {return device;};
    VkQueue getComputequeue() const {return computeQueue;};
    void destroy();

    void* createBuffer(uint32_t size, VkBufferUsageFlagBits flags, VkMemoryPropertyFlags properties);
    void deleteBuffer(void* ptr);

    /**
     * Add a drawable element
     * The first element of bindings is the vertex buffer, other are simple buffer
     * @param bindings
     * @param nbVertices
     */
    void addDrawableObject(std::vector<std::string>& shaders, std::vector<void*>& buffers, size_t nbVertices, uint32_t nbInstance = 1);
    void clearDrawableObjects();

    struct DrawableObject {
        std::vector<VkBuffer> bindings;
        size_t nbVertices;
        uint32_t nbInstance;
    };

    struct Vertex {
        glm::vec3 pos;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            return attributeDescriptions;
        }


    };

    GLFWwindow* getGLFWindow(){return window;};

    //Rendering features
    void switchConservativeRaster();

    MeshPushConstants pushConstants{};

private:

    //Rendering features
    bool conservativeRasterizer = false;

    Camera camera{};
    uint32_t framenumber = 0;

    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    std::vector<DrawableObject> drawablesObjects;
    std::vector<VkBuffer> vkBuffermanaged;
    std::vector<VkDeviceMemory> vkmemorymanaged;
    std::unordered_map<void*, BufferInfo> vkbuffersrawpointer;

    std::string lastVertexShader;
    std::string lastFragmentShader;

    //Depth
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    /**
     * @brief Infos of a queue.
     */
    struct QueueInfo {
        uint32_t family;
        uint32_t indices;
    };

    /**
     * @brief Struct containing indices of family queue.
     * @brief see @ref findQueueFamilies() to see how data are filled
     */
    struct QueuesNeeded {
        QueueInfo graphics;
        QueueInfo present;
        QueueInfo transfert;
        QueueInfo compute;
        bool graphicsHasValue = false;
        bool presentHasValue = false;
        bool transfertHasValue = false;
        bool computeHasValue = false;
        bool isComplete() { return graphicsHasValue && presentHasValue && transfertHasValue && computeHasValue; }
    };

    void initVulkan();
    void pickPhysicalDevice();
    void createInstance();
    void createSurface();
    void createLogicalDevice();
    QueuesNeeded getQueuesNeeded(VkPhysicalDevice device);

    void recreateSwapChain();
    void cleanupSwapChain();
    void createSwapChain();

    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createDepthResources();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();

    void initWindow();

    void mainLoop();
    void drawFrame();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    void cleanup();

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pDeviceT);
    bool isDeviceSuitable(VkPhysicalDevice pDeviceT);
    bool checkDeviceExtensionSupport(VkPhysicalDevice pDeviceT);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pDeviceT);
    static std::vector<const char*> getRequiredExtensions();

    static bool checkValidationLayerSupport();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};


#endif //IFS_ENGINE_H
