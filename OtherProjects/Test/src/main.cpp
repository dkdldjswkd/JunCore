#include <iostream>
#include "addressbook.pb.h"
#include "google/protobuf/port_def.inc"
using namespace std;

int main()
{
	Person p;
	p.set_id(3);
	cout << p.id() << endl;
}