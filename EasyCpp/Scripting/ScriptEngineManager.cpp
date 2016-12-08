#include "ScriptEngineManager.h"

namespace EasyCpp
{
	namespace Scripting
	{
		std::vector<ScriptEngineFactoryPtr> ScriptEngineManager::getAvailableEngines()
		{
			return getInstance()._getAvailableEngines();
		}

		ScriptEnginePtr ScriptEngineManager::getEngineByExtension(const std::string & extension)
		{
			return getInstance()._getEngineByExtension(extension);
		}

		ScriptEnginePtr ScriptEngineManager::getEngineByMimeType(const std::string & mime)
		{
			return getInstance()._getEngineByMimeType(mime);
		}

		ScriptEnginePtr ScriptEngineManager::getEngineByName(const std::string & sname)
		{
			return getInstance()._getEngineByName(sname);
		}

		void ScriptEngineManager::registerEngineFactory(ScriptEngineFactoryPtr factory)
		{
			getInstance()._registerEngineFactory(factory);
		}

		void ScriptEngineManager::deregisterEngineFactory(const std::string & short_name)
		{
			getInstance()._deregisterEngineFactory(short_name);
		}

		void ScriptEngineManager::deregisterEngineFactory(ScriptEngineFactoryPtr factory)
		{
			deregisterEngineFactory(factory->getNames().at(0));
		}

		ScriptEngineManager::ScriptEngineManager()
		{
		}

		ScriptEngineManager & ScriptEngineManager::getInstance()
		{
			static ScriptEngineManager manager;
			return manager;
		}

		std::vector<ScriptEngineFactoryPtr> ScriptEngineManager::_getAvailableEngines()
		{
			return *_factories;
		}

		ScriptEnginePtr ScriptEngineManager::_getEngineByExtension(const std::string & extension)
		{
			auto lock = _factories.lock();
			for (auto& e : *lock)
			{
				for (auto& ext : e->getExtensions()) {
					if (ext == extension) {
						return e->getScriptEngine();
					}
				}
			}
			return nullptr;
		}

		ScriptEnginePtr ScriptEngineManager::_getEngineByMimeType(const std::string & mime)
		{
			auto lock = _factories.lock();
			for (auto& e : *lock)
			{
				for (auto& type : e->getMimeTypes()) {
					if (type == mime) {
						return e->getScriptEngine();
					}
				}
			}
			return nullptr;
		}

		ScriptEnginePtr ScriptEngineManager::_getEngineByName(const std::string & sname)
		{
			auto lock = _factories.lock();
			for (auto& e: *lock)
			{
				for (auto& name : e->getNames()) {
					if (name == sname) {
						return e->getScriptEngine();
					}
				}
			}
			return nullptr;
		}

		void ScriptEngineManager::_registerEngineFactory(ScriptEngineFactoryPtr factory)
		{
			// TODO: Check double registration
			_factories->push_back(factory);
		}

		void ScriptEngineManager::_deregisterEngineFactory(const std::string & short_name)
		{
			auto lock = _factories.lock();
			for (size_t i=0; i< lock->size(); i++)
			{
				for (auto& sname : (*lock)[i]->getNames()) {
					if (sname == short_name) {
						lock->erase(lock->begin() + i);
						return;
					}
				}
			}
		}
	}
}