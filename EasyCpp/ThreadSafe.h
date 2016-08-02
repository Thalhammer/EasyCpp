#pragma once
#include <mutex>
#include <functional>

namespace EasyCpp
{
	template<typename T, typename MutexType = std::mutex>
	class ThreadSafe
	{
	private:
		T _object;
		MutexType _mutex;

		class ThreadSafeLock
		{
			std::unique_lock<MutexType> _lock;
			T& _data;

			ThreadSafeLock(MutexType& mtx, T& data)
				: _lock(mtx), _data(data)
			{}
			friend class ThreadSafe<T>;
		public:
			ThreadSafeLock(ThreadSafeLock&&) = default;
			~ThreadSafeLock() {}

			T* operator->() { return &_data; }
			T& operator*() { return _data; }
		};

	public:
		template<typename ...Args>
		ThreadSafe(Args... args)
			:_object(std::forward<Args>(args)...)
		{

		}

		ThreadSafeLock lock() { return ThreadSafeLock(_mutex, _object); }
		ThreadSafeLock operator->() { return lock(); }
		T& operator*() { return *lock(); }
	};
}