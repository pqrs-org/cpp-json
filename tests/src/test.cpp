#include <boost/ut.hpp>
#include <pqrs/json.hpp>

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "find"_test = [] {
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

    expect(pqrs::json::find<int>(json, "number") == 123);
    expect(pqrs::json::find<int>(json, "dummy") == std::nullopt);
    expect(pqrs::json::find<int>(json, "string") == std::nullopt);
    expect(pqrs::json::find<int>(json, "array") == std::nullopt);
    expect(pqrs::json::find<int>(json, "object") == std::nullopt);
    expect(pqrs::json::find<int>(nlohmann::json(), "key") == std::nullopt);

    expect(pqrs::json::find<std::string>(json, "number") == std::nullopt);
    expect(pqrs::json::find<std::string>(json, "dummy") == std::nullopt);
    expect(pqrs::json::find<std::string>(json, "string") == std::string("abc"));
    expect(pqrs::json::find<std::string>(json, "array") == std::nullopt);
    expect(pqrs::json::find<std::string>(json, "object") == std::nullopt);

    expect(pqrs::json::find_array(json, "number") == std::nullopt);
    expect(pqrs::json::find_array(json, "dummy") == std::nullopt);
    expect(pqrs::json::find_array(json, "string") == std::nullopt);
    expect(pqrs::json::find_array(json, "array") != std::nullopt);
    expect(pqrs::json::find_array(json, "array")->value().dump() == json["array"].dump());
    expect(pqrs::json::find_array(json, "object") == std::nullopt);

    expect(pqrs::json::find_object(json, "number") == std::nullopt);
    expect(pqrs::json::find_object(json, "dummy") == std::nullopt);
    expect(pqrs::json::find_object(json, "string") == std::nullopt);
    expect(pqrs::json::find_object(json, "array") == std::nullopt);
    expect(pqrs::json::find_object(json, "object") != std::nullopt);
    expect(pqrs::json::find_object(json, "object")->value().dump() == json["object"].dump());

    expect(pqrs::json::find_copy(json, "number", nlohmann::json("fallback_value")) == nlohmann::json(123));
    expect(pqrs::json::find_copy(json, "string", nlohmann::json("fallback_value")) == nlohmann::json("abc"));
    expect(pqrs::json::find_copy(json, "array", nlohmann::json("fallback_value")) == json["array"]);
    expect(pqrs::json::find_copy(json, "object", nlohmann::json("fallback_value")) == json["object"]);
    expect(pqrs::json::find_copy(json, "unknown", nlohmann::json("fallback_value")) == nlohmann::json("fallback_value"));
  };

  "marshal_error"_test = [] {
    try {
      throw pqrs::json::marshal_error("hello world");
      expect(false);
    } catch (pqrs::json::marshal_error& ex) {
      expect(std::string("hello world") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "unmarshal_error"_test = [] {
    try {
      throw pqrs::json::unmarshal_error("hello world");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("hello world") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "requires_array"_test = [] {
    auto json = nlohmann::json::array();
    pqrs::json::requires_array(json, "`json`");
  };

  "requires_array throw"_test = [] {
    auto json = nlohmann::json::object({
        {"key1", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
        {"key2", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
        {"key3", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
        {"key4", "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."},
    });

    try {
      pqrs::json::requires_array(json, "`json`");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("`json` must be array, but is `{") +
                 "\"key1\":\"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\"," +
                 "\"key2\":\"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore...`" ==
             ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "requires_boolean"_test = [] {
    nlohmann::json json = true;
    pqrs::json::requires_boolean(json, "`json`");
  };

  "requires_boolean throw"_test = [] {
    auto json = nlohmann::json::array();

    try {
      pqrs::json::requires_boolean(json, "`json`");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("`json` must be boolean, but is `[]`") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "requires_number"_test = [] {
    nlohmann::json json = 123;
    pqrs::json::requires_number(json, "`json`");
  };

  "requires_number throw"_test = [] {
    auto json = nlohmann::json::array();

    try {
      pqrs::json::requires_number(json, "`json`");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("`json` must be number, but is `[]`") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "requires_object"_test = [] {
    auto json = nlohmann::json::object();
    pqrs::json::requires_object(json, "`json`");
  };

  "requires_object throw"_test = [] {
    auto json = nlohmann::json::array();

    try {
      pqrs::json::requires_object(json, "`json`");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
            expect(std::string("`json` must be object, but is `[]`") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  "requires_string"_test = [] {
    nlohmann::json json = "example";
    pqrs::json::requires_string(json, "`json`");
  };

  "requires_string throw"_test = [] {
    auto json = nlohmann::json::array();

    try {
      pqrs::json::requires_string(json, "`json`");
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("`json` must be string, but is `[]`") == ex.what());
    } catch (...) {
      expect(false);
    }
  };

  return 0;
}
