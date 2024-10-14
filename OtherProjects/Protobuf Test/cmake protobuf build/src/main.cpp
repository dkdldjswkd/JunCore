#include <iostream>
#include "addressbook.pb.h"
using namespace std;

int main()
{
    // Person °´Ã¼ »ý¼º
    Person person;
    person.set_name("Alice");
    person.set_id(1);
    person.set_email("alice@example.com");

    // Test console output
    std::cout << person.name() << std::endl;
    std::cout << "Alice" << std::endl;
}