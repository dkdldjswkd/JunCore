#include <boost/circular_buffer.hpp>
#include <iostream>

int main() {
    // 용량이 5인 circular_buffer 생성
    boost::circular_buffer<int> cb(5);

    // 3개의 요소 추가
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    // 남은 사이즈 출력
    std::cout << "남은 사이즈: " << cb.reserve() << std::endl; // 출력: 2
    std::cout << "사용 사이즈: " << cb.size() << std::endl; // 출력: 3
    std::cout << "총 용량: " << cb.capacity() << std::endl; // 출력: 5

    return 0;
}