#ifndef _VAS_UTILITY_H_
#define _VAS_UTILITY_H_

#include "json.hpp"
#include <format>
#include <string>

namespace vastina {

enum class CHARTYPE
{
  CHAR,
  NUM,
  OTHER
};

inline constexpr CHARTYPE CharType( char ch )
{
  if ( ch >= '0' && ch <= '9' )
    return CHARTYPE::NUM;
  if ( ( ch >= 'a' && ch <= 'z' ) || ( ch == '_' ) || ( ch >= 'A' && ch <= 'Z' ) )
    return CHARTYPE::CHAR;
  return CHARTYPE::OTHER;
}

inline constexpr bool isWhiteSpace( char ch )
{
  return ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );
}

inline bool Strcmp( const std::string& buffer, u32 offset, const string_view& temp )
{
  u32 len = temp.size();
  for ( u32 i = 0; i < len; i++ ) {
    if ( buffer.at( i + offset ) != temp.at( i ) )
      return false;
  }
  return true;
}

template<typename... Args>
void print( const std::string_view& fmt_str, Args&&... args )
{
  fputs( std::vformat( fmt_str, std::make_format_args( args... ) ).c_str(), stdout );
};

template<typename... Args>
std::string format( const std::string_view& fmt_str, Args&&... args )
{
  return std::vformat( fmt_str, std::make_format_args( args... ) );
}

#define LEAVE_MSG( msg ) ::vastina::print( "{}, at {} {}\n", msg, __FILE__, __LINE__ );
#define LEAVE_NOTHING ::vastina::print( "{} {}\n", __FILE__, __LINE__ );
#define EXIT_ERROR                                                                                                 \
  {                                                                                                                \
    LEAVE_NOTHING                                                                                                  \
    exit( -1 );                                                                                                    \
  }

#define tryCall( target, callee, ... )                                                                             \
  {                                                                                                                \
    if ( callee( __VA_ARGS__ ) != target ) {                                                                       \
      EXIT_ERROR;                                                                                                  \
    }                                                                                                              \
  }

#define EXCEPT_ZERO( callee, ... ) tryCall( 0, callee, __VA_ARGS__ )

}; // namespace vastina

#endif