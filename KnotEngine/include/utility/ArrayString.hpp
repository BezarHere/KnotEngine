#pragma once
#include <string>
#include <stdexcept>

namespace kt
{
  template <size_t _MaxLen = 40, typename _T>
  class BasicArrayString
  {
  public:
    static constexpr size_t max_length = _MaxLen;

    typedef _T value_type;
    typedef _T char_type;
    typedef std::basic_string<value_type> string_type;
    typedef typename string_type::traits_type traits_type;

    constexpr BasicArrayString() = default;

    constexpr BasicArrayString(std::nullptr_t)
      : BasicArrayString() {
    }

    constexpr BasicArrayString(const char_type *cstr, const size_t length)
      : m_length{ std::min(max_length, length) } {
      (void)memcpy(m_data, cstr, sizeof(*m_data) * m_length);
      m_data[m_length] = char_type();
    }

    constexpr explicit BasicArrayString(const char_type _char)
      : m_length{ _char != char_type() } {
      // filling the string with null chars
      if (m_length)
      {
        m_data[m_length - 1] = _char;
      }

      m_data[m_length] = char_type();
    }

    constexpr explicit BasicArrayString(const size_t count, const char_type _char)
      : m_length{ std::min(max_length, count) } {
      // filling the string with null chars
      if (_char == char_type())
      {
        m_length = 0;
      }
      else
      {
        for (size_t i = 0; i < m_length; i++)
        {
          m_data[i] = _char;
        }
      }

      m_data[m_length] = char_type();
    }

    constexpr BasicArrayString(const char_type *cstr)
      : BasicArrayString(cstr, traits_type::length(cstr)) {
    }

    constexpr BasicArrayString(const string_type &str)
      : BasicArrayString(str.c_str(), str.length()) {
    }

    inline operator string_type() const {
      return string_type(m_data, m_length);
    }

    inline char_type &operator[](size_t pos) {
      if (pos > m_length)
      {
        throw std::out_of_range("pos");
      }

      return m_data[pos];
    }

    inline char_type operator[](size_t pos) const {
      if (pos > m_length)
      {
        throw std::out_of_range("pos");
      }

      return m_data[pos];
    }

    inline size_t length() const {
      return m_length;
    }
    inline size_t size() const {
      return m_length;
    }

    inline bool empty() const {
      return m_length == 0;
    }

    inline value_type *data() {
      return m_data;
    }
    inline const value_type *data() const {
      return m_data;
    }
    inline const value_type *c_str() const {
      return m_data;
    }

    inline value_type *begin() {
      return m_data;
    }
    inline value_type *end() {
      return m_data + m_length;
    }

    inline const value_type *begin() const {
      return m_data;
    }
    inline const value_type *end() const {
      return m_data + m_length;
    }

  private:
    size_t m_length = 0;
    char_type m_data[max_length + 1] = { 0 };
  };

  template <size_t MaxLen>
  using ArrayString = BasicArrayString<MaxLen, char>;

  static inline ArrayString<64> xx;
}
