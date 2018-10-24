#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <range/v3/algorithm.hpp>
#include <gtest/gtest.h>
#include <cpr/cpr.h>

int main() {
    auto test = Node4<void *>();

    test.find(std::byte(128));
    fmt::print("Hello {}!", "world");

    auto d = rapidjson::Document(nullptr, 1024, nullptr);
    d.Parse(R"({"project":"rapidjson","stars":10})");

    auto buffer = rapidjson::StringBuffer(nullptr);
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer, nullptr);
    d.Accept(writer);

    fmt::print("{}\n", buffer.GetString());

    auto res = ranges::all_of(test, [](auto &a) noexcept { return a == nullptr; });
    assert(res);

    auto r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass"},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});

    return 0;
}