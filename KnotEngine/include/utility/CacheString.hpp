#pragma once
#include <string>

namespace kt
{
  template <typename _T, size_t _MaxCache = 40>
  class BasicCachedString
  {
  public:
    static constexpr size_t max_cache = _MaxCache;

    typedef _T value_type;
    typedef _T char_type;
    typedef std::basic_string<value_type> string_type;




  private:
    bool m_cached;
    char_type m_cache[max_cache] = { 0 };
    string_type m_str;
  };

  typedef BasicCachedString<char> CachedString;
}
