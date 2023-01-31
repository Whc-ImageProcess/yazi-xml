#include <iostream>
#include "Xml.h"
using namespace yazi::xml;

int main()
{
    Xml root;
    root.load("./test.xml");

    Xml & student = root["student"];

    int id = student.attr("id");
    const string & name = student["name"].text();
    const string & age = student["age"].text();
    const string & gender = student["gender"].text();

    std::cout << id << "," << age << "," << gender << std::endl;

    root.clear();
    return 0;
}