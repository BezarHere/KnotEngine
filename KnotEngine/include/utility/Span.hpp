#pragma once
#include <stdexcept>

namespace kt
{
  template <typename _T>
  class Span
  {
  public:
    typedef _T value_type;

    void clear();

    inline Span(size_t count) : m_length{ count }, m_data{ new value_type[count] } {
    }

    inline Span(Span &&move) noexcept
      : m_data{ move.m_data }, m_length{ move.m_length } {
      move.m_data = nullptr;
    }

    inline Span &operator=(Span &&move) noexcept {
      clear();
      m_data = move.m_data;
      m_length = move.m_length;

      move.m_data = nullptr;
    }

    inline ~Span() {
      clear();
    }

    inline bool empty() const noexcept {
      return m_length == 0;
    }

    inline size_t size() const {
      return m_length;
    }
    inline size_t length() const {
      return m_length;
    }

    inline value_type &operator[](size_t pos) {
      if (pos > m_length)
      {
        throw std::out_of_range("pos");
      }

      return m_data[pos];
    }

    inline const value_type &operator[](size_t pos) const {
      if (pos > m_length)
      {
        throw std::out_of_range("pos");
      }

      return m_data[pos];
    }

    inline value_type *begin() {
      return m_data;
    }
    inline const value_type *begin() const {
      return m_data;
    }

    inline value_type *end() {
      return m_data + m_length;
    }
    inline const value_type *end() const {
      return m_data + m_length;
    }

    inline value_type *data() {
      return m_data;
    }
    inline const value_type *data() const {
      return m_data;
    }

  private:
    size_t m_length = 0;
    value_type *m_data = nullptr;
  };

  template<typename _T>
  inline void Span<_T>::clear() {
    delete[] m_data;
    m_data = nullptr;
    m_length = 0;
  }
}
