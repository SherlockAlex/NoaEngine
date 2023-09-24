#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = { 1, 2, 2, 3, 4, 4, 4, 5, 6, 6 ,1,1,3,2,5,5,6,1,1,0};

    // 对向量进行排序
    std::sort(vec.begin(), vec.end());

    // 使用 std::unique() 函数删除重复元素
    auto new_end = std::unique(vec.begin(), vec.end());

    // 删除多余的元素
    vec.erase(new_end, vec.end());

    // 输出结果
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}