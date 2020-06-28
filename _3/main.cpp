/*
ID: mgramezani
LANG: C++
TASK: friday
PROG: friday
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace friday
{
    constexpr auto source = "https://www.includehelp.com/cpp-programs/get-week-day-from-given-date.aspx";

    using vec_int = std::vector<size_t>;

    namespace validator
    {
        bool is_repeat_time(const size_t in)
        {
            return (in <= 400);
        }
    }

    namespace date
    {
        enum month
        {
            January = 1, February,
            March, April,
            May, June,
            July, August,
            September, October,
            November, December
        };

        enum week
        {
            Saturday = 0, Sunday,
            Monday, Tuesday,
            Wednesday, Thursday,
            Friday
        };

        constexpr size_t week_length = 7;

        namespace helper
        {
            inline week day(int16_t y, uint16_t m, uint16_t d)
            {
                auto day = d + ((153 * (m + 12 * ((14 - m) / 12) - 3) + 2) / 5)
                            + (365 * (y + 4800 - ((14 - m) / 12)))
                            + ((y + 4800 - ((14 - m) / 12)) / 4)
                            - ((y + 4800 - ((14 - m) / 12)) / 100)
                            + ((y + 4800 - ((14 - m) / 12)) / 400)
                            - 32045;
                return week((day + 1) % 7);
            }
        }
    }

    class day_calculate
    {
    public:
        day_calculate()
            : m_year(1900),
              m_month(date::month::June),
              m_day(date::week::Monday)
        {
        }

        vec_int operator()() const
        {
            vec_int result(date::week_length /* week length */, 0);
            const auto length = m_year + m_number_of_repeat;

            constexpr size_t month_length = 12;
            for (size_t year = m_year; year < length; ++year)
            {
                for (size_t month = 1; month <= month_length; ++month)
                {
                    result.at(date::helper::day(year, month, 13))++;
                }
            }

            return result;
        }

    private:
        /*
         * I pick variable type from:
         *   https://github.com/soroush/libcalendars#usage
         */
        int16_t m_year;
        uint8_t m_month;
        uint16_t m_day;

        size_t m_number_of_repeat;

    private:
        friend std::ifstream& operator>>(std::ifstream& in, day_calculate& out);
        friend std::ofstream& operator<<(std::ofstream& out, const day_calculate& in);
    };

    std::ifstream& operator>>(std::ifstream& in, day_calculate& out)
    {
        in >> out.m_number_of_repeat;

        if (not validator::is_repeat_time(out.m_number_of_repeat))
        {
            throw std::out_of_range("N <= 400");
        }

        return in;
    }

    std::ofstream& operator<<(std::ofstream& out, const day_calculate& in)
    {
        const auto result = in();

        out << result.at(date::week::Friday) << " ";
        out << result.at(date::week::Saturday) << " ";
        out << result.at(date::week::Sunday) << " ";
        out << result.at(date::week::Monday) << " ";
        out << result.at(date::week::Tuesday) << " ";
        out << result.at(date::week::Wednesday) << " ";
        out << result.at(date::week::Thursday);
        out << std::endl;

        return out;
    }

}

namespace usaco
{
    constexpr auto in_file = "friday.in";
    constexpr auto out_file = "friday.out";
}

int main()
{
    std::ifstream in_file(usaco::in_file);
    std::ofstream out_file(usaco::out_file);
    friday::day_calculate d;

    in_file >> d;
    out_file << d;
}
