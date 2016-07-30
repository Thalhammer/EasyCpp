#pragma once

#include <Database/DatabaseException.h>

#include <my_global.h>
#include <mysql.h>

namespace EasyCppMySql
{
	class MySQLHandle
	{
	public:
		class HandleAccessor
		{
		public:
			void setInTransaction(bool i) { _hdl->_in_transaction = i; }
			bool getInTransaction() { return _hdl->_in_transaction; }
			MYSQL* getHandle() { return _hdl->_hdl; }
		private:
			HandleAccessor(MySQLHandle* hdl) { _hdl = hdl; }
			~HandleAccessor() {}

			HandleAccessor(HandleAccessor const&) = delete;
			void operator=(HandleAccessor const&) = delete;

			MySQLHandle* _hdl;
			friend class MySQLHandle;
		};

		MySQLHandle() {
			_hdl = mysql_init(nullptr);
			if (!_hdl) {
				throw EasyCpp::Database::DatabaseException("Failed to init mysql handle", {});
			}
		}

		~MySQLHandle() {
			mysql_close(_hdl);
		}

		template<typename T>
		T executeThreadSafe(std::function<T(HandleAccessor& hdl)> fn)
		{
			std::lock_guard<std::recursive_mutex> lck(_hdl_mutex);
			initializer.dummy();
			HandleAccessor accessor(this);
			return fn(accessor);
		}

		void executeThreadSafe(std::function<void(HandleAccessor& hdl)> fn)
		{
			std::lock_guard<std::recursive_mutex> lck(_hdl_mutex);
			initializer.dummy();
			HandleAccessor accessor(this);
			fn(accessor);
		}
	private:
		std::recursive_mutex _hdl_mutex;
		MYSQL* _hdl;
		bool _in_transaction;

		class ThreadInitializer {
		public:
			ThreadInitializer() {
				mysql_thread_init();
			};
			~ThreadInitializer() {
				mysql_thread_end();
			};

			// Call this just to make sure object gets initialized.
			void dummy() {}
		};
		static thread_local ThreadInitializer initializer;
	};
}