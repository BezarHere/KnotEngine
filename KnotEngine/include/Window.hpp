#include "_Base.hpp"
#include "math/Rect.hpp"
#include <vector>

namespace kt
{
	enum class WindowMode : uint8_t
	{
		Windowed,
		Borderless,
		FullScreen,
	};

	enum class WindowEventType
	{
		Closed,

		Moved,
		Resized,

		Focused,
		Unfocused,

		Maximized,
		Minimized,
		Restored
	};

	struct WindowAttributes
	{
		const string_char *title = "Dolce";
		Vec2i size = {640, 480};
		WindowMode mode = WindowMode::Windowed;
	};


	union WindowEventData
	{
		struct ResizedMoved
		{
			Vec2i value;
		} resized, moved;
	};

	struct WindowEvent
	{
		inline WindowEvent(WindowEventType _type, const WindowEventData &_data)
			: type{_type}, data{_data} {
		}

		inline WindowEvent(WindowEventType _type)
			: type{_type}, data{} {
		}

		WindowEventType type;
		WindowEventData data;
	};

	class Window
	{
	public:
		struct CallbackServer;
		typedef void *WindowHandle;

		enum class WindowState
		{
			Closed,
			Maximized,
			Minimized,
			Restored
		};

		Window(const WindowAttributes &attributes);
		inline Window() : Window(WindowAttributes()) {}

		inline WindowState get_state() const { return m_state; }
		inline bool closed() const { return m_state == WindowState::Closed; }

		void poll();

		inline WindowEvent pop_event() {
			WindowEvent event = m_events.back();
			m_events.pop_back();
			return event;
		}

		inline const WindowEvent &peek_event() const {
			return m_events.back();
		}

		inline bool has_events() const {
			return !m_events.empty();
		}


	private:
		WindowHandle m_handle;
		WindowState m_state;

		RectI m_rect;

		std::vector<WindowEvent> m_events;
	};

}
