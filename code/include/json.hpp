#ifndef _VAS_JSON_H_
#define _VAS_JSON_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <cstdint>

namespace vastina {

using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::string_view;

namespace json {

enum class j_type
{
  object,
  array,
  string,
  number,
  boolean,
  null
};

// template<auto V>
// concept is_j_type = std::is_same_v<decltype(V), j_type>;

template<const j_type V>
//requires is_j_type<V>
class j_data {

public:
  constexpr decltype( auto ) getType() const { return V; }
};

}

}; //namespace vastina

#endif