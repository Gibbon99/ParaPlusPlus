/*
 *
 * Created by zmant on 17-10-2.
 * IniFileParser is a tiny ini file parser library.
 * Any Problem Please Connect zmant724@aliyun.com
 *
*/

#ifndef _INITIAL_FILE_HPP_
#define _INITIAL_FILE_HPP_

#include <map>
#include <vector>
#include <string>
#include <sstream>

class IniFileParser
{
    typedef std::map<std::string, std::string>  SectionKey2Values;

public:
    explicit IniFileParser(const std::string& filePath="");

    bool Open(const std::string& filePath);
    bool GetValuesToString(const std::string& sections, const std::string& keys, std::string& values);
    bool GetValueToInt(const std::string& sections, const std::string& keys, int& value, const int index=0);
    bool GetValueToFloat(const std::string& sections, const std::string& keys, float& value, const int index=0);
    bool GetValuesToIntVec(const std::string& sections, const std::string& keys, std::vector<int>& values);
    bool GetValuesToFloatVec(const std::string& sections, const std::string& keys, std::vector<float>& values);

private:
    bool ReadIniFile(std::ifstream& file);
    bool ParserKeyValue();
    template <typename T> bool GetValueToType(const std::string& sections, const std::string& keys, T& value, const int index);
    template <typename T> bool GetValuesToTypeVec(const std::string& sections, const std::string& keys, std::vector<T>& values);
    template <typename T> bool GetValuesFromString(const std::string& str, std::vector<T>& values);

private:
    std::stringstream           m_iniText;
    SectionKey2Values           m_iniValues;
};



#endif //_INITIAL_FILE_HPP_
