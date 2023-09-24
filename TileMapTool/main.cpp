#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = { 1, 2, 2, 3, 4, 4, 4, 5, 6, 6 ,1,1,3,2,5,5,6,1,1,0};

    // ��������������
    std::sort(vec.begin(), vec.end());

    // ʹ�� std::unique() ����ɾ���ظ�Ԫ��
    auto new_end = std::unique(vec.begin(), vec.end());

    // ɾ�������Ԫ��
    vec.erase(new_end, vec.end());

    // ������
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}