#include <fstream>
#include <iostream>
#include <pqrs/json.hpp>

int main(void) {
  nlohmann::json json;
  json["number"] = 123;
  json["string"] = "abc";

  if (auto v = pqrs::json::find<int>(json, "number")) {
    std::cout << "number " << *v << std::endl;
  }
  if (auto v = pqrs::json::find<std::string>(json, "number")) {
    std::cout << "string " << *v << std::endl;
  }

  if (auto v = pqrs::json::find<int>(json, "string")) {
    std::cout << "number " << *v << std::endl;
  }
  if (auto v = pqrs::json::find<std::string>(json, "string")) {
    std::cout << "string " << *v << std::endl;
  }

  auto j = nlohmann::ordered_json::parse(std::ifstream("data/personal_tekezo_simple_vi_mode.json"));
  std::cout << pqrs::json::pqrs_formatter::format(j, {.indent_size = 4}) << std::endl;

  return 0;
}
