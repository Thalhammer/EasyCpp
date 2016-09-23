#include "Mapper.h"
#include "../StringAlgorithm.h"

namespace EasyCpp
{
	namespace Database
	{
		Mapper::Mapper(DatabasePtr db, const std::string & dbname)
			: _db(db), _dbname(dbname)
		{
		}

		Mapper::~Mapper()
		{
		}

		void Mapper::setTypeTable(const std::type_info & type, const std::string & tablename)
		{
			_tables.insert({ type, tablename });
		}

		void Mapper::store(AnyValue val)
		{
			Bundle data;
			bool done = false;
			if (val.isSerializable()) {
				auto ser = val.serialize();
				if (ser.isType<Bundle>()) {
					data = ser.as<Bundle>();
					done = true;
				}
			}
			if (!done && val.isDynamicObject()) {
				auto& obj = val.asDynamicObject();
				for (auto& prop : obj.getProperties()) {
					data.set(prop, obj.getProperty(prop));
				}
				done = true;
			}
			if (!done) {
				throw std::runtime_error("Could not convert object");
			}
			if (!_tables.count(val.type()))
				throw std::runtime_error("Could not find table");
			this->store(_tables.at(val.type()), data);
		}

		void Mapper::store(const std::string& table, Bundle b)
		{
			std::string insert = "INSERT INTO `";
			if (_dbname != "") {
				insert += _dbname + "`.`";
			}
			insert += table;
			insert += "` (";
			std::vector<std::string> cols;
			std::vector<AnyValue> vals;
			for (auto& e : b) {
				cols.push_back("`" + e.first + "`");
				vals.push_back(e.second);
			}
			insert += implode<std::string>(",", cols);
			insert += ") VALUES (";
			insert += implode<std::string>(",", std::vector<std::string>(cols.size(), "?"));
			insert += ");";

			auto stmt = _db->prepare(insert);
			for (size_t i = 0; i < vals.size(); i++) stmt->bind(i, vals[i]);

			auto num = stmt->execute();
			if (num != 1)
				throw std::runtime_error("Failed to insert row");
		}

		void Mapper::remove(AnyValue val)
		{
			Bundle data;
			bool done = false;
			if (val.isSerializable()) {
				auto ser = val.serialize();
				if (ser.isType<Bundle>()) {
					data = ser.as<Bundle>();
					done = true;
				}
			}
			if (!done && val.isDynamicObject()) {
				auto& obj = val.asDynamicObject();
				for (auto& prop : obj.getProperties()) {
					data.set(prop, obj.getProperty(prop));
				}
				done = true;
			}
			if (!done) {
				throw std::runtime_error("Could not convert object");
			}
			if (!_tables.count(val.type()))
				throw std::runtime_error("Could not find table");
			this->remove(_tables.at(val.type()), data);
		}

		void Mapper::remove(const std::string & table, Bundle b)
		{
			std::string del = "DELETE FROM `";
			if (_dbname != "") {
				del += _dbname + "`.`";
			}
			del += table;
			del += "` WHERE ";
			std::vector<std::string> cols;
			std::vector<AnyValue> vals;
			for (auto& e : b) {
				cols.push_back("`" + e.first + "`=?");
				vals.push_back(e.second);
			}
			del += implode<std::string>(" AND ", cols) + ";";

			auto stmt = _db->prepare(del);
			for (size_t i = 0; i < vals.size(); i++) stmt->bind(i, vals[i]);

			auto num = stmt->execute();
			if (num != 1)
				throw std::runtime_error("Failed to remove row");
		}

		void Mapper::update(AnyValue val, const std::string & condition, const AnyArray & params)
		{
			Bundle data;
			bool done = false;
			if (val.isSerializable()) {
				auto ser = val.serialize();
				if (ser.isType<Bundle>()) {
					data = ser.as<Bundle>();
					done = true;
				}
			}
			if (!done && val.isDynamicObject()) {
				auto& obj = val.asDynamicObject();
				for (auto& prop : obj.getProperties()) {
					data.set(prop, obj.getProperty(prop));
				}
				done = true;
			}
			if (!done) {
				throw std::runtime_error("Could not convert object");
			}
			if (!_tables.count(val.type()))
				throw std::runtime_error("Could not find table");
			this->update(_tables.at(val.type()), data, condition, params);
		}

		void Mapper::update(const std::string & table, Bundle b, const std::string & condition, const AnyArray & params)
		{
			std::string update = "UPDATE `";
			if (_dbname != "") {
				update += _dbname + "`.`";
			}
			update += table;
			update += "` SET ";
			std::vector<std::string> cols;
			std::vector<AnyValue> vals;
			for (auto& e : b) {
				cols.push_back("`" + e.first + "`=?");
				vals.push_back(e.second);
			}
			update += implode<std::string>(", ", cols) + " " + condition;

			auto stmt = _db->prepare(update);
			for (size_t i = 0; i < vals.size(); i++) stmt->bind(i, vals[i]);
			for (size_t i = 0; i < params.size(); i++) stmt->bind(i + vals.size(), params.at(i));

			auto num = stmt->execute();
			if (num != 1)
				throw std::runtime_error("Failed to update row");
		}

		Bundle Mapper::read(const std::string & table, const std::string & condition, const AnyArray & params)
		{
			std::string select = "SELECT * FROM `";
			if (_dbname != "") {
				select += _dbname + "`.`";
			}
			select += table;
			select += "` " + condition + ";";

			auto stmt = _db->prepare(select);
			for (size_t i = 0; i < params.size(); i++) stmt->bind(i, params.at(i));

			return stmt->executeQueryRow();
		}
	}
}