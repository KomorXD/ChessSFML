#pragma once

#include <unordered_map>
#include <stdexcept>

template<typename ResType>
class ResourceMananger {
	private:
		std::unordered_map<std::string, ResType> m_Resources;
		std::string m_Folder;
		std::string m_Ext;

		std::string GetFullPath(const std::string& name) {
			return m_Folder + name + m_Ext;
		}

	public:
		ResourceMananger(const std::string& folder, const std::string& ext)
			: m_Folder("Resources/" + folder + "/"), m_Ext("." + ext) { }

		bool AddResource(const std::string& name) {
			ResType res;

			if (res.loadFromFile(GetFullPath(name))) {
				m_Resources.insert(std::make_pair(name, res));

				return true;
			}

			return false;
		}

		bool EraseResource(const std::string& name) {
			return m_Resources.erase(name);
		}

		ResType& Get(const std::string& name) {
			try {
				return m_Resources.at(name);
			}
			catch (...) {
				return m_Resources.at("missing");
			}
		}
};