#include <iostream>
#include <string>
#include <google/protobuf/message.h> // Protobuf ��� ���� �߰�
#include "addressbook.pb.h" // ������ protobuf ��� ���� �߰�

// ����ȭ �Լ�: Person�� uint8_t �迭�� ����
size_t SerializePersonToArray(uint8_t* array, size_t array_size) {
    // Person ��ü ����
    Person person;
    person.set_name("Alice");
    person.set_id(1);
    person.set_email("alice@example.com");

	// Test console output
	std::cout << person.name() << std::endl;
    std::cout << "Alice" << std::endl;

    // ����ȭ�� �������� ũ��
    size_t size = person.ByteSizeLong();
    if (size > array_size) {
        std::cerr << "Array size is not sufficient for serialization." << std::endl;
        return 0; // ����
    }

    // �迭�� ����ȭ
    if (!person.SerializeToArray(array, size)) {
        std::cerr << "Failed to serialize person to array." << std::endl;
        return 0; // ����
    }

    return size; // ����ȭ�� �������� ũ�⸦ ��ȯ
}

// ������ȭ �Լ�: uint8_t �迭���� Person �б�
bool DeserializePersonFromArray(const uint8_t* array, size_t size) {
    // Person ��ü ����
    Person person;

    // �迭���� ������ȭ
    if (!person.ParseFromArray(array, size)) {
        std::cerr << "Failed to deserialize person from array." << std::endl;
        return false; // ����
    }

    // ������ȭ�� ������ ��� (�ʵ庰�� ���)
    std::cout << "Name: " << person.name() << std::endl;
    std::cout << "ID: " << person.id() << std::endl;
    std::cout << "Email: " << person.email() << std::endl;

    return true; // ����
}

int main() {
    // ����ȭ�� Person ������ ũ��
    const size_t array_size = 128; // ����� ũ��� ����
    uint8_t serialized_data[array_size];
	memset(serialized_data, 0, array_size); // �迭 �ʱ�ȭ

    // Person ����ȭ
    size_t serialized_size = SerializePersonToArray(serialized_data, array_size);

    if (serialized_size > 0) {
        // �迭���� ������ȭ
        DeserializePersonFromArray(serialized_data, serialized_size);
    }

    return 0;
}
