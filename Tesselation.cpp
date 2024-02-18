//
// Tesselation.cpp
//  : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Tesselation.h"

#include <sstream>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::stringstream;

namespace Nessie {
    using std::map;

    const map<MonthEnum, int> MonthDays {
        { MonthEnum::January,	31 },
        { MonthEnum::LeapFebr,	29 },
        { MonthEnum::February,	28 },
        { MonthEnum::March,		31 },
        { MonthEnum::April,		30 },
        { MonthEnum::May,		31 },
        { MonthEnum::June,		30 },
        { MonthEnum::July,		31 },
        { MonthEnum::August,	31 },
        { MonthEnum::September,	30 },
        { MonthEnum::October,	31 },
        { MonthEnum::November,	30 },
        { MonthEnum::December,	31 }
    };

    const map<MonthEnum, string> MonthNames {
        { MonthEnum::Unknown,	"Undisclosed"	},
        { MonthEnum::January,	"January"		},
        { MonthEnum::LeapFebr,	"February"		},
        { MonthEnum::February,	"February"		},
        { MonthEnum::March,		"March"			},
        { MonthEnum::April,		"April"			},
        { MonthEnum::May,		"May"			},
        { MonthEnum::June,		"June"			},
        { MonthEnum::July,		"July"			},
        { MonthEnum::August,	"August"		},
        { MonthEnum::September,	"September"		},
        { MonthEnum::October,	"October"		},
        { MonthEnum::November,	"November"		},
        { MonthEnum::December,	"December"		}
    };

    // standalone helper functions
    string NameOfMonth (MonthEnum	month)
    {
        string mnStr;
        auto mnmCit = MonthNames.find(month);
        if (mnmCit != MonthNames.end()) {
            mnStr = mnmCit->second;
        } else {
            mnStr = "***No Such month!***";
        }
        return mnStr;
    }

    string DateStr (MonthEnum	month,
                    int		day)
    {
        stringstream dateStream;
        dateStream << NameOfMonth(month) << " " << day;
        return dateStream.str();
    }

    bool IsValidDate (MonthEnum		month,
                      int		day)
    {
        bool isValid{};
        auto mldCit = MonthDays.find(month);
        if (mldCit != MonthDays.end()) {
            isValid = mldCit->second >= day;
        }
        return isValid;
    }

    Tesselation::Tesselation (const TableLayout			& table,
                              const ShapeCollection		& shapes,
                              MonthEnum					month,
                              int						day)
     : m_tableLayout	(table)
     , m_shapes			(shapes)
     , m_month			(month)
     , m_day			(day)
     , m_tableShape		(table.NumRows(),
                         table.NumCols())
    {
        for (size_t row = 0; row < m_tableLayout.NumRows(); ++row) {
            for (size_t col = 0; col < m_tableLayout.NumRows(); ++col) {
//				m_tableShape[row][col] = m_tableLayout[row][col];
            }
        }
    }

    MonthEnum Tesselation::Month ()	const
    {
        return m_month;
    }

    int Tesselation::Day ()	const
    {
        return m_day;
    }

        // accessors
    ostream& operator << (ostream& os,
                          const Tesselation& tessy)
    {
        os << "Tesselation Date = " << DateStr(tessy.Month(),
                                               tessy.Day());
        return os;
    }
} // namespace Nessie

using namespace Nessie;

int main ()
{
    // The zeros representing the excluded parts
    // The negative numbers are placeholders for the monthes
    // The positive numbers specify the position of the given day on the table
    const TableLayout tableLayout = {
        {  -1,  -2,  -3,  -4,  -5,  -7,   0 },
        {  -7,  -8,  -9, -10, -11, -12,   0 },
        {  +1,  +2,  +3,  +4,  +5,  +6,  +7 },
        {  +8,  +9, +10, +11, +12, +13, +14 },
        { +15, +16, +17, +18, +19, +20, +21 },
        { +22, +23, +24, +25, +26, +27, +28 },
        { +29, +30, +31,   0,   0,   0,   0 }
    };

    //const TableShape tableShape = {
    //	{ 0, 0, 0, 0, 0, 0, 1 },
    //	{ 0, 0, 0, 0, 0, 0, 1 },
    //	{ 0, 0, 0, 0, 0, 0, 0 },
    //	{ 0, 0, 0, 0, 0, 0, 0 },
    //	{ 0, 0, 0, 0, 0, 0, 0 },
    //	{ 0, 0, 0, 0, 0, 0, 0 },
    //	{ 0, 0, 0, 1, 1, 1, 1 }
    //};


    const ShapeCollection shapes = {

        {{ 1, 0, 1 },
         { 1, 1, 1 }},
    
        {{ 1, 1, 1 },
         { 1, 1, 1 }},
    
        {{ 1, 1, 1 },
         { 1, 1, 0 }},

        {{ 1, 0, 0 },
         { 1, 0, 0 },
         { 1, 1, 1 }},

        {{ 1, 1, 0 },
         { 0, 1, 0 },
         { 0, 1, 1 }},

        {{ 1, 1, 1, 1 },
         { 0, 1, 0, 0 }},

        {{ 1, 1, 1, 0 },
         { 0, 0, 1, 1 }},

        {{ 1, 1, 1, 1 },
         { 1, 0, 0, 0 }}
    };

    MonthEnum	month		= MonthEnum::February;
    int		day			= 17;
    bool	isLeapYear	= true;

    if (isLeapYear && month == MonthEnum::February) {
        month = MonthEnum::LeapFebr;
    }

    if (IsValidDate(month, day)) {
        cout << "The date is " << DateStr(month, day) << endl;
        Tesselation tesselation (tableLayout,
                                 shapes,
                                 month,
                                 day);
        //tesselation.Solve();
        cout << tesselation << "\n";
    } else {
        cout << "No such date as " << DateStr(month, day) << endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
