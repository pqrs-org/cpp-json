#pragma once

#include <boost/ut.hpp>
#include <pqrs/json.hpp>

void run_pqrs_formatter_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "format"_test = [] {
    std::string json_string = R"(

{
    "bool": true,
    "double": 123.456,
    "int": 42,
    "multi_line_array1": [
        [1, 2, 3, 4],
        [5, 6, 7, 8]
    ],
    "multi_line_array2": [
        { "key": 1 },
        { "key": 2 }
    ],
    "multi_line_object1": {
        "key1": 1,
        "key2": 2
    },
    "multi_line_object2": {
        "key": {
            "key1": 1,
            "key2": 2
        }
    },
    "null": null,
    "single_line_array1": [],
    "single_line_array2": [1, 2, 3, 4],
    "single_line_array3": [[1, 2, 3, 4]],
    "single_line_array4": [{ "key": "value" }],
    "single_line_object1": {},
    "single_line_object2": { "key": "value" },
    "single_line_object3": { "key": [1, 2, 3, 4] },
    "single_line_object4": { "key1": { "key2": { "key3": "value3" } } }
}

)";

    pqrs::string::trim(json_string);
    auto json = nlohmann::json::parse(json_string);
    auto actual = pqrs::json::pqrs_formatter::format(json, 4);
    expect(json_string == actual);

    expect(json == nlohmann::json::parse(actual));
  };
}
