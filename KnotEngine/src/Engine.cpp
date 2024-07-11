#include "pch.h"
#include "Engine.hpp"

using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

template <typename _T, typename _Trans, typename _Interval>
inline void waited_iter(_T &&begin, const _T &&end, _Trans &&transformer, const _Interval &max_interval) {
	for (; begin != end; begin++)
	{
		const auto iteration_start = std::chrono::steady_clock::now();

		transformer(*begin);

		const auto time_left = max_interval - (std::chrono::steady_clock::now() - iteration_start);

		if (time_left.count() > 0)
		{
			std::this_thread::sleep_for(time_left);
		}
	}
}

namespace kt
{
	Engine::Engine()
		: m_threads{}, m_window{new Window()} {
	}

	void Engine::start() {
		m_threads.draw.thread = EngineThreading::thread_type([this]() {this->_draw_loop();});
	}

	void Engine::_draw_loop() {
		for (; m_running; m_running = false)
		{
			const auto iteration_start = std::chrono::steady_clock::now();

			{
				EngineThreading::lock_type lock = m_threads.draw.acquire();
				_draw_objects();
			}

			const auto time_left = m_fps.cached_interval - (std::chrono::steady_clock::now() - iteration_start);

			if (time_left.count() > 0)
			{
				std::this_thread::sleep_for(time_left);
			}
		}
	}

	void Engine::_update_loop() {
		while (m_running)
		{
			const auto iteration_start = std::chrono::steady_clock::now();

			_update_objects();

			const auto time_left = m_ups.cached_interval - (std::chrono::steady_clock::now() - iteration_start);

			if (time_left.count() > 0)
			{
				std::this_thread::sleep_for(time_left);
			}
		}
	}

	void Engine::_physics_loop() {
		while (m_running)
		{
			const auto iteration_start = std::chrono::steady_clock::now();

			_update_physics();

			const auto time_left = m_phy_ups.cached_interval - (std::chrono::steady_clock::now() - iteration_start);

			if (time_left.count() > 0)
			{
				std::this_thread::sleep_for(time_left);
			}
		}
	}

	void Engine::_draw_objects() {
		for (const SPObject &p_obj : m_objects)
		{
			p_obj->draw();
		}
	}

	void Engine::_update_objects() {
	}

	void Engine::_update_physics() {
		for (const SPObject &p_obj : m_objects)
		{
			p_obj->physics();
		}
	}

	EngineThreading::lock_type EngineThreading::ThreadInstance::acquire() {
		return lock_type(this->mutex);
	}

	bool EngineThreading::ThreadInstance::is_running() const {
		return this->mutex.is_locked();
	}
}
