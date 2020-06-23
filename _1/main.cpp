/*
ID: mgramezani
LANG: C++
PROG: ride
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>

namespace ufo
{
    const static std::map<char, unsigned short> char_map
    {
        {'A', 1}, {'B', 2},
        {'C', 3}, {'D', 4},
        {'E', 5}, {'F', 6},
        {'G', 7}, {'H', 8},
        {'I', 9}, {'J', 10},
        {'K', 11}, {'L', 12},
        {'M', 13}, {'N', 14},
        {'O', 15}, {'P', 16},
        {'Q', 17}, {'R', 18},
        {'S', 19}, {'T', 20},
        {'U', 21}, {'V', 22},
        {'W', 23}, {'X', 24},
        {'Y', 25}, {'Z', 26}
    };

    namespace validator
    {
        bool is_length_okay(const std::string& str)
        {
            return str.length() > 0 and str.length() <= 6;
        }
    }

    namespace correction
    {
        void capitilize_string(std::string& str)
        {
            std::transform(str.begin(), str.end(), str.begin(),
                        [](unsigned char i)
            {
                return std::toupper(i);
            });
        }

        void remove_char(std::string& str)
        {
            str.erase(std::remove_if(str.begin(), str.end(), [](auto i)
            {
                return !std::isalpha(i);
            }), str.end());
        }
    }

    namespace licensing
    {
        constexpr unsigned key = 47;

        unsigned code(const std::string& str)
        {
            unsigned result = 1;
            std::for_each(str.begin(), str.end(), [&result](char i)
            {
                result *= char_map.at(i);
            });

            return result;
        }

        namespace validator
        {
            bool its_right(unsigned code_1, unsigned code_2)
            {
                return (code_1 % key) == (code_2 % key);
            }
        }
    }

    class ufo_group
    {
    public:
        ufo_group() = default;

        bool operator()() const
        {
            return licensing::validator::its_right(
                                    licensing::code(m_group_name),
                                    licensing::code(m_comet));
        }

    private:
        std::string m_group_name;
        std::string m_comet;


    private:
        bool validate() const
        {
            return validator::is_length_okay(m_group_name) and
                        validator::is_length_okay(m_comet);
        }
        void do_correction()
        {
            correction::remove_char(m_group_name);
            correction::remove_char(m_comet);
        }

        friend std::ifstream& operator>>(std::ifstream&, ufo_group&);
        friend std::ofstream& operator<<(std::ofstream&, const ufo_group&);
    };

    std::ifstream& operator>>(std::ifstream& in, ufo_group& out)
    {
        in >> out.m_comet;
        in >> out.m_group_name;

        out.do_correction();
        if (not out.validate())
        {
            throw std::runtime_error("Wrong string length.");
        }

        return in;
    }
    std::ofstream& operator<<(std::ofstream& out, const ufo_group& in)
    {
        out << (in() ? "GO" : "STAY") << std::endl;

        return out;
    }

}

namespace usaco
{
    constexpr auto input_file = "ride.in";
    constexpr auto output_file = "ride.out";
}

int main()
{
    ufo::ufo_group group;
    std::ifstream in_file(usaco::input_file);
    std::ofstream out_file(usaco::output_file);

    if (not in_file.is_open() or not in_file.is_open())
    {
        throw std::runtime_error("Couldn't open the file.");
    }

    try
    {
        in_file >> group;
        out_file << group;
    }
    catch (const std::exception& e)
    {
        std::clog << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
