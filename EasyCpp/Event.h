#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <mutex>

namespace EasyCpp
{
	template<typename ...Args>
	class SharedEvent;

	template<typename ...Args>
	class EventConnection
	{
	public:
		EventConnection(std::weak_ptr<SharedEvent<Args...>> ev)
		{
			_event = ev;
		}

		~EventConnection()
		{
			try {
				auto ptr = _event.lock();
				if (ptr)
					ptr->deregisterConnection(this);
			}
			catch (std::exception&) {}
		}

		void setFunction(std::function<void(Args...)> fn) { _function = fn; }
		std::function<void(Args...)> getFunction() const { return _function; }
	private:
		std::function<void(Args...)> _function;
		std::weak_ptr<SharedEvent<Args...>> _event;
	};

	template<typename ...Args>
	class SharedEvent : public std::enable_shared_from_this<SharedEvent<Args...>>
	{
	public:
		typedef std::shared_ptr<EventConnection<Args...>> ConnectionType;

		void operator()(Args... args) const
		{
			std::lock_guard<std::recursive_mutex> lck(_thread_lock);
			for (auto& con : _connections)
			{
				try {
					auto ptr = con.lock();
					if (ptr)
					{
						std::function<void(Args...)> fn = ptr->getFunction();
						if (fn)
							fn(args...);
					}
				}
				catch (std::exception&) {
				}
			}
		}

		ConnectionType registerConnection()
		{
			std::lock_guard<std::recursive_mutex> lck(_thread_lock);
			auto con = std::make_shared<EventConnection<Args...>>(this->shared_from_this());
			_connections.push_back(con);
			return con;
		}

		void deregisterConnection(EventConnection<Args...>* ev)
		{
			std::lock_guard<std::recursive_mutex> lck(_thread_lock);
			for (size_t i = 0; i < _connections.size(); i++)
			{
				try {
					auto ptr = _connections[i].lock();
					if (!ptr || ptr.get() == ev)
					{
						_connections.erase(_connections.begin() + i);
					}
				}
				catch (std::exception&)
				{
					_connections.erase(_connections.begin() + i);
					i--;
				}
			}
		}

		size_t getNumConnections() const
		{
			std::lock_guard<std::recursive_mutex> lck(_thread_lock);
			return _connections.size();
		}

	private:
		mutable std::recursive_mutex _thread_lock;
		std::vector<std::weak_ptr<EventConnection<Args...>>> _connections;
	};

	template<typename ...Args>
	class Event
	{
	public:
		typedef typename SharedEvent<Args...>::ConnectionType ConnectionType;
		Event()
		{
			_event = std::make_shared<SharedEvent<Args...>>();
		}

		void operator()(Args... params) const
		{
			(*_event)(params...);
		}

		ConnectionType registerConnection()
		{
			return _event->registerConnection();
		}

		size_t getNumConnections() const
		{
			return _event->getNumConnections();
		}
	private:
		std::shared_ptr<SharedEvent<Args...>> _event;
	};
}
