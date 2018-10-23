#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

int main() {
    auto test = Node4<void *>();

    test.find(std::byte(128));
    fmt::print("Hello {}!", "world");

    rapidjson::Document d;
    d.Parse(R"({"project":"rapidjson","stars":10})");

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    fmt::print("{}", buffer.GetString());

    return 0;
}