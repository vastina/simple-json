#ifndef _LEXER_H_
#define _LEXER_H_

#include "base/io.hpp"

#include <functional>

namespace vastina {
namespace json {

enum class TOKEN
{
  String,
  Number,
  Comma,  // ,
  Colon,  // :
  Null,   // n
//  Dquote, // "
  Oparan, // [
  Cparan, // ]
  Obrac,  // {
  Cbrac,  // }
  True,   // t
  False,  // f
};

struct token_t
{
  TOKEN token_;
  string data_;
  // u32 line_;
  // u32 lineoffset_ {};

  token_t( TOKEN, const string&/*, u32, u32*/ );
};

class lexer
{
public:
  // only local use enum
  enum STATE
  {
    END = -1,
    NORMAL,
    ERROR
  };

private:
  std::vector<token_t> tokens_ {};
  Filer* buffer_;
  string current_ {};

  u32 offset_ {};
  //u32 line_ { 1 };
  //u32 lineoffset_ { 1 };

  STATE state {};

  STATE ParseWhiteSpace();
  // return non-zero mean fail
  i32 ParseNext( TOKEN, const std::function<bool( char )>&, u32 );

  // return non-zero mean fail and format error
  i32 ParseNumber();
  void forSingelWord( TOKEN, const string_view& );

  STATE Checker();
  STATE NextLine(); // for comment mod
  i32 reScan();

private:
  void WriteBack();

public:
  lexer( const string_view& );
  lexer( Filer* );
  ~lexer();

  i32 Parse();
  STATE Next();

  const std::vector<token_t>& getTokens();
};

}; // namespace json
}; // namespace vastina

#endif