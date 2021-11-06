

#include "application.h"


#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <crtdbg.h>



int main() {


    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    // Turn On (OR) - Keep freed memory blocks in the
    // heap's linked list and mark them as freed
    tmpFlag |= _CRTDBG_DELAY_FREE_MEM_DF;

    // Turn on memory checking at each heap operation
    tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;

    // Set the new state for the flag
    _CrtSetDbgFlag(tmpFlag);

    Application app{};



    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }


    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    
    _CrtDumpMemoryLeaks();
    return 0;
}

