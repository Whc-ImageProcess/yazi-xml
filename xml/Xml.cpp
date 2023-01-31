#include "Xml.h"
#include "Parser.h"
using namespace yazi::xml;

#include <fstream>
#include <sstream>
using namespace std;

Value::Value()
{
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(const char * value) : m_value(value)
{
}

Value::Value(const string & value) : m_value(value)
{
}

Value::~Value()
{
}

Value & Value::operator = (bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator = (int value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator = (double value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator = (const char * value)
{
    m_value = value;
    return *this;
}

Value & Value::operator = (const string & value)
{
    m_value = value;
    return *this;
}

Value & Value::operator = (const Value & value)
{
    m_value = value.m_value;
    return *this;
}

bool Value::operator == (const Value & other)
{
    return m_value == other.m_value;
}

bool Value::operator != (const Value & other)
{
    return !(m_value == other.m_value);
}

Value::operator bool()
{
    if (m_value == "true")
        return true;
    else if (m_value == "false")
        return false;
    return false;
}

Value::operator int()
{
    return std::atoi(m_value.c_str());
}

Value::operator double()
{
    return std::atof(m_value.c_str());
}

Value::operator string()
{
    return m_value;
}

Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
}

Xml::Xml(const char * name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
    m_name = new string(name);
}

Xml::Xml(const string & name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
    m_name = new string(name);
}

Xml::Xml(const Xml & other)
{
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;   
}

string Xml::name() const
{
    if (m_name == nullptr)
    {
        return "";
    }
    return *m_name;
}

void Xml::name(const string & name)
{
    if (m_name != nullptr)
    {
        delete m_name;
    }
    m_name = new string(name);
}

string Xml::text() const
{
    if (m_text == nullptr)
    {
        return "";
    }
    return *m_text;
}

void Xml::text(const string & text)
{
    if (m_text != nullptr)
    {
        delete m_text;
    }
    m_text = new string(text);
}

Value & Xml::attr(const string & key)
{
    if (m_attrs == nullptr)
    {
        m_attrs = new std::map<string, Value>();
    }
    return (*m_attrs)[key];
}

void Xml::attr(const string & key, const Value & value)
{
    if (m_attrs == nullptr)
    {
        m_attrs = new std::map<string, Value>();
    }
    (*m_attrs)[key] = value;
}

Xml & Xml::operator = (const Xml & other)
{
    clear();
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
    return *this;
}

void Xml::append(const Xml & child)
{
    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    m_child->push_back(child);
}

Xml & Xml::operator [] (int index)
{
    if (index < 0)
    {
        throw std::logic_error("index less than zero");
    }
    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    int size = m_child->size();
    if (index >= 0 && index < size)
    {
        auto it = m_child->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }
        return *it;
    }
    if (index >= size)
    {
        for (int i = size; i < index; i++)
        {
            m_child->push_back(Xml());
        }
    }
    return m_child->back();
}

Xml & Xml::operator [] (const char * name)
{
    return (*this)[string(name)];
}

Xml & Xml::operator [] (const string & name)
{
    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    for (auto it = m_child->begin(); it != m_child->end(); it++)
    {
        if (it->name() == name)
        {
            return *it;
        }
    }
    m_child->push_back(Xml(name));
    return m_child->back();
}

void Xml::remove(int index)
{
    if (m_child == nullptr)
    {
        return;
    }
    int size = m_child->size();
    if (index < 0 || index >= size)
    {
        return;
    }
    auto it = m_child->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    it->clear();
    m_child->erase(it);
}

void Xml::remove(const char * name)
{
    return remove(string(name));
}

void Xml::remove(const string & name)
{
    for (auto it = m_child->begin(); it != m_child->end();)
    {
        if (it->name() == name)
        {
            it->clear();
            it = m_child->erase(it);
        }
        else
        {
            it++;
        }
    }
}

int Xml::size() const
{
    if (m_child == nullptr)
    {
        return 0;
    }
    return m_child->size();
}

void Xml::clear()
{
    if (m_name != nullptr)
    {
        delete m_name;
        m_name = nullptr;
    }
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }
    if (m_attrs != nullptr)
    {
        delete m_attrs;
        m_attrs = nullptr;
    }
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); it++)
        {
            it->clear();
        }
        delete m_child;
        m_child = nullptr;
    }
}

string Xml::str() const
{
    if (m_name == nullptr)
    {
        return "";
    }
    ostringstream os;
    os << "<";
    os << *m_name;
    if (m_attrs != nullptr)
    {
        for (auto it = m_attrs->begin(); it != m_attrs->end(); it++)
        {
            os << " " << it->first << "=\"" << (string)(it->second) << "\""; 
        }
    }
    os << ">";
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); it++)
        {
            os << it->str();
        }
    }
    if (m_text != nullptr)
    {
        os << *m_text;
    }
    os << "</" << *m_name << ">";
    return os.str();
}

bool Xml::load(const string & filename)
{
    Parser p;
    if (!p.load_file(filename))
    {
        return false;
    }
    *this = p.parse();
    return true;
}

bool Xml::save(const string & filename)
{
    ofstream fout(filename);
    if (fout.fail())
    {
        return false;
    }
    fout << str();
    fout.close();
    return true;
}

bool Xml::parse(const string & str)
{
    Parser p;
    if (!p.load_string(str))
    {
        return false;
    }
    *this = p.parse();
    return true;
}