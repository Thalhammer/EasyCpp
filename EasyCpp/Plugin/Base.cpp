#include "Base.h"

namespace EasyCpp
{
	namespace Plugin
	{
		Base::~Base()
		{
		}

		void Base::init(InitArgs& args)
		{
			for (auto e : args.getServerInterfaces())
				_server_interfaces[e->getName()][e->getVersion()] = e;

			init(args);
		}

		InterfacePtr Base::getServerInterface(const std::string & ifacename, uint64_t version)
		{
			if (!_server_interfaces.count(ifacename))
				throw std::runtime_error("Interface not found");
			if (!_server_interfaces.at(ifacename).count(version))
				throw std::runtime_error("Interface version not found");
			return _server_interfaces.at(ifacename).at(version);
		}

		bool Base::hasServerInterface(const std::string & ifacename, uint64_t version)
		{
			return _server_interfaces.count(ifacename) && _server_interfaces.at(ifacename).count(version);
		}
	}
}