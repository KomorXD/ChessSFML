#include "Helper.hpp"

Helper& Helper::Get() {
	static Helper h;

	return h;
}

std::pair<int, int> Helper::StringToPos(const std::string& strPos) {
	return { strPos[0] - 96, strPos[1] - 48 };
}

std::string Helper::PosToString(const std::pair<int, int>& pos) {
	std::string res = "";

	res += (char)(pos.first + 96);
	res += (char)(pos.second + 48);

	return res;
}

std::string Helper::SecondsToStringTime(float secs) {
	int minutes = (int)secs / 60;
	int seconds = (int)secs % 60;

	std::string out = "";

	if (minutes == 0)
		out += "00:";
	else if (minutes < 10) {
		out += "0";
		out += (char)(minutes + 48);
		out += ":";
	}
	else
		out += std::to_string(minutes) + ':';

	if (seconds == 0)
		out += "00";
	else if (seconds < 10) {
		out += "0";
		out += (char)(seconds + 48);
	}
	else
		out += std::to_string(seconds);

	return out;
}

std::vector<std::string> Helper::StringSplit(const std::string& line, char separator) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(line);

	while (std::getline(tokenStream, token, separator))
		tokens.push_back(token);

	return tokens;
}