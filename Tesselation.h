#pragma once
//
// Tesselation.h
//
#include "Shape.h"
#include "Date.h"

namespace Nessie {

    using TableLayout       = Geom::Shape<int>;
    using TableShape        = Geom::Shape<bool>;
    using ShapeCollection   = vector<Geom::Shape<bool>>;
    using MonthMap          = map<MonthEnum, Position>;
    using DayMap            = map<int, Position>;

    class Tesselation {
    public:
    // constructors
        Tesselation (const TableLayout      & tableLayout,
                     const ShapeCollection  & shapes,
                     const Date             & date);

    // accessors
        inline const Date   & GetDate () const;

    private:
    // data members
        const TableLayout       & m_tableLayout;
        const ShapeCollection   & m_shapes;
        Date                    m_date;
        TableShape              m_tableShape;
        MonthMap                m_monthMap;
        DayMap                  m_dayMap;
    };

    std::ostream& operator << (std::ostream         & os,
                               const Tesselation    & tesselation);
} // namespace Nessie
