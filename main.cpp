#include <iostream>
#include <vector>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"
#include "engine/utils.h"

int main()
{

    //Initialize small render engine
    Engine e;
    e.initEngine();

    auto instance = std::make_shared<vk::Instance>(e.getVkInstance());
    auto physicalDevice = std::make_shared<vk::PhysicalDevice>(e.getVkPhysicalDevice());
    auto device = std::make_shared<vk::Device>(e.getVkDevice());

    std::vector<uint32_t> familyIndice{2};
    auto computeQueue = std::make_shared<vk::Queue>(e.getComputequeue());
    std::vector<std::shared_ptr<vk::Queue>> computeList{computeQueue};

    kp::Manager mgr{instance, physicalDevice, device, familyIndice, computeList};

    // 2. Create and initialise Kompute Tensors through manager

    // Default tensor constructor simplifies creation of float values
    auto tensorInA = mgr.tensor({ 2., 2., 2. });
    auto tensorInB = mgr.tensor({ 1., 2., 3. });
    // Explicit type constructor supports uint32, int32, double, float and bool
    auto tensorOutA = mgr.tensorT<uint32_t>({ 0, 0, 0 });
    auto tensorOutB = mgr.tensorT<uint32_t>({ 0, 0, 0 });

    std::vector<std::shared_ptr<kp::Tensor>> params = {tensorInA, tensorInB, tensorOutA, tensorOutB};

    // 3. Create algorithm based on shader (supports buffers & push/spec constants)
    kp::Workgroup workgroup({3, 1, 1});
    std::vector<float> specConsts({ 2 });
    std::vector<float> pushConstsA({ 2.0 });
    std::vector<float> pushConstsB({ 3.0 });

    std::vector<char> buffer = Utils::readFile("shaders/testcomp.spv");
    //Convert vector type char into int32 for kompute
    std::vector<uint32_t> spirv = {(uint32_t*)buffer.data(), (uint32_t*)(buffer.data() + buffer.size())};

    auto algorithm = mgr.algorithm(params,
                                   spirv,
                                   workgroup,
                                   specConsts,
                                   pushConstsA);

    // 4. Run operation synchronously using sequence
    mgr.sequence()
            ->record<kp::OpTensorSyncDevice>(params)
            ->record<kp::OpAlgoDispatch>(algorithm) // Binds default push consts
            ->eval() // Evaluates the two recorded operations
            ->record<kp::OpAlgoDispatch>(algorithm, pushConstsB) // Overrides push consts
            ->eval(); // Evaluates only last recorded operation

    // 5. Sync results from the GPU asynchronously
    auto sq = mgr.sequence();
    sq->evalAsync<kp::OpTensorSyncLocal>(params);

    // ... Do other work asynchronously whilst GPU finishes

    sq->evalAwait();


    std::cout << "---------------------------------------------\n";
    // Prints the first output which is: { 4, 8, 12 }
    for (const float& elem : tensorOutA->vector()) std::cout << elem << "  ";
    std::cout << std::endl;
    // Prints the second output which is: { 10, 10, 10 }
    for (const float& elem : tensorOutB->vector()) std::cout << elem << "  ";
    std::cout << std::endl;
    std::cout << "---------------------------------------------\n";
}
