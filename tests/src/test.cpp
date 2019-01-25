#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/json.hpp>

TEST_CASE("find") {
  nlohmann::json json;
  json["number"] = 123;
  json["string"] = "abc";
  json["array"] = nlohmann::json::array();
  json["array"].push_back(1);
  json["array"].push_back(2);
  json["array"].push_back(3);
  json["object"] = nlohmann::json::object();
  json["object"]["a"] = 1;
  json["object"]["b"] = 2;
  json["object"]["c"] = 3;

  REQUIRE(pqrs::json::find<int>(json, "number") == 123);
  REQUIRE(pqrs::json::find<int>(json, "dummy") == std::nullopt);
  REQUIRE(pqrs::json::find<int>(json, "string") == std::nullopt);
  REQUIRE(pqrs::json::find<int>(json, "array") == std::nullopt);
  REQUIRE(pqrs::json::find<int>(json, "object") == std::nullopt);
  REQUIRE(pqrs::json::find<int>(nlohmann::json(), "key") == std::nullopt);

  REQUIRE(pqrs::json::find<std::string>(json, "number") == std::nullopt);
  REQUIRE(pqrs::json::find<std::string>(json, "dummy") == std::nullopt);
  REQUIRE(pqrs::json::find<std::string>(json, "string") == std::string("abc"));
  REQUIRE(pqrs::json::find<std::string>(json, "array") == std::nullopt);
  REQUIRE(pqrs::json::find<std::string>(json, "object") == std::nullopt);

  REQUIRE(pqrs::json::find_array(json, "number") == std::nullopt);
  REQUIRE(pqrs::json::find_array(json, "dummy") == std::nullopt);
  REQUIRE(pqrs::json::find_array(json, "string") == std::nullopt);
  REQUIRE(pqrs::json::find_array(json, "array"));
  REQUIRE(pqrs::json::find_array(json, "array")->value().dump() == json["array"].dump());
  REQUIRE(pqrs::json::find_array(json, "object") == std::nullopt);

  REQUIRE(pqrs::json::find_object(json, "number") == std::nullopt);
  REQUIRE(pqrs::json::find_object(json, "dummy") == std::nullopt);
  REQUIRE(pqrs::json::find_object(json, "string") == std::nullopt);
  REQUIRE(pqrs::json::find_object(json, "array") == std::nullopt);
  REQUIRE(pqrs::json::find_object(json, "object"));
  REQUIRE(pqrs::json::find_object(json, "object")->value().dump() == json["object"].dump());

  REQUIRE(pqrs::json::find_copy(json, "number", nlohmann::json("fallback_value")) == nlohmann::json(123));
  REQUIRE(pqrs::json::find_copy(json, "string", nlohmann::json("fallback_value")) == nlohmann::json("abc"));
  REQUIRE(pqrs::json::find_copy(json, "array", nlohmann::json("fallback_value")) == json["array"]);
  REQUIRE(pqrs::json::find_copy(json, "object", nlohmann::json("fallback_value")) == json["object"]);
  REQUIRE(pqrs::json::find_copy(json, "unknown", nlohmann::json("fallback_value")) == nlohmann::json("fallback_value"));
}
