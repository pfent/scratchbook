#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <range/v3/algorithm.hpp>

int main() {
    auto test = Node4<void *>();

    test.find(std::byte(128));
    fmt::print("Hello {}!", "world");

    auto d = rapidjson::Document(nullptr, 1024, nullptr);
    d.Parse(R"({"project":"rapidjson","stars":10})");

    auto buffer = rapidjson::StringBuffer(nullptr);
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer, nullptr);
    d.Accept(writer);

    fmt::print("{}", buffer.GetString());

    ranges::all_of(test, [](auto &a) { return a == nullptr; });

    return 0;
}