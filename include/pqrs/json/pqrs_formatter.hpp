#pragma once

// (C) Copyright Takayama Fumihiko 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See https://www.boost.org/LICENSE_1_0.txt)

#include <nlohmann/json.hpp>
#include <sstream>
#include <stack>

namespace pqrs {
namespace json {
namespace pqrs_formatter {

// The custom JSON formatter follows these rules:
//
// - Arrays are kept on a single line. However, they are spread across multiple lines in the following cases:
//   - If the size is 2 or more and they contain arrays or objects as values.
//   - If they contain multi-line values.
// - Objects with only one key and a single-line value are kept on a single line.
//
// ```json
// {
//     "bool": true,
//     "double": 123.456,
//     "int": 42,
//     "multi_line_array1": [
//         [1, 2, 3, 4],
//         [5, 6, 7, 8]
//     ],
//     "multi_line_array2": [
//         { "key": 1 },
//         { "key": 2 }
//     ],
//     "multi_line_object1": {
//         "key1": 1,
//         "key2": 2
//     },
//     "multi_line_object2": {
//         "key": {
//             "key1": 1,
//             "key2": 2
//         }
//     },
//     "null": null,
//     "single_line_array1": [],
//     "single_line_array2": [1, 2, 3, 4],
//     "single_line_array3": [[1, 2, 3, 4]],
//     "single_line_array4": [{ "key": "value" }],
//     "single_line_object1": {},
//     "single_line_object2": { "key": "value" },
//     "single_line_object3": { "key": [1, 2, 3, 4] },
//     "single_line_object4": { "key1": { "key2": { "key3": "value3" } } }
// }
// ```

namespace impl {

bool multi_line(const nlohmann::json& json) {
  if (json.is_object()) {
    if (json.size() == 0) {
      return false;
    }

    if (json.size() == 1) {
      return multi_line(json.begin().value());
    }

    if (json.size() > 1) {
      return true;
    }

  } else if (json.is_array()) {
    if (json.size() == 0) {
      return false;
    }

    if (json.size() == 1) {
      return multi_line(json[0]);
    }

    for (const auto& j : json) {
      if (j.is_object() || j.is_array()) {
        return true;
      }
    }
  }

  return false;
}

void indent(std::ostringstream& ss,
            int indent_size,
            int indent_level) {
  for (int i = 0; i < indent_size * indent_level; ++i) {
    ss << ' ';
  }
}

void format(std::ostringstream& ss,
            const nlohmann::json& json,
            int indent_size,
            int indent_level) {
  if (json.is_object()) {
    if (!multi_line(json)) {
      //
      // Single-line object
      //

      if (json.empty()) {
        ss << "{}";

      } else {

        ss << "{ ";

        ss << std::quoted(json.begin().key()) << ": ";

        format(ss,
               json.begin().value(),
               indent_size,
               indent_level + 1);

        ss << " }";
      }

    } else {
      //
      // Multi-line object
      //

      ss << "{\n";

      bool first = true;
      for (const auto& [k, v] : json.items()) {
        if (!first) {
          ss << ",\n";
        }
        first = false;

        indent(ss,
               indent_size,
               indent_level + 1);

        ss << std::quoted(k) << ": ";
        format(ss,
               v,
               indent_size,
               indent_level + 1);
      }

      ss << '\n';

      indent(ss,
             indent_size,
             indent_level);

      ss << '}';
    }
  } else if (json.is_array()) {
    if (!multi_line(json)) {
      //
      // Single-line array
      //

      ss << '[';

      bool first = true;
      for (const auto& v : json) {
        if (!first) {
          ss << ", ";
        }
        first = false;

        format(ss,
               v,
               indent_size,
               indent_level + 1);
      }

      ss << ']';

    } else {
      //
      // Multi-line array
      //

      ss << "[\n";

      bool first = true;
      for (const auto& v : json) {
        if (!first) {
          ss << ",\n";
        }
        first = false;

        indent(ss,
               indent_size,
               indent_level + 1);

        format(ss,
               v,
               indent_size,
               indent_level + 1);
      }

      ss << '\n';

      indent(ss,
             indent_size,
             indent_level);

      ss << ']';
    }
  } else {
    ss << json;
  }
}

} // namespace impl

std::string format(const nlohmann::json& json,
                   int indent_size = 4) {
  std::ostringstream ss;
  impl::format(ss, json, indent_size, 0);
  return ss.str();
}

} // namespace pqrs_formatter
} // namespace json
} // namespace pqrs
