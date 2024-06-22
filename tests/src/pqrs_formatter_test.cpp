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
    "force_multi_line_array1": [
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.",
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
    ],
    "force_multi_line_array2": {
        "force_multi_line_array3": [
            1
        ]
    },
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
    auto actual = pqrs::json::pqrs_formatter::format(
        json,
        {
            .indent_size = 4,
            .force_multi_line_array_object_keys = {
                "force_multi_line_array1",
                // This test does not specify `force_multi_line_array2`.
                // By specifying `force_multi_line_array3`, it ensures that force_multi_line_array2 becomes multi-line without being explicitly specified.
                "force_multi_line_array3",
            },
        });
    expect(json_string == actual);

    expect(json == nlohmann::json::parse(actual));
  };

  "ordered_json"_test = [] {
    std::string json_string = R"(

{
  "one": 1,
  "two": 2,
  "three": 3
}

)";

    pqrs::string::trim(json_string);
    auto json = nlohmann::ordered_json::parse(json_string);
    auto actual = pqrs::json::pqrs_formatter::format(json, {.indent_size = 2});
    expect(json_string == actual);

    expect(json == nlohmann::ordered_json::parse(actual));
  };

  "corrupted_string"_test = [] {
    auto json = nlohmann::json::object();
    json["string"] = "hello\xe6world";

    expect(throws([json] {
      pqrs::json::pqrs_formatter::format(json, {});
    })) << "expected exception is not thrown";

    std::string expected = R"({ "string": "helloworld" })";
    auto actual = pqrs::json::pqrs_formatter::format(json, {.error_handler = nlohmann::json::error_handler_t::ignore});
    expect(expected == actual);
  };
}
