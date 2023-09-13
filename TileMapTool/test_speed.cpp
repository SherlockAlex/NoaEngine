#include <iostream>
#include <chrono>
#include <cstdlib> // 用于 std::abs 函数

#define NOAABS(x) ((x ^ (x >> (sizeof(int) * 8 - 1))) - (x >> (sizeof(int) * 8 - 1)))
#define ABS(x) (x>0?x:-x)

int main() {
    const int iterations = 99999999;
    float testValue = -42.5;

    for (int j =0;j<100;j++)
    {
        // 测试 NOAABS(x)
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile int result = NOAABS((int)testValue);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "NOAABS(x) 执行 " << iterations << " 次花费时间: " << duration.count() << " 微秒" << std::endl;

        // 测试 std::abs(x)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile int result = (int)std::abs(testValue);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "std::abs(x) 执行 " << iterations << " 次花费时间: " << duration.count() << " 微秒" << std::endl;

    }

    
    return 0;
}