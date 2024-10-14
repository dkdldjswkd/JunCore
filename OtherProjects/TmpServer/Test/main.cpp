#include <iostream>
#include <fstream>
#include "addressbook.pb.h"  // proto 파일에서 생성된 헤더 파일

// Person 메시지 직렬화 함수
void SerializePerson(const std::string& filename) {
    // Person 객체 생성
    Person person;

    // 필드에 데이터 설정
    person.set_name("Alice");
    person.set_id(1);
    person.set_email("alice@example.com");

    // 직렬화된 데이터를 파일로 저장
    std::ofstream output_file(filename, std::ios::binary);
    if (!person.SerializeToOstream(&output_file)) {
        std::cerr << "Failed to serialize person." << std::endl;
    }
    output_file.close();
}

// Person 메시지 역직렬화 함수
void DeserializePerson(const std::string& filename) {
    // Person 객체 생성
    Person person;

    // 파일에서 직렬화된 데이터 읽기
    std::ifstream input_file(filename, std::ios::binary);
    if (!person.ParseFromIstream(&input_file)) {
        std::cerr << "Failed to deserialize person." << std::endl;
        return;
    }
    input_file.close();

    // 역직렬화된 데이터 출력
    std::cout << "Name: " << person.name() << std::endl;
    std::cout << "ID: " << person.id() << std::endl;
    std::cout << "Email: " << person.email() << std::endl;
}

int main() {
    const std::string filename = "person.bin";

    // Person 직렬화
    SerializePerson(filename);
    std::cout << "Data serialized to " << filename << std::endl;

    // Person 역직렬화
    DeserializePerson(filename);

    return 0;
}
