#pragma once
#include "Window.hpp"
#include "Object.hpp"

#include <array>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>

namespace kt
{
	struct EngineThreading
	{
		struct TMutex : public std::mutex
		{
			inline void lock() {
				m_locked = true;
				std::mutex::lock();
			}

			inline bool try_lock() noexcept {
				m_locked = std::mutex::try_lock();
				return m_locked;
			}

			inline void unlock() {
				m_locked = false;
				std::mutex::unlock();
			}

			inline bool is_locked() const {
				return m_locked;
			}

		private:
			bool m_locked;
		};

		typedef TMutex mutex_type;
		typedef std::scoped_lock<mutex_type> lock_type;
		typedef std::thread thread_type;

		struct ThreadInstance
		{
			lock_type acquire();
			bool is_running() const;

			thread_type thread;
			mutex_type mutex;
		};

		// actual to-gpu drawing is on the main thread
		ThreadInstance draw;
		ThreadInstance update;
		ThreadInstance physics;
	};

	class Engine
	{
	public:
		typedef std::chrono::nanoseconds iteration_interval;
		typedef std::unique_ptr<Object> SPObject;

		Engine();

		void start();

		static constexpr iteration_interval calc_interval(iteration_interval::rep ticks) {
			return iteration_interval(
				iteration_interval::period::den / (ticks * iteration_interval::period::num)
			);
		}

	private:
		void _draw_loop();
		void _update_loop();
		void _physics_loop();

		void _draw_objects();
		void _update_objects();
		void _update_physics();

	private:
		struct TickIntervalData
		{
			inline TickIntervalData(size_t _ticks)
				: ticks{_ticks} {
			}

			inline void update_cached() {
				cached_interval = calc_interval(ticks);
			}

			size_t ticks;
			iteration_interval cached_interval = calc_interval(ticks);
		};

		bool m_running;

		std::unique_ptr<Window> m_window;

		// private:
		std::vector<SPObject> m_objects;

		std::array<std::vector<Object *>, 6> m_objects_collection;

		TickIntervalData m_fps = 60;
		TickIntervalData m_ups = 40;
		TickIntervalData m_phy_ups = 40;

		EngineThreading m_threads;
	};
}