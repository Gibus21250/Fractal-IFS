#include <iostream>
#include <vector>

#include <kompute/Kompute.hpp>
#include <GLFW/glfw3.h>

GLFWwindow* window;

int main() {

    std::ifstream myfile; 
    myfile.open("shaders/testcomp.spv", std::ios::binary);

     if (!myfile.is_open()) {
        std::cout << "wesh c'est null ça mere\n";
        return 1;
    }

    myfile.seekg(0, std::ios::end);
    size_t fileSize = myfile.tellg();
    myfile.seekg(0, std::ios::beg);
    size_t numWords = fileSize / sizeof(uint32_t);

    std::vector<uint32_t> spirv(numWords);

    myfile.read(reinterpret_cast<char*>(spirv.data()), fileSize);

    std::shared_ptr<kp::Sequence> sq = nullptr;

    {
        kp::Manager mgr(1, {}, { "VK_EXT_shader_atomic_float" });

        std::shared_ptr<kp::Tensor> tensor = mgr.tensor({ 0, 0, 0 });

        std::shared_ptr<kp::Algorithm> algo =
          mgr.algorithm({ tensor }, spirv, kp::Workgroup({ 1 }), {}, { 0.0, 0.0, 0.0 });

        sq = mgr.sequence()
               ->record<kp::OpTensorSyncDevice>({ tensor })
               ->record<kp::OpAlgoDispatch>(algo,
                                            std::vector<float>{ 0.1, 0.2, 0.3 })
               ->record<kp::OpAlgoDispatch>(algo,
                                            std::vector<float>{ 0.3, 0.2, 0.1 })
               ->record<kp::OpTensorSyncLocal>({ tensor })
               ->eval();

        std::cout << tensor->data<float>()[0] << " " <<
        tensor->data<float>()[0] << " " << tensor->data<float>()[0];

    }
}

