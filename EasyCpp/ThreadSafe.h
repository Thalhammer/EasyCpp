#pragma once
#include <mutex>
#include <functional>

namespace EasyCpp
{
	template<typename T, typename MutexType = std::mutex>
	class ThreadSafe
	{
	public:
		typedef T member_type;
		typedef MutexType mutex_type;
	private:
		mutable T _object;
		mutable MutexType _mutex;

		class ThreadSafeLock
		{
			std::unique_lock<MutexType> _lock;
			T& _data;
		public:
			ThreadSafeLock(MutexType& mtx, T& data)
				: _lock(mtx), _data(data)
			{}
			friend class ThreadSafe<T>;
			ThreadSafeLock(ThreadSafeLock&&) = default;
			~ThreadSafeLock() {}

			T* operator->() const { return &_data; }
			T& operator*() const { return _data; }
		};
	public:
		template<typename ...Args>
		ThreadSafe(Args... args)
			:_object(std::forward<Args>(args)...)
		{}
		ThreadSafe()
		{}
		ThreadSafe(ThreadSafe<T>&& obj)
			:_object(std::move(*(obj)))
		{}
		ThreadSafe(const ThreadSafe<T>& obj)
			:_object(*(obj)), _mutex()
		{}
		ThreadSafe(const T& obj)
			:_object(obj)
		{}

		ThreadSafeLock lock() const { return ThreadSafeLock(_mutex, _object); }
		ThreadSafeLock operator->() const { return lock(); }
		T& operator*() const { return *lock(); }
	};
}
