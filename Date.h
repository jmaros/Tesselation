#pragma once
//
// Date.h
//

#include <chrono>   // chrono::system_clock
#include <ctime>    // localtime
#include <map>      // map
#include <sstream>  // stringstream
#include <iomanip>  // put_time
#include <iostream> // cout
#include <regex>    // regexp search
#include <string>   // string
#include <type_traits>

#include <cmath>    // abs

namespace Nessie {

    using namespace std::chrono;

    using std::cout;
    using std::endl;
    using std::map;
    using std::regex;
    using std::regex_search;
    using std::regex_constants::egrep;
    using std::right;
    using std::setw;
    using std::setfill;
    using std::smatch;
    using std::string;
    using std::stringstream;
    using std::underlying_type;

    enum class MonthEnum : int {
        Unknown        =  0,
        January        =  1, // 31 days
        LeapFebr       = -2, // 29 days in leap years
        February       =  2, // 28 days in a common year
        March,      // =  3,    31 days
        April,      // =  4,    30 days
        May,        // =  5,    31 days
        June,       // =  6,    30 days
        July,       // =  7,    31 days
        August,     // =  8,    31 days
        September,  // =  9,    30 days
        October,    // = 10,    31 days
        November,   // = 11,    30 days
        December,   // = 12,    31 days
    };

    const map<MonthEnum, int> MonthDays {
        { MonthEnum::January,   31 },
        { MonthEnum::LeapFebr,  29 },
        { MonthEnum::February,  28 },
        { MonthEnum::March,     31 },
        { MonthEnum::April,     30 },
        { MonthEnum::May,       31 },
        { MonthEnum::June,      30 },
        { MonthEnum::July,      31 },
        { MonthEnum::August,    31 },
        { MonthEnum::September, 30 },
        { MonthEnum::October,   31 },
        { MonthEnum::November,  30 },
        { MonthEnum::December,  31 }
    };

    const map<MonthEnum, string> MonthNames {
        { MonthEnum::Unknown,    "Undisclosed" },
        { MonthEnum::January,    "January"     },
        { MonthEnum::LeapFebr,   "February"    },
        { MonthEnum::February,   "February"    },
        { MonthEnum::March,      "March"       },
        { MonthEnum::April,      "April"       },
        { MonthEnum::May,        "May"         },
        { MonthEnum::June,       "June"        },
        { MonthEnum::July,       "July"        },
        { MonthEnum::August,    "August"       },
        { MonthEnum::September, "September"    },
        { MonthEnum::October,   "October"      },
        { MonthEnum::November,  "November"     },
        { MonthEnum::December,  "December"     }
    };

    enum class DayOfWeekEnum : int {
        Unknown         = 0,
        Sunday,     //  = 1,
        Monday,     //  = 2,
        Tuesday,    //  = 3,
        Wednesday,  //  = 4,
        Thursday,   //  = 5,
        Friday,     //  = 6,
        Saturday    //  = 7
    };

    const map<DayOfWeekEnum, string> DayNames {
        { DayOfWeekEnum::Unknown,     "Admittedly Unknown"    },
        { DayOfWeekEnum::Sunday,      "Sunday"                },
        { DayOfWeekEnum::Monday,      "Monday"                },
        { DayOfWeekEnum::Tuesday,     "Tuesday"               },
        { DayOfWeekEnum::Wednesday,   "Wednesday"             },
        { DayOfWeekEnum::Thursday,    "Thursday"              },
        { DayOfWeekEnum::Friday,      "Friday"                },
        { DayOfWeekEnum::Saturday,    "Saturday"              }
    };

class Date {
public:
    // constructors
    Date (int                    year,
          MonthEnum              month,
          int                    day)
     : m_year       (year)
     , m_month      (month)
     , m_day        (day)
     , m_dayOfWeek  (DayOfWeekEnum::Unknown)
    {
        AdjustLeapMonthAndWeekday();
        cout << DateStr() << endl;
    }

    // accessors
    int             GetYear         () const
    {
        return m_year;
    }

    MonthEnum       GetMonth        () const
    {
        return m_month;
    }

    int             GetDay          () const
    {
        return m_day;
    }

    DayOfWeekEnum   GetDayOfWeek    () const
    {
        uint32_t md = m_day;
        uint32_t mm = (underlying_type<MonthEnum>::type(m_month) + 9) % 12;
        uint32_t yy = m_year - (mm / 10);
        uint64_t dn = 365 * yy + (yy / 4) - (yy / 100) + (yy / 400) + ((306 * mm) + 5) / 10 + md + 2;
        int dow = (dn % 7) + 1;
        return DayOfWeekEnum(dow);
    }

    bool        IsLeapYear  () const
    {
        return m_year % 4 == 0 && (m_year % 100 != 0 || m_year % 400 == 0);
    }

    string      NameOfMonth () const
    {
        string mnStr;
        auto mnmCit = MonthNames.find(m_month);
        if (mnmCit != MonthNames.end()) {
            mnStr = mnmCit->second;
        } else {
            mnStr = "***No Such month!***";
        }
        return mnStr;
    }

    string      NameOfDay () const
    {
        string wdnStr;
        auto wdnmCit = DayNames.find(m_dayOfWeek);
        if (wdnmCit != DayNames.end()) {
            wdnStr = wdnmCit->second;
        } else {
            wdnStr = "***No Such day!***";
        }
        return wdnStr;
    }

    string      DateStr     () const
    {
        stringstream dateStream;
        dateStream << m_year << ' ' << NameOfMonth() << ' ' << m_day << ' ' << NameOfDay();
        return dateStream.str();
    }

    bool IsValidDate () const
    {
        bool isValid{};
        auto mldCit = MonthDays.find(m_month);
        if (mldCit != MonthDays.end()) {
            isValid = mldCit->second >= m_day;
        }
        return isValid;
    }

    // modifiers
    void AdjustLeapMonthAndWeekday ()
    {
        if (IsLeapYear()) {
            if (m_month == MonthEnum::February) {
                m_month = MonthEnum::LeapFebr;
            }
        } else {
            if (m_month == MonthEnum::LeapFebr) {
                m_month = MonthEnum::February;
            }
        }
        m_dayOfWeek = GetDayOfWeek();
    }

    void SetYear (const string   & ys)
    {
        stringstream ios(ys);
        ios >> m_year;
        AdjustLeapMonthAndWeekday();
    }

    void SetMonth (const string   & ms)
    {
        stringstream ios(ms);
        int moi;
        ios >> moi;
        m_month = MonthEnum(moi);
        AdjustLeapMonthAndWeekday();
    }

    void SetDay (const string   & ds)
    {
        stringstream ios(ds);
        ios >> m_day;
        m_dayOfWeek = GetDayOfWeek();
    }


    // static methods
    static string CurrentTimeAndDate ()
    {
        auto now        = system_clock::now();
        auto in_time_t  = system_clock::to_time_t(now);

        stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }

    static Date GetCurrentDate ()
    {
        int         year            = 2024;
        MonthEnum   month           = MonthEnum::February;
        int         day             = 17;
        //
        // Date-time format:
        // 2024-02-19 13:49:16
        //
        string stregex = "^[ \\t]*([0-9]{4})-([0-9]{2})-([0-9]{2})"
                         "[ \\t]+([0-9]{2}):([0-9]{2}):([0-9]{2})$";
        regex dateRegex(stregex);
        smatch dateMatch;

        string dateStr{ Date::CurrentTimeAndDate() };
        cout << dateStr << endl;

        if (regex_search(dateStr,
                         dateMatch,
                         dateRegex) &&
            dateMatch.size() == 7) {
            int dateparts[7] = {};
            for (size_t i = 1; i < dateMatch.size(); ++i) {
                stringstream sios;
                sios.str(dateMatch[i]);
                sios >> dateparts[i];
            }
            year = dateparts[1];
            month = MonthEnum (dateparts[2]);
            day = dateparts[3];

#if defined(VERBOSE)
            bool isHmsRequired = true;
            if (isHmsRequired) {
                int hour = dateparts[4];
                int min = dateparts[5];
                int sec = dateparts[6];
                stringstream sos;
                sos << right << setfill('0')
                    << setw(4) << year << '-'
                    << setw(2) << abs(static_cast<int>(month)) << '-'
                    << setw(2) << day  << ' '
                    << setw(2) << hour << ':'
                    << setw(2) << min  << ':'
                    << setw(2) << sec  << endl;
                cout << sos.str();
            }
#endif
        } else {
            cout << "The regexp search didn't yeald match!" << endl;
            cout << "Pattern:\n";
            cout << stregex << endl;
            cout << "String:\n";
            cout << dateStr << endl;
        }
        Date date (year, month, day);
        return date;
    }

private:
    int                     m_year;
    MonthEnum               m_month;
    int                     m_day;
    DayOfWeekEnum           m_dayOfWeek;
};

} // namespace Nessie
