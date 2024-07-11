#pragma once
#include "Defs.hpp"
#include "Object.hpp"
#include "Log.hpp"
#include <memory>
#include <array>

template <typename _T, size_t _ChunkSize = 16>
class ChunkAllocator
{
public:
	typedef _T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	static constexpr size_type chunk_size = _ChunkSize;

	inline value_type *allocate(size_t count);
	inline void deallocate(value_type *ptr, size_t count);

private:
	typedef uint8_t NakedValueType[sizeof(_T)];
	struct Chunk;

	typedef std::unique_ptr<Chunk> UPChunk;

	struct Chunk
	{
		typedef std::unique_ptr<Chunk> UPChunk;
		inline Chunk(Chunk *prev) : _prev{prev} {}

		inline _T *get(const size_t pos) {
			return reinterpret_cast<_T *>(data[pos]);
		}

		inline const _T *get(const size_t pos) const {
			return reinterpret_cast<const _T *>(data[pos]);
		}

		inline bool empty() const {
			for (bool value : occupation)
			{
				if (value)
				{
					return false;
				}
			}
			return true;
		}

		inline bool full() const {
			for (bool value : occupation)
			{
				if (!value)
				{
					return false;
				}
			}
			return true;
		}

		// my link to my parent
		inline UPChunk &my_link() const {
			return _parent->_child;
		}

		inline void reparent(UPChunk &link) {
			// make chunk of link my child (deleting my child if i had any)
			_child.reset(link.release());
			// tell the original link (parent to child link of old parent) to reset to null
			// i.e there is no longer a child of the old parent
			link.reset(nullptr);
		}

		std::array<bool, chunk_size> occupation = {};
		NakedValueType data[chunk_size] = {};
		UPChunk _child = {nullptr};
		Chunk *_parent;
	};

	typedef std::unique_ptr<Chunk> UPChunk;

	inline static Chunk *_create_chunk(Chunk *prev) {
		return new Chunk(prev);
	}

	template <typename _T>
	struct linked_list
	{
		inline void push(Chunk *chunk) {
			// empty list, start at chunk
			if (current == nullptr)
			{
				first.reset(chunk);
				current = chunk;
				return;
			}
			// [current]->[null] to [current]->[child (chunk)]
			current->_child.reset(chunk);

			// [current]->[child] to [child (now current)]->[null (future grand child)] 
			current = current->_child.get();
		}

		inline void push_new() {
			push(_create_chunk());
		}

		inline void move_current(linked_list &from) {
			// no current or same family
			if (from.current == nullptr || from.first == this->first)
			{
				return;
			}

			if (from.current->_parent != nullptr)
			{
				// [current]->[something] & [from.parent]->[from.current]
				// 
				// to
				//
				// [current]->[from.current (if something isn't null, it's deleted)]
				// & [from.grandparent]->[from.parent]

				Chunk *from_parent = from.current->_parent;
				current->reparent(from.current->my_link());
				from.current = from_parent;
			}
			else
			{
				push(from.current);
				from.current = nullptr;
				from.first.reset();
			}

			// [current]->[child] to [child (now current)]->[null (future grand child)] 
			current = current->_child.get();
		}

		std::unique_ptr<_T> first; // first
		_T *current = first.get();
	};

	linked_list<Chunk> m_vacant;
	linked_list<Chunk> m_occupied;
};

typedef ChunkAllocator<Object, 16> CO;
constexpr auto size = sizeof(CO::value_type);

template<typename _T, size_t _ChunkSize>
inline ChunkAllocator<_T, _ChunkSize>::value_type *
ChunkAllocator<_T, _ChunkSize>::allocate(size_t count) {
	if (m_vacant.current->full())
	{
		m_occupied.move_current(m_vacant);

		if (m_vacant.current == nullptr)
		{
			m_vacant.push_new();
		}
	}



}

template<typename _T, size_t _ChunkSize>
inline void ChunkAllocator<_T, _ChunkSize>::deallocate(value_type *ptr, size_t count) {
}
