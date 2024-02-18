#pragma once
//
// Tesselation.h
//
#include "Shape.h"

#include <map>

namespace Nessie {

    enum class MonthEnum : int {
        Unknown		=  0,
        January		=  1, // 31 days
        LeapFebr	= -2, // 29 days in leap years
        February	=  2, // 28 days in a common year
        March,		// =  3, 31 days
        April,		// =  4, 30 days
        May,		// =  5, 31 days
        June,		// =  6, 30 days
        July,		// =  7, 31 days
        August,		// =  8, 31 days
        September,	// =  9, 30 days
        October,	// = 10, 31 days
        November,	// = 11, 30 days
        December,	// = 12, 31 days
    };

    using TableLayout		= geom::Shape<int>;
    using TableShape		= geom::Shape<bool>;
    using ShapeCollection	= vector<geom::Shape<bool>>;

    class Tesselation {
    public:
    // constructors
        Tesselation (const TableLayout		& tableLayout,
                     const ShapeCollection	& shapes,
                     enum MonthEnum			month,
                     int					day);

    // accessors
        MonthEnum	Month ()	const;
        int			Day ()		const;

    private:
    // data members
        const TableLayout					& m_tableLayout;
        const ShapeCollection				& m_shapes;
        MonthEnum							m_month;
        int									m_day;
        TableShape							m_tableShape;
    };

    std::ostream& operator << (std::ostream			& os,
                               const Tesselation	& tesselation);
} // namespace Nessie
