/*
ID: mgramezani
LANG: C++
TASK: gift1
PROG: gift1
*/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace gift
{
    using map_group = std::vector<std::pair<std::string, int>>;

    namespace validator
    {
        inline bool np(const int number)
        {
            return (number >= 2  and number <= 10) ?
                        true : false;
        }

        inline bool ng(const int number, const int length)
        {
            return (number >= 0 and number <= length) ?
                        true : false;
        }

        inline bool money(const int m)
        {
            return (m >= 0 and m <= 2000);
        }
    }

    namespace helper
    {
        inline void add_money(const std::string& name,
                    map_group& members,
                    const int money)
        {
            auto f = std::find_if(members.begin(), members.end(), [&name](auto & i)
            {
                return (i.first == name);
            });
            if (f != members.end())
            {
                f->second += money;
            }
        }
    }

    class group
    {
    public:
        group() = default;

    private:
        map_group m_member;

        friend std::ifstream& operator>>(std::ifstream& in, group& out);
        friend std::ofstream& operator<<(std::ofstream& out, const group& in);
    };

    std::ifstream& operator>>(std::ifstream& in, group& out)
    {
        /* Read number of NP and members name */
        {
            size_t np = 0;
            in >> np;
            if (not validator::np(np))
            {
                throw std::runtime_error("number <= 2  and number >= 10");
            }

            std::string tmp;
            for (size_t i = 0; i < np; ++i)
            {
                in >> tmp;
                out.m_member.push_back({std::move(tmp), 0});
            }
        }

        /* Read amount of money and number of giftier */
        for (size_t i = 0; i < out.m_member.size(); ++i)
        {
            size_t money = 0;
            size_t giftier_length = 0;

            std::string gifter;

            in >> gifter;
            in >> money >> giftier_length;

            if (not validator::money(money))
            {
                continue;
            }

            if (not validator::ng(giftier_length, out.m_member.size()))
            {
                helper::add_money(gifter, out.m_member, money);
                continue;
            }

            const int slice = (money != 0) ? (money / giftier_length) : 0;
            helper::add_money(gifter, out.m_member,
                        -money + ((money != 0) ? (money % giftier_length) : 0));

            std::string giftier;
            for (size_t j = 0; j < giftier_length; ++j)
            {
                in >> giftier;
                helper::add_money(giftier, out.m_member, slice);
            }
        }

        return in;
    }

    std::ofstream& operator<<(std::ofstream& out, const group& in)
    {
        for (const auto [key, value] : in.m_member)
        {
            out << key << " " << value << std::endl;
        }

        return out;
    }

}

namespace usaco
{
    constexpr auto input_file = "gift1.in";
    constexpr auto output_file = "gift1.out";
}

int main()
{
    std::ifstream in_file(usaco::input_file);
    std::ofstream out_file(usaco::output_file);
    gift::group g;

    in_file >> g;
    out_file << g;
}
