#pragma once
#include <stdexcept>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <atomic>
#include <condition_variable>

namespace EasyCpp
{
	template<typename T>
	class Promise;

	template<>
	class Promise<void>;

	template<typename T>
	struct is_promise {
		static const bool value = false;
	};

	template<typename T>
	struct is_promise<Promise<T>> {
		static const bool value = true;
	};

	template<>
	class Promise<void>
	{
	public:
		typedef void value_type;

		Promise()
		{
			_shared = std::make_shared<Shared>();
		}

		void resolve()
		{
			_shared->resolve();
		}

		void reject(std::exception_ptr ex)
		{
			_shared->reject(ex);
		}

		template<typename Func>
		auto then(Func fn)
		{
			return _shared->then(fn);
		}

		void await()
		{
			_shared->await();
		}

		Promise<void> error(std::function<void(std::exception_ptr)> fn)
		{
			return _shared->error(fn);
		}

		void reset()
		{
			_shared->reset();
		}
	private:
		class Shared
		{
		public:
			Shared()
			{
				_state = State::PENDING;
			}

			void resolve()
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state != State::PENDING)
					throw std::runtime_error("Invalid state");
				for (auto& e : _then)
				{
					e();
				}
				_then.clear();
				_state = State::FULFILLED;
			}

			void reject(std::exception_ptr ex)
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state != State::PENDING)
					throw std::runtime_error("Invalid state");
				_exception = ex;
				for (auto& e : _error)
				{
					e(_exception);
				}
				_error.clear();
				_state = State::REJECTED;
			}

			template<typename Func, typename Result = typename std::result_of<Func()>::type>
			typename std::enable_if<!std::is_void<Result>::value && !is_promise<Result>::value, Promise<Result>>::type
				then(Func fn)
			{
				Promise<Result> pres;
				auto pfn = [fn, pres]() mutable {
					try {
						Result res = fn();
						pres.resolve(res);
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn();
				}
				return pres;
			}

			template<typename Func, typename Result = typename std::result_of<Func()>::type>
			typename std::enable_if<std::is_void<Result>::value && !is_promise<Result>::value, Promise<void>>::type
				then(Func fn)
			{
				Promise<void> pres;
				auto pfn = [fn, pres]() mutable {
					try {
						fn();
						pres.resolve();
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn();
				}
				return pres;
			}

			template<typename Func, typename Result = typename std::result_of<Func()>::type>
			typename std::enable_if<!std::is_void<Result>::value && is_promise<Result>::value, Promise<typename Result::value_type>>::type
				then(Func fn)
			{
				Promise<typename Result::value_type> pres;
				auto pfn = [fn, pres]() mutable {
					try {
						Result res = fn();
						res.then([pres](typename Result::value_type& val) mutable {
							pres.resolve(val);
						});
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn();
				}
				return pres;
			}

			void await()
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					// Wait
					std::condition_variable cv_wait;
					_then.push_back([&cv_wait]() {
						// Mutex is locked by resolv call
						cv_wait.notify_all();
					});
					_error.push_back([&cv_wait](std::exception_ptr) {
						// Mutex is locked by reject call
						cv_wait.notify_all();
					});
					cv_wait.wait(lck, [this]() { return _state != State::PENDING; });
				}
				if (_state == State::REJECTED) {
					std::rethrow_exception(_exception);
				}
			}

			Promise<void> error(std::function<void(std::exception_ptr)> fn)
			{
				Promise<void> pres;
				auto pfn = [fn, pres](std::exception_ptr val) mutable {
					try {
						fn(val);
						pres.resolve();
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_error.push_back(pfn);
				}
				else {
					pfn(_exception);
				}
				return pres;
			}

			void reset()
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					throw std::runtime_error("Invalid state");
				}
				_state = State::PENDING;
				_exception = nullptr;
				_then.clear();
				_error.clear();
			}

		private:
			enum class State
			{
				PENDING,
				FULFILLED,
				REJECTED
			};
			std::mutex _mtx;
			std::exception_ptr _exception;
			State _state;
			std::vector<std::function<void()>> _then;
			std::vector<std::function<void(std::exception_ptr)>> _error;
		};
		std::shared_ptr<Shared> _shared;
	public:
		static Promise<void> Race(std::vector<Promise<void>> promises)
		{
			Promise<void> res;
			for (auto& e : promises)
			{
				std::shared_ptr<std::atomic<bool>> done = std::make_shared<std::atomic<bool>>(false);
				std::function<void()> fn([res, done]() mutable {
					if (done->exchange(true))
						return;
					res.resolve();
				});
				e.then(fn);
				e.error(std::function<void(std::exception_ptr ex)>([res, done](auto ex) mutable {
					if (done->exchange(true))
						return;
					res.reject(ex);
				}));
			}
			return res;
		}

		static Promise<void> All(std::vector<Promise<void>> promises)
		{
			Promise<void> pres;
			size_t done = promises.size();
			auto fn = [pres, done]() mutable {
				done--;
				if (done == 0)
				{
					pres.resolve();
				}
			};
			// Required to prevent lambda copy to use persistent variables
			auto ptr_fn = std::make_shared<decltype(fn)>(fn);

			for (auto& e : promises)
			{
				e.then(std::function<void()>([ptr_fn]() mutable {
					(*ptr_fn)();
				}));
				e.error(std::function<void(std::exception_ptr ex)>([pres](auto ex) mutable {
					pres.reject(ex);
				}));
			}
			return pres;
		}
	};

	template<typename T>
	class Promise
	{
	public:
		typedef T value_type;

		Promise()
		{
			_shared = std::make_shared<Shared>();
		}

		void resolve(T result)
		{
			_shared->resolve(result);
		}

		void reject(std::exception_ptr ex)
		{
			_shared->reject(ex);
		}

		template<typename Func>
		auto then(Func f)
		{
			return _shared->then(f);
		}

		T await()
		{
			return _shared->await();
		}

		Promise<void> error(std::function<void(std::exception_ptr)> fn)
		{
			return _shared->error(fn);
		}

		void reset()
		{
			_shared->reset();
		}
	private:
		class Shared
		{
		public:
			Shared()
			{
				_state = State::PENDING;
			}

			void resolve(T val)
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state != State::PENDING)
					throw std::runtime_error("Invalid state");
				_value = std::unique_ptr<T>(new T(val));
				for (auto& e : _then)
				{
					e(val);
				}
				_then.clear();
				_state = State::FULFILLED;
			}

			void reject(std::exception_ptr ex)
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state != State::PENDING)
					throw std::runtime_error("Invalid state");
				_exception = ex;
				for (auto& e : _error)
				{
					e(_exception);
				}
				_error.clear();
				_state = State::REJECTED;
			}

			template<typename Func, typename Result = typename std::result_of<Func(T&)>::type>
			typename std::enable_if<!std::is_void<Result>::value && !is_promise<Result>::value, Promise<Result>>::type
				then(Func fn)
			{
				Promise<Result> pres;
				auto pfn = [fn, pres](T& val) mutable {
					try {
						Result res = fn(val);
						pres.resolve(res);
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn(*_value);
				}
				return pres;
			}

			template<typename Func, typename Result = typename std::result_of<Func(T&)>::type>
			typename std::enable_if<std::is_void<Result>::value && !is_promise<Result>::value, Promise<void>>::type
				then(Func fn)
			{
				Promise<void> pres;
				auto pfn = [fn, pres](T& val) mutable {
					try {
						fn(val);
						pres.resolve();
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn(*_value);
				}
				return pres;
			}

			template<typename Func, typename Result = typename std::result_of<Func(T&)>::type>
			typename std::enable_if<!std::is_void<Result>::value && is_promise<Result>::value, Promise<typename Result::value_type>>::type
				then(Func fn)
			{
				Promise<typename Result::value_type> pres;
				auto pfn = [fn, pres](T& val) mutable {
					try {
						Result res = fn(val);
						res.then([pres](typename Result::value_type& val) mutable {
							pres.resolve(val);
						});
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_then.push_back(pfn);
				}
				else {
					pfn(*_value);
				}
				return pres;
			}

			T await()
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					// Wait
					std::condition_variable cv_wait;
					_then.push_back([&cv_wait](T& res) {
						// Mutex is locked by resolv call
						cv_wait.notify_all();
					});
					_error.push_back([&cv_wait](std::exception_ptr ex) {
						// Mutex is locked by reject call
						cv_wait.notify_all();
					});
					cv_wait.wait(lck, [this]() { return _state != State::PENDING; });
				}
				if (_state == State::FULFILLED) {
					return *_value;
				}
				else {
					std::rethrow_exception(_exception);
				}
			}

			Promise<void> error(std::function<void(std::exception_ptr)> fn)
			{
				Promise<void> pres;
				auto pfn = [fn, pres](std::exception_ptr val) mutable {
					try {
						fn(val);
						pres.resolve();
					}
					catch (...) {
						pres.reject(std::current_exception());
					}
				};
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					_error.push_back(pfn);
				}
				else {
					pfn(_exception);
				}
				return pres;
			}

			void reset()
			{
				std::unique_lock<std::mutex> lck(_mtx);
				if (_state == State::PENDING) {
					throw std::runtime_error("Invalid state");
				}
				_state = State::PENDING;
				_exception = nullptr;
				_value = nullptr;
				_then.clear();
				_error.clear();
			}

		private:
			enum class State
			{
				PENDING,
				FULFILLED,
				REJECTED
			};
			std::mutex _mtx;
			std::exception_ptr _exception;
			std::unique_ptr<T> _value;
			State _state;
			std::vector<std::function<void(T&)>> _then;
			std::vector<std::function<void(std::exception_ptr)>> _error;
		};
		std::shared_ptr<Shared> _shared;
	public:
		static Promise<T> Race(std::vector<Promise<T>> promises)
		{
			Promise<T> res;
			for (auto& e : promises)
			{
				std::shared_ptr<std::atomic<bool>> done = std::make_shared<std::atomic<bool>>(false);
				std::function<void(T&)> fn([res, done](T& val) mutable {
					if (done->exchange(true))
						return;
					res.resolve(val);
				});
				e.then(fn);
				e.error(std::function<void(std::exception_ptr ex)>([res, done](auto ex) mutable {
					if (done->exchange(true))
						return;
					res.reject(ex);
				}));
			}
			return res;
		}

		static Promise<std::vector<T>> All(std::vector<Promise<T>> promises)
		{
			Promise<std::vector<T>> pres;
			std::vector<T> res;
			size_t done = 0;
			res.resize(promises.size());
			auto fn = [res, pres, done](size_t id, T& val) mutable {
				res.at(id) = val;
				done++;
				if (done == res.size())
				{
					pres.resolve(res);
				}
			};
			// Required to prevent lambda copy to use persistent variables
			auto ptr_fn = std::make_shared<decltype(fn)>(fn);

			for (size_t id = 0; id < promises.size(); id++)
			{
				auto& e = promises.at(id);
				e.then(std::function<void(T&)>([ptr_fn, id](T& val) mutable {
					(*ptr_fn)(id, val);
				}));
				e.error(std::function<void(std::exception_ptr ex)>([pres](auto ex) mutable {
					pres.reject(ex);
				}));
			}
			return pres;
		}
	};
}