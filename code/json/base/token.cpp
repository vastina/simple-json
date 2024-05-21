#include "base/token.hpp"

#include "base/util.hpp"
#include <stdexcept>

namespace vastina {
namespace json {

token_t::token_t( TOKEN tk, const string& data/*, u32 line, u32 lineoffset*/ )
  : token_( tk ), data_( data )/*, line_( line ), lineoffset_( lineoffset ) */
{}

lexer::lexer( const string_view& filename ) : buffer_ { new Filer( filename ) } {}
lexer::lexer( Filer* buffer ) : buffer_ { buffer } {
  current_ = buffer_->PopFront();
}

lexer::~lexer()
{
  tokens_.clear();
}

lexer::STATE lexer::Checker(){
  if( offset_ >= current_.size() ){
    if( buffer_->Empty() ){
      return STATE::END;
    } else {
      current_ = buffer_->PopFront();
      offset_ = 0;
    }
  }
  return STATE::NORMAL;
}

lexer::STATE lexer::ParseWhiteSpace(){
  do {
    if(STATE::END == Checker()) return STATE::END;
    if (!isWhiteSpace( current_[offset_] )) break;
    else offset_++;
  } while ( true );
  return STATE::NORMAL;
}

lexer::STATE lexer::NextLine(){
  do {
    if(STATE::END == Checker()) return STATE::END;
    if ('\n' == current_[offset_]){
      offset_++;
      break;
    }
    offset_++;
  } while ( true );
  return STATE::NORMAL;
}

i32 lexer::ParseNext( TOKEN target, const std::function<bool( char )>& Endjudge, u32 except_len ){
  if(STATE::END == Checker()) return 0;
  u32 last_offset {offset_};
  while(Endjudge(current_[offset_]))
  {
    offset_++;
  }
  if(offset_ - last_offset != except_len && except_len > 0) return -1;
  tokens_.push_back( token_t(target ,string(current_.substr(last_offset, offset_-last_offset))) );
  return 0;
}

void lexer::forSingelWord( TOKEN target, const string_view&data ){
  // assert
  tokens_.push_back( token_t(target, string(data)) );
  offset_ ++;
}

i32 lexer::ParseNumber(){
  if(STATE::END == Checker()) return 0;
  u32 last_offset {offset_};
  // todo
  while(CharType(current_[offset_]) == CHARTYPE::NUM)
  {
    offset_++;
  }
  tokens_.push_back( token_t(TOKEN::Number ,{current_.data()+last_offset, offset_-last_offset}) );
  return 0;
}

lexer::STATE lexer::Next(){
  if ( buffer_->Empty() )
    return STATE::END;
  state = ParseWhiteSpace();
  if ( state == STATE::END )
    return state;

  // some compilers have a extension, but some not
  // case '0'...'9'
  // case 'a'...'z'
  switch (CharType( current_[offset_] )) {
    case CHARTYPE::CHAR: {
      constexpr auto helper{[](const string_view& target){
        return [&target,i{0u}](char ch)mutable{
          if(i<target.size()) return ch == target[i++];
          return !(CharType(ch) == CHARTYPE::OTHER);
        };
      }};
      switch (current_[offset_ ]) {
        case 'n':{
          constexpr static string_view sv {"null"};
          EXCEPT_ZERO(ParseNext, TOKEN::Null,
            helper(sv), sv.size());
          break;
        }
        case 't':{
          constexpr static string_view sv {"true"};
          EXCEPT_ZERO(ParseNext, TOKEN::True,
            helper(sv), sv.size());
          break;
        }
        case 'f':{
          constexpr static string_view sv {"false"};
          EXCEPT_ZERO(ParseNext, TOKEN::False,
            helper(sv), sv.size());
          break;
        }
        default:{
          throw std::runtime_error("bad json format");
        }
      }
      break;
    }
    case CHARTYPE::NUM: {
      if(ParseNumber()) return STATE::ERROR;
      break;
    }
    case CHARTYPE::OTHER: {
      switch (current_[offset_]) {
        case ',':{
          constexpr static string_view sv { ","};
          forSingelWord(TOKEN::Comma, sv);
          break;
        }
        case ':':{
          constexpr static string_view sv { ":"};
          forSingelWord(TOKEN::Colon, sv);
          break;
        }
        case '"':{
          // todo, support ParserConfig::more_str

          offset_++;
          EXCEPT_ZERO(ParseNext, TOKEN::String,
            [](char ch){ return ch != '"'; }, 0);
          offset_++;
          break;
        }
        case '[':{
          constexpr static string_view sv { "["};
          forSingelWord(TOKEN::Oparan, sv);
          break;
        }
        case ']':{
          constexpr static string_view sv { "]"};
          forSingelWord(TOKEN::Cparan, sv);
          break;
        }
        case '{':{
          constexpr static string_view sv { "{"};
          forSingelWord(TOKEN::Obrac, sv);
          break;
        }
        case '}':{
          constexpr static string_view sv { "}"};
          forSingelWord(TOKEN::Cbrac, sv);
          break;
        }
        default:{
          //throw std::runtime_error("bad json format");
          break;
        }
      }
      break;
    }
  }
  return STATE::NORMAL;
}

i32 lexer::Parse(){
  STATE state;
  while ( true ) {
    state = this->Next();
    if ( STATE::ERROR == state )
      return -1;
    else if ( STATE::END == state )
      break;
  }
  return 0;
}

const std::vector<token_t>& lexer::getTokens(){
  return tokens_;
}

}; // namespace json
}; // namespace vastina