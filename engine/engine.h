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

public:
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
    void addDrawableObject(std::vector<void*>& bindings, size_t nbVertices);

    struct DrawableObject {
        std::vector<VkBuffer> bindings;
        size_t nbvertices;
    };

    struct Vertex {
        glm::vec2 pos;

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
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            return attributeDescriptions;
        }


    };

    GLFWwindow* getGLFWindow(){return window;};
    void setKeyboardCallBack(void (*key_callback) (GLFWwindow* window, int key, int scancode, int action, int mods));

private:
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
    VkQueue transfertQueue;

    std::vector<DrawableObject> drawablesObjects;

    std::vector<VkBuffer> vkBuffermanaged;
    std::vector<VkDeviceMemory> vkmemorymanaged;
    std::unordered_map<void*, VkBuffer> vkbuffersrawpointer;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    //------------------------------------------------------------------

    /**
     * @brief Infos of a queue.
     */
    struct QueueInfo {
        uint32_t family;
        uint32_t indice;
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
    void createGraphicsPipeline();
    void createFramebuffers();
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

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};


#endif //IFS_ENGINE_H
