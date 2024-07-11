#pragma once

template <typename _T>
class Cached
{
public:
	typedef _T value_type;

	inline bool is_dirty() const noexcept {
		return m_dirty;
	}

	inline void mark_dirty() { m_dirty = true; }
	inline void mark_clean() { m_dirty = false; }

	inline const value_type &get() const noexcept {
		return m_value;
	}

	inline value_type &get() noexcept {
		return m_value;
	}

	inline value_type *operator->() noexcept { return &m_value; }
	inline const value_type *operator->() const noexcept { return &m_value; }

	inline value_type &operator*() noexcept { return m_value; }
	inline const value_type &operator*() const noexcept { return m_value; }

	inline operator value_type *() noexcept { return &m_value; }
	inline operator const value_type *() const noexcept { return &m_value; }


private:
	value_type m_value;
	bool m_dirty;
};

