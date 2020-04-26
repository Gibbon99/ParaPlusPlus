/*
 *
 * Created by zmant on 17-10-2.
 * IniFileParser is a tiny ini file parser library.
 * Any Problem Please Connect zmant724@aliyun.com
 *
*/


#include "../../hdr/classes/IniFileParser.hpp"

#include <array>
#include <fstream>
#include <iostream>

IniFileParser::IniFileParser(const std::string& filePath)
{
    if(filePath.size()>1)
        Open(filePath);
}

bool IniFileParser::Open(const std::string &filePath)
{
    std::ifstream inifile(filePath, std::ifstream::in);
    if(!inifile.is_open())
    {
        std::cerr << " Open The Ini File: " << filePath << " Failed ! " << std::endl;
        return false;
    }

    ReadIniFile(inifile);
    ParserKeyValue();

    inifile.close();
    return true;
}


bool IniFileParser::GetValuesToString(const std::string &sections, const std::string &keys, std::string &values)
{
    auto sec = m_iniValues.find(sections+keys);
    if(sec == m_iniValues.end())
    {
        std::cerr << " The " << sections << " Or " << keys << " Is Not Exist. Please Confirm The Section Or Key Name!"<<std::endl;
        return false;
    }
    values = sec->second;
    return true;
}

bool IniFileParser::GetValueToFloat(const std::string &sections, const std::string &keys, float &value, const int index)
{
    GetValueToType<float>(sections, keys, value, index);
    return true;
}

bool IniFileParser::GetValueToInt(const std::string &sections, const std::string& keys, int& value, const int index)
{
    GetValueToType<int>(sections, keys, value, index);
    return true;
}

bool IniFileParser::GetValuesToIntVec(const std::string &sections, const std::string &keys, std::vector<int> &values)
{
    GetValuesToTypeVec<int>(sections, keys, values);
    return true;
}

bool IniFileParser::GetValuesToFloatVec(const std::string &sections, const std::string &keys, std::vector<float> &values)
{
    GetValuesToTypeVec<float>(sections, keys, values);
    return true;
}

template <typename T> bool IniFileParser::GetValueToType(const std::string &sections, const std::string &keys, T& value, const int index)
{
    std::string values;
    GetValuesToString(sections, keys, values);

    std::vector<T> value_vec;
    GetValuesFromString<T>(values, value_vec);

    if(index < value_vec.size())
    {
        value = value_vec[index];
        return true;
    }

    std::cerr << "The section: " << sections << " and the key: " << keys << " Out of range " << std::endl;
    return false;
}

template <typename T> bool IniFileParser::GetValuesToTypeVec(const std::string &sections, const std::string &keys, std::vector<T>& value_vec)
{
    std::string values;
    GetValuesToString(sections, keys, values);

    GetValuesFromString<T>(values, value_vec);
    return true;
}

template<typename T> bool IniFileParser::GetValuesFromString(const std::string& str, std::vector<T>& values)
{
    std::stringstream buf(str);
    T tmp = 0;
    while(buf.good())
    {
        buf >> tmp;
        values.push_back(tmp);
    }
    return true;
}

bool IniFileParser::ReadIniFile(std::ifstream& inifile)
{
    bool comment = false;
    char prech = inifile.get();
    char ch = prech;
    while(inifile.good())
    {
	    if ((prech == '\n' && ch == ' ') || (prech == '=' && ch == ' ') || (prech == ']' && ch != '\n'))
	    {
		    ch = inifile.get();
		    continue;
	    }
	    if (ch == '#' || ch == ';' || comment)
	    {
		    comment     = true;
		    if (ch == '\n')
			    comment = false;
		    ch          = inifile.get();
		    continue;
	    }
	    if ((prech == '\n' && ch == '\n'))
	    {
		    prech = ch;
		    ch    = inifile.get();
		    continue;
	    }

	    if (prech == ' ' && ch == '=')
	    {
		    long p = m_iniText.tellp();
		    m_iniText.seekp(p - 1);
	    }

	    m_iniText << ch;
	    prech = ch;
	    ch    = inifile.get();
    }
//
// Remove console output from the library
//
//    std::cout << " ====================== Ini Config Start ================ " <<std::endl;
//    std::cout << m_iniText.str();
//    std::cout << " ====================== Ini Config End ================ " <<std::endl;

	return true;
}

bool IniFileParser::ParserKeyValue()
{
    std::string iniStr = m_iniText.str();
    std::size_t start = iniStr.find("[");
    while(m_iniText.good())
    {
        std::size_t end = iniStr.find("]", start);
        if(end == std::string::npos)
            break;

        SectionKey2Values ck;
        std::string section = iniStr.substr(start+1, end-start-1);


        m_iniText.seekg(end+2);
        std::array<char,1024> line;
        while(true)
        {
            m_iniText.getline(&line[0], 1024, '\n');

            std::string keyValues(&line[0]);
            if (keyValues.find("[") != std::string::npos)
                break;

            std::size_t sep = keyValues.find("=");
            if(sep == std::string::npos)
                break;

            std::string key = keyValues.substr(0, sep);

            std::string values = keyValues.substr(sep + 1, keyValues.size() - sep - 1);

            m_iniValues.insert(SectionKey2Values::value_type(section+key, values));

            start = m_iniText.tellg();
        }
    }
    return true;
}

