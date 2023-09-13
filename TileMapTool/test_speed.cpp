#include <iostream>
#include <chrono>
#include <cstdlib> // ���� std::abs ����

#define NOAABS(x) ((x ^ (x >> (sizeof(int) * 8 - 1))) - (x >> (sizeof(int) * 8 - 1)))
#define ABS(x) (x>0?x:-x)

int main() {
    const int iterations = 99999999;
    float testValue = -42.5;

    for (int j =0;j<100;j++)
    {
        // ���� NOAABS(x)
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile int result = NOAABS((int)testValue);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "NOAABS(x) ִ�� " << iterations << " �λ���ʱ��: " << duration.count() << " ΢��" << std::endl;

        // ���� std::abs(x)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile int result = (int)std::abs(testValue);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "std::abs(x) ִ�� " << iterations << " �λ���ʱ��: " << duration.count() << " ΢��" << std::endl;

    }

    
    return 0;
}