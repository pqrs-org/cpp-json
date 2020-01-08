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

TEST_CASE("marshal_error") {
  REQUIRE_THROWS_AS(throw pqrs::json::marshal_error("hello world"),
                    pqrs::json::marshal_error);
  REQUIRE_THROWS_WITH(throw pqrs::json::marshal_error("hello world"),
                      "hello world");
}

TEST_CASE("unmarshal_error") {
  REQUIRE_THROWS_AS(throw pqrs::json::unmarshal_error("hello world"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(throw pqrs::json::unmarshal_error("hello world"),
                      "hello world");
}

TEST_CASE("requires_array") {
  auto json = nlohmann::json::array();
  pqrs::json::requires_array(json, "`json`");
}

TEST_CASE("requires_array throw") {
  auto json = nlohmann::json::object({
      {"key1", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
      {"key2", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
      {"key3", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
      {"key4", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
  });

  REQUIRE_THROWS_AS(pqrs::json::requires_array(json, "`json`"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(pqrs::json::requires_array(json, "`json`"),
                      std::string("`json` must be array, but is `{") +
                          "\"key1\":\"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\"," +
                          "\"key2\":\"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore...`");
}

TEST_CASE("requires_boolean") {
  nlohmann::json json = true;
  pqrs::json::requires_boolean(json, "`json`");
}

TEST_CASE("requires_boolean throw") {
  auto json = nlohmann::json::array();

  REQUIRE_THROWS_AS(pqrs::json::requires_boolean(json, "`json`"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(pqrs::json::requires_boolean(json, "`json`"),
                      "`json` must be boolean, but is `[]`");
}

TEST_CASE("requires_number") {
  nlohmann::json json = 123;
  pqrs::json::requires_number(json, "`json`");
}

TEST_CASE("requires_number throw") {
  auto json = nlohmann::json::array();

  REQUIRE_THROWS_AS(pqrs::json::requires_number(json, "`json`"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(pqrs::json::requires_number(json, "`json`"),
                      "`json` must be number, but is `[]`");
}

TEST_CASE("requires_object") {
  auto json = nlohmann::json::object();
  pqrs::json::requires_object(json, "`json`");
}

TEST_CASE("requires_object throw") {
  auto json = nlohmann::json::array();

  REQUIRE_THROWS_AS(pqrs::json::requires_object(json, "`json`"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(pqrs::json::requires_object(json, "`json`"),
                      "`json` must be object, but is `[]`");
}

TEST_CASE("requires_string") {
  nlohmann::json json = "example";
  pqrs::json::requires_string(json, "`json`");
}

TEST_CASE("requires_string throw") {
  auto json = nlohmann::json::array();

  REQUIRE_THROWS_AS(pqrs::json::requires_string(json, "`json`"),
                    pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(pqrs::json::requires_string(json, "`json`"),
                      "`json` must be string, but is `[]`");
}
