#pragma once


namespace Dot {
	struct JsonAttribute
	{
		//Name : Man
		std::pair<std::string, std::string> m_Value;
	};
	struct JsonObject
	{
		std::vector<JsonAttribute> m_Attribute;
	};
	struct Json
	{
		std::unordered_map<std::string, std::vector<JsonObject>> m_Object;
	};

	class JsonParser
	{
	public:
		Json ParseJson(const std::string& filepath,const std::vector<std::string>& keywords);


	private:
		std::vector<std::string> DelimiterString(const std::string& str, const std::string& delimiter);
		void EraseFromString(std::string& str, const char character);
	};
}