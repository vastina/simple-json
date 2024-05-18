
#include <type_traits>

namespace vastina {

enum class j_type{
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