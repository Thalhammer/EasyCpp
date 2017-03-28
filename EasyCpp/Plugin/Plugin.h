#pragma once
#include <string>
#include "../DynLib.h"
#include "BaseInterface.h"
#include "Manager.h"


namespace EasyCpp
{
	namespace Plugin
	{
		class Manager::Plugin
		{
		public:
			Plugin(std::string name, std::string path, const interface_map_t& server_ifaces);
			~Plugin();
			void deinit();
			bool canUnload() const;
			std::string getName() const;
			std::string getPath() const;
			DynLib& getDynLib();
			InterfacePtr getInterface(const std::string& ifacename, uint64_t version) const;
			bool hasInterface(const std::string& ifacename, uint64_t version) const;
		private:
			std::string _name;
			std::string _path;
			DynLib _lib;
			interface_map_t _interfaces;
			std::shared_ptr<BaseInterface> _baseiface;
			std::shared_ptr<void> _unload_protect;
		};
	}
}