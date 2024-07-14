#pragma once
#include <inttypes.h>

namespace kt
{

  template <size_t _Bits>
  class Bitfield
  {
  public:
    static constexpr size_t bits_count = _Bits;
    static constexpr bool has_carry_byte = !!(bits_count % 8);
    static constexpr size_t bytes_count = (bits_count / 8) + has_carry_byte;
    static constexpr size_t carry_bits_count = (bytes_count * 8) - bits_count;

    static constexpr uint8_t carry_byte_full_value = (1 << (carry_bits_count)) - 1;

    inline void fill(bool value) {
      for (size_t i = 0; i < bytes_count; i++)
      {
        m_field[i] = value * 255;
      }
    }

    inline void clear() {
      for (size_t i = 0; i < bytes_count; i++)
      {
        m_field[i] = 0;
      }
    }

    inline bool all() const {
      for (size_t i = 0; i < (bytes_count - has_carry_byte); i++)
      {
        if (m_field[i] != 255)
        {
          return false;
        }
      }

      if constexpr (has_carry_byte && carry_bits_count != 0)
      {
        if (m_field[bytes_count - 1] != carry_byte_full_value)
        {
          return false;
        }
      }

      return true;
    }

    inline void turn_on(size_t index) {
      m_field[index >> 3] |= 1U << (index & 7);
    }

    inline void turn_off(size_t index) {
      m_field[index >> 3] &= ~(1U << (index & 7));
    }

    inline void set(size_t index, bool value) {
      if (value)
      {
        turn_on(index);
      }
      else
      {
        turn_off(index);
      }
    }

    inline bool get(size_t index) const {
      return m_field[index >> 3] & (1U << (index & 7));
    }

    inline void toggle(size_t index) {
      this->set(index, !this->get(index));
    }

  private:
    uint8_t m_field[bytes_count] = {};
  };

}
