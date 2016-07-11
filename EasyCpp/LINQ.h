#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>
#include <algorithm>

namespace EasyCpp
{
	template<typename T>
	class LazyIterator;
	template<typename T, typename Iterator>
	class LazySTLIterator;
	template<typename T>
	class LazyWhereIterator;
	template<typename In, typename Out>
	class LazySelectIterator;
	template<typename TKey, typename T>
	class LazyGroupIterator;
	template<typename TKey, typename T>
	class LazyOrderIterator;

	template<typename TKey, typename T>
	class LazyGroup
	{
	public:
		void setKey(TKey k) { _key = k; }
		TKey getKey() { return _key; }
		void setIterator(std::shared_ptr<LazyIterator<T>> it) { _iterator = it; }
		std::shared_ptr<LazyIterator<T>> getIterator() { return _iterator; }
	private:
		TKey _key;
		std::shared_ptr<LazyIterator<T>> _iterator;
	};

	template<typename T>
	class LazyIterator : public std::enable_shared_from_this<LazyIterator<T>>
	{
	public:
		typedef T element_type;

		virtual void next() = 0;
		virtual bool ended() = 0;
		virtual T element() = 0;

		std::shared_ptr<LazyIterator<T>> Where(std::function<bool(const T&)> fn)
		{
			return std::make_shared<LazyWhereIterator<T>>(this->shared_from_this(), fn);
		}

		template<typename Out>
		std::shared_ptr<LazyIterator<Out>> Select(std::function<Out(const T&)> fn)
		{
			return std::make_shared<LazySelectIterator<Out, T>>(this->shared_from_this(), fn);
		}

		template<typename TKey>
		std::shared_ptr<LazyIterator<LazyGroup<TKey, T>>> GroupBy(std::function<TKey(const T&)> fn)
		{
			return std::make_shared<LazyGroupIterator<TKey, T>>(this->shared_from_this(), fn);
		}

		template<typename TKey>
		std::shared_ptr<LazyIterator<T>> OrderBy(std::function<TKey(const T&)> fn)
		{
			return std::make_shared<LazyOrderIterator<TKey, T>>(this->shared_from_this(), fn);
		}

		std::vector<T> ToVector()
		{
			std::vector<T> res;
			while (!ended())
			{
				res.push_back(element());
				next();
			}
			return res;
		}

		T Single()
		{
			T res = this->element();
			this->next();
			if (!this->ended())
				throw std::runtime_error("Resultset contains multiple entries");
			return res;
		}

		T First()
		{
			if (ended())
				throw std::runtime_error("Empty resultset");
			return element();
		}

		size_t Count()
		{
			size_t res = 0;
			while (!ended())
			{
				res++;
				next();
			}
			return res;
		}

		T Sum()
		{
			T res = T();
			while (!ended())
			{
				res += element();
				next();
			}
			return res;
		}

		T Average()
		{
			T sum = T();
			size_t count = 0;
			while (!ended())
			{
				sum += element();
				count++;
				next();
			}
			if (count == 0)
				throw std::runtime_error("Empty resultset");
			return sum / count;
		}

		T Maximum()
		{
			T max = T();
			bool first = true;
			while (!ended())
			{
				auto el = element();
				if (el > max || first)
					max = el;
				first = false;
				next();
			}
			if (first)
				throw std::runtime_error("Empty resultset");
			return max;
		}

		T Minimum()
		{
			T min = T();
			bool first = true;
			while (!ended())
			{
				auto el = element();
				if (el < min || first)
					min = el;
				first = false;
				next();
			}
			if (first)
				throw std::runtime_error("Empty resultset");
			return min;
		}
	};

	template<typename T>
	class LazyWhereIterator : public LazyIterator<T>
	{
	public:
		LazyWhereIterator(std::shared_ptr<LazyIterator<T>> it, std::function<bool(const T&)> fn)
			: _source(it), _where(fn)
		{
			while (!_source->ended())
			{
				if (_where(_source->element()))
					break;
				_source->next();
			}
		}
		virtual bool ended() {
			return _source->ended();
		}
		virtual void next() {
			if (ended())
				return;
			do {
				_source->next();
			} while (!_source->ended() && !_where(_source->element()));
		}
		virtual T element() {
			if (_source->ended())
				throw std::range_error("Iterator ended");
			return _source->element();
		}
	private:
		std::shared_ptr<LazyIterator<T>> _source;
		std::function<bool(const T&)> _where;
	};

	template<typename T>
	class VectorIterator : public LazySTLIterator<T, typename std::vector<T>::const_iterator >
	{
	public:
		VectorIterator(std::unique_ptr<std::vector<T>> vect)
			: LazySTLIterator<T, typename std::vector<T>::const_iterator>(vect->begin(), vect->end()), _vect(std::move(vect))
		{}
	private:
		std::unique_ptr<std::vector<T>> _vect;
	};

	template<typename TKey, typename T>
	class LazyGroupIterator : public LazyIterator<LazyGroup<TKey, T>>
	{
	public:
		LazyGroupIterator(std::shared_ptr<LazyIterator<T>> it, std::function<TKey(const T&)> fn)
			: _source(it), _group_select(fn)
		{
			while (!_source->ended())
			{
				TKey key = _group_select(_source->element());
				_result[key].push_back(_source->element());
				_source->next();
			}
			_result_iterator = _result.begin();
		}

		virtual bool ended() {
			return _result_iterator == _result.end();
		}

		virtual void next() {
			if (ended())
				return;
			_result_iterator++;
		}

		virtual LazyGroup<TKey, T> element() {
			if (ended())
				throw std::range_error("Iterator ended");
			LazyGroup<TKey, T> g;
			g.setKey(_result_iterator->first);
			g.setIterator(std::make_shared<VectorIterator<T>>(std::make_unique<std::vector<T>>(_result_iterator->second)));
			return g;
		}
	private:
		std::shared_ptr<LazyIterator<T>> _source;
		std::function<TKey(const T&)> _group_select;
		std::unordered_map<TKey, std::vector<T>> _result;
		typename std::unordered_map<TKey, std::vector<T>>::iterator _result_iterator;
	};

	template<typename TKey, typename T>
	class LazyOrderIterator : public LazyIterator<T>
	{
	public:
		LazyOrderIterator(std::shared_ptr<LazyIterator<T>> it, std::function<TKey(const T&)> fn)
		{
			_result = it->ToVector();
			std::sort(_result.begin(), _result.end(), [&fn](auto first, auto second) {
				return std::less<TKey>()(fn(first), fn(second));
			});
			_result_iterator = _result.begin();
		}

		virtual bool ended() {
			return _result_iterator == _result.cend();
		}

		virtual void next() {
			if (ended())
				return;
			_result_iterator++;
		}

		virtual T element() {
			if (ended())
				throw std::range_error("Iterator ended");
			return *_result_iterator;
		}
	private:
		std::vector<T> _result;
		typename std::vector<T>::iterator _result_iterator;
	};

	template<typename T, typename Iterator>
	class LazySTLIterator : public LazyIterator<T>
	{
	public:
		LazySTLIterator(Iterator start, Iterator end)
			:_start(start), _end(end)
		{
			_iterator = _start;
		}

		virtual void next()
		{
			if (ended())
				return;
			_iterator++;
		}

		virtual bool ended()
		{
			return _iterator == _end;
		}

		virtual T element()
		{
			return *_iterator;
		}
	private:
		Iterator _iterator;
		Iterator _start;
		Iterator _end;
	};

	template<typename Out, typename In>
	class LazySelectIterator : public LazyIterator<Out>
	{
	public:
		LazySelectIterator(std::shared_ptr<LazyIterator<In>> source, std::function<Out(const In&)> fn)
			:_source(source), _select(fn)
		{

		}

		virtual bool ended()
		{
			return _source->ended();
		}

		virtual void next()
		{
			_source->next();
		}

		virtual Out element()
		{
			return _select(_source->element());
		}
	private:
		std::shared_ptr<LazyIterator<In>> _source;
		std::function<Out(const In&)> _select;
	};

	template<typename T>
	std::shared_ptr<LazyIterator<T>> LINQ(const std::vector<T>& elem)
	{
		return std::make_shared<LazySTLIterator<T, typename std::vector<T>::const_iterator>>(begin(elem), end(elem));
	}

	template<typename T, typename Iterator>
	std::shared_ptr<LazyIterator<T>> LINQ(Iterator begin, Iterator end)
	{
		return std::make_shared<LazySTLIterator<T, Iterator>>(begin, end);
	}
}
