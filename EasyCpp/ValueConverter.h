#pragma once
#include "DllExport.h"
#include <map>
#include <functional>
#include <memory>
#include <mutex>

namespace EasyCpp
{
	class DLL_EXPORT ValueConverter
	{
	public:
		typedef std::function<void(void*, void*)> converter_function_t;
	private:
		ValueConverter() {}
		~ValueConverter() {}
	public:
		template <typename from, typename to>
		static to convert(from from_val)
		{
			to res;
			convert(typeid(from), &from_val, typeid(to), &res);
			return res;
		}

		static void convert(const std::type_info& from_type, void* from_val, const std::type_info& to_type, void* to_val);

		template <typename from, typename to>
		static bool isConvertable()
		{
			return isConvertable(typeid(from), typeid(to));
		}

		static bool isConvertable(const std::type_info& from, const std::type_info& to);

		template <typename from, typename to>
		static void setConverter(converter_function_t fn)
		{
			setConverter(typeid(from), typeid(to), fn);
		}

		static void setConverter(const std::type_info& from, const std::type_info& to, converter_function_t fn);
	private:
		static void Init();

		static std::recursive_mutex _mtx;
		static std::map<size_t, std::map<size_t, converter_function_t>> _converters;
		static bool _init_done;
	};

#define CONVERT_FN(x,y,fn) EasyCpp::ValueConverter::setConverter<x, y>([](void* f, void* t) { \
							x& from = *((x*)f);\
							y& to = *((y*)t);\
							fn \
						});

}
