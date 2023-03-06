//
// Created by Arthur Abel Motelevicz on 19/03/22.
//

#include "Json.h"
#include <rapidjson/filewritestream.h>

const rapidjson::Value bb::Json::kEmptyArray = rapidjson::Value(rapidjson::kArrayType);
const rapidjson::Value bb::Json::kEmptyObject = rapidjson::Value(rapidjson::kObjectType);

rapidjson::Document bb::Json::document()
{
    auto document = rapidjson::Document();
    document.SetObject();

    return document;
}

rapidjson::Value bb::Json::array()
{
    return rapidjson::Value(rapidjson::kArrayType);
}

rapidjson::Value bb::Json::object()
{
    return rapidjson::Value(rapidjson::kObjectType);
}

bool bb::Json::getBool(const rapidjson::Value& object, const char* key, bool defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsBool()) {
        return defaultValue;
    }

    return object[key].GetBool();
}

int bb::Json::getInt(const rapidjson::Value& object, const char* key, int defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsInt()) {
        return defaultValue;
    }

    return object[key].GetInt();
}

float bb::Json::getFloat(const rapidjson::Value& object, const char* key, float defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsNumber()) {
        return defaultValue;
    }

    return object[key].GetFloat();
}

int64_t bb::Json::getLong(const rapidjson::Value& object, const char* key, int64_t defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsInt64()) {
        return defaultValue;
    }

    return object[key].GetInt64();
}

double bb::Json::getDouble(const rapidjson::Value& object, const char* key, double defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsNumber()) {
        return defaultValue;
    }

    return object[key].GetDouble();
}

std::string bb::Json::getString(const rapidjson::Value& object, const char* key, const std::string& defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsString()) {
        return defaultValue;
    }

    return object[key].GetString();
}

const char* bb::Json::getStringConst(const rapidjson::Value& object, const char* key, const char* defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsString()) {
        return defaultValue;
    }

    return object[key].GetString();
}

std::vector<std::string> bb::Json::getStringVector(const rapidjson::Value& object, const char* key)
{
    return getVector<std::string, const char*, &rapidjson::Value::IsString, &rapidjson::Value::GetString>(object, key);
}

std::vector<int> bb::Json::getIntVector(const rapidjson::Value& object, const char* key)
{
    return getVector<int, int, &rapidjson::Value::IsInt, &rapidjson::Value::GetInt>(object, key);
}

std::vector<float> bb::Json::getFloatVector(const rapidjson::Value& object, const char* key)
{
    return getVector<float, float, &rapidjson::Value::IsNumber, &rapidjson::Value::GetFloat>(object, key);
}

bool bb::Json::parse(const std::string& json, rapidjson::Document& document)
{
    document.Parse(json.c_str(), json.length());

    if (document.HasParseError()) {
        return false;
    }

    return true;
}

bool bb::Json::parseFile(const std::string& filePath, rapidjson::Document& document)
{
    FILE* fp = fopen(filePath.c_str(), "r"); // non-Windows use "r"

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    document.ParseStream(is);

    fclose(fp);

    return !document.IsNull();
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, bool boolValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, boolValue, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int intValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, intValue, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, float floatValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, floatValue, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int64_t int64Value)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, (int64_t)int64Value, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, double doubleValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, doubleValue, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const std::string& stringValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    rapidjson::Value value(stringValue.c_str(), (unsigned int)stringValue.size(), document.GetAllocator());

    object.AddMember(name, value, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue)
{
    set(document, object, key, stringValue, strlen(stringValue));
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue, std::size_t size)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    rapidjson::Value value(stringValue, (unsigned int)size, document.GetAllocator());

    object.AddMember(name, value, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, rapidjson::Value& value)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, value, document.GetAllocator());
}

void bb::Json::set(rapidjson::Document &document, const std::string &key, rapidjson::Value &value)
{
    set(document, document, key, value);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, bool boolValue)
{
    set(document, document, key, boolValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, int intValue)
{
    set(document, document, key, intValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, float floatValue)
{
    set(document, document, key, floatValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, int64_t int64Value)
{
    set(document, document, key, int64Value);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, double doubleValue)
{
    set(document, document, key, doubleValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, const std::string& stringValue)
{
    set(document, document, key, stringValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, const char* stringValue)
{
    set(document, document, key, stringValue);
}

void bb::Json::set(rapidjson::Document& document, const std::string& key, const char* stringValue, std::size_t size)
{
    set(document, document, key, stringValue, size);
}

std::string bb::Json::toString(const rapidjson::Value& value, bool pretty)
{
    rapidjson::StringBuffer buffer;

    if (pretty)
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        value.Accept(writer);
    }
    else
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        value.Accept(writer);
    }

    return std::string(buffer.GetString(), buffer.GetSize());
}


void bb::Json::append(rapidjson::Document& document, bool boolValue)
{
    append(document, document, boolValue);
}

void bb::Json::append(rapidjson::Document& document, int intValue)
{
    append(document, document, intValue);
}

void bb::Json::append(rapidjson::Document& document, float floatValue)
{
    append(document, document, floatValue);
}

void bb::Json::append(rapidjson::Document& document, int64_t int64Value)
{
    append(document, document, int64Value);
}

void bb::Json::append(rapidjson::Document& document, double doubleValue)
{
    append(document, document, doubleValue);
}

void bb::Json::append(rapidjson::Document& document, const std::string& stringValue)
{
    append(document, document, stringValue);
}

void bb::Json::append(rapidjson::Document& document, const char* stringValue)
{
    append(document, document, stringValue);
}

void bb::Json::append(rapidjson::Document& document, const char* stringValue, std::size_t size)
{
    append(document, document, stringValue, size);
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& value)
{
    append(document, document, value);
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, bool boolValue)
{
    object.PushBack(boolValue, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, int intValue)
{
    object.PushBack(intValue, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, float floatValue)
{
    object.PushBack(floatValue, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, int64_t int64Value)
{
    object.PushBack((int64_t)int64Value, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, double doubleValue)
{
    object.PushBack(doubleValue, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, const std::string& stringValue)
{
    rapidjson::Value value(stringValue.c_str(), (unsigned int)stringValue.size(), document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue)
{
    rapidjson::Value value(stringValue, (unsigned int)strlen(stringValue), document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue, std::size_t size)
{
    rapidjson::Value value(stringValue, (unsigned int)size, document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void bb::Json::append(rapidjson::Document& document, rapidjson::Value& object, rapidjson::Value& value)
{
    object.PushBack(value, document.GetAllocator());
}

bool bb::Json::exists(const rapidjson::Value& value, const std::string& memberName)
{
    return value.IsObject() && value.HasMember(memberName.c_str());
}

bool bb::Json::existsPath(rapidjson::Value &value, const std::string &path)
{
    rapidjson::Value* temp = &value;

//    auto components = BAString::split(path, '/');
//
//    for (auto comp : components)
//    {
//        if (comp.size() != 0)
//        {
//            if (temp->IsObject() && temp->HasMember(comp.c_str())) {
//                temp = &(*temp)[comp.c_str()];
//            }
//            else if (temp->IsArray() && BAString::isNumber(comp))
//            {
//                int index = BAString::getInt(comp);
//
//                if (index < 0 || index >= temp->Size()) {
//                    return false;
//                }
//
//                temp = &(*temp)[index];
//            }
//            else {
//                return false;
//            }
//        }
//    }

    return true;
}

void bb::Json::save(const rapidjson::Document& document, const std::string& file)
{
    FILE* fp = fopen(file.c_str(), "wb"); // non-Windows use "w"

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    document.Accept(writer);

    fclose(fp);
}

void bb::Json::save(const std::string& strJson, const std::string& file)
{
    std::ofstream outFile(file.c_str());
    outFile << strJson;
}

bool bb::Json::hasBool(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsBool();
}

bool bb::Json::hasNumber(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsNumber();
}

bool bb::Json::hasString(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsString();
}

bool bb::Json::hasObject(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsObject() || member.IsNull();
}

bool bb::Json::hasArray(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsArray() || member.IsNull();
}

std::string bb::Json::stringfy(const rapidjson::Document &document) {
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document.Accept(writer);
    return strbuf.GetString();
}

