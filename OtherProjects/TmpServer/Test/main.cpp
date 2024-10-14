#include <iostream>
#include <string>
#include <google/protobuf/message.h> // Protobuf 헤더 파일 추가
#include "addressbook.pb.h" // 생성한 protobuf 헤더 파일 추가

// 직렬화 함수: Person을 uint8_t 배열에 저장
size_t SerializePersonToArray(uint8_t* array, size_t array_size) {
    // Person 객체 생성
    Person person;
    person.set_name("Alice");
    person.set_id(1);
    person.set_email("alice@example.com");

	// Test console output
	std::cout << person.name() << std::endl;
    std::cout << "Alice" << std::endl;

    // 직렬화된 데이터의 크기
    size_t size = person.ByteSizeLong();
    if (size > array_size) {
        std::cerr << "Array size is not sufficient for serialization." << std::endl;
        return 0; // 실패
    }

    // 배열에 직렬화
    if (!person.SerializeToArray(array, size)) {
        std::cerr << "Failed to serialize person to array." << std::endl;
        return 0; // 실패
    }

    return size; // 직렬화된 데이터의 크기를 반환
}

// 역직렬화 함수: uint8_t 배열에서 Person 읽기
bool DeserializePersonFromArray(const uint8_t* array, size_t size) {
    // Person 객체 생성
    Person person;

    // 배열에서 역직렬화
    if (!person.ParseFromArray(array, size)) {
        std::cerr << "Failed to deserialize person from array." << std::endl;
        return false; // 실패
    }

    // 역직렬화된 데이터 출력 (필드별로 출력)
    std::cout << "Name: " << person.name() << std::endl;
    std::cout << "ID: " << person.id() << std::endl;
    std::cout << "Email: " << person.email() << std::endl;

    return true; // 성공
}

int main() {
    // 직렬화할 Person 데이터 크기
    const size_t array_size = 128; // 충분한 크기로 설정
    uint8_t serialized_data[array_size];
	memset(serialized_data, 0, array_size); // 배열 초기화

    // Person 직렬화
    size_t serialized_size = SerializePersonToArray(serialized_data, array_size);

    if (serialized_size > 0) {
        // 배열에서 역직렬화
        DeserializePersonFromArray(serialized_data, serialized_size);
    }

    return 0;
}
