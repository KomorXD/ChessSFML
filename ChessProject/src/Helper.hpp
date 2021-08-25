#pragma once

#include <string>
#include <vector>
#include <sstream>

class Helper {
	private:
		Helper() = default;

	public:
		static Helper& Get();
		
		static std::pair<int, int>		StringToPos(const std::string& strPos);
		static std::string				PosToString(const std::pair<int, int>& pos);
		static std::string				SecondsToStringTime(float secs);
		static std::vector<std::string> StringSplit(const std::string& line, char separator = ' ');
};