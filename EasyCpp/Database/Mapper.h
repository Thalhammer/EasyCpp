#pragma once
#include "Database.h"
#include "../AnyArray.h"
#include "../DynamicObject.h"
#include <typeindex>

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT Mapper
		{
		public:
			Mapper(DatabasePtr db, const std::string& dbname = "");
			~Mapper();

			void setTypeTable(const std::type_info& type, const std::string& tablename);

			void store(AnyValue val);
			void store(const std::string& table, Bundle b);
			void remove(AnyValue val);
			void remove(const std::string& table, Bundle b);
			void update(AnyValue val, const std::string& condition, const AnyArray& params);
			void update(const std::string& table, Bundle b, const std::string& condition, const AnyArray& params);
			Bundle read(const std::string& table, const std::string& condition, const AnyArray& params);
			template<typename T>
			T read(const std::string& condition, const AnyArray& params)
			{
				T res;
				Bundle data = this->read(_tables.at(typeid(T)),condition, params);
				if (trySerializable<T>(res, data)) {
					return res;
				}
				if (tryDynamicObject<T>(res, data)) {
					return res;
				}
				throw std::runtime_error("Type not serializable");
			}
		private:
			template<typename T>
			typename std::enable_if<std::is_base_of<Serialize::Serializable, T>::value, bool>::type
				trySerializable(T& t, Bundle data)
			{
				t.fromAnyValue(data);
				return true;
			}
			template<typename T>
			typename std::enable_if<!std::is_base_of<Serialize::Serializable, T>::value, bool>::type
				trySerializable(T& t, Bundle data)
			{
				return false;
			}

			template<typename T>
			typename std::enable_if<std::is_base_of<DynamicObject, T>::value, bool>::type
				tryDynamicObject(T& t, Bundle data)
			{
				for (auto& e : t.getProperties()) {
					t.setProperty(e, data.get(e));
				}
				return true;
			}
			template<typename T>
			typename std::enable_if<!std::is_base_of<DynamicObject, T>::value, bool>::type
				tryDynamicObject(T& t, Bundle data)
			{
				return false;
			}

			DatabasePtr _db;
			std::string _dbname;
			std::map<std::type_index, std::string> _tables;
		};
	}
}