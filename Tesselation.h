#pragma once
//
// Tesselation.h
//
#include "Shape.h"
#include "Date.h"

#include <set>

namespace Nessie {

    using std::set;

    using TableLayout       = Geom::Shape<int>;
    using TableShape        = Geom::Shape<bool>;
    using ShapeCollection   = vector<Geom::Shape<bool>>;
    using MonthMap          = map<MonthEnum, Position>;
    using DayMap            = map<int, Position>;
    using ShapeSet          = set<Geom::Shape<bool>>;
    using ShapeSets         = vector<ShapeSet>;

        struct SolutionStep {
        explicit SolutionStep (const TableShape  & tableResult)
         :  m_indexOfShapeSet   ()
         ,  m_shapeIndexInSet   ()
         ,  m_position          ()
         ,  m_callDepth         ()
         ,  m_tableResult       (tableResult)
        {
        }

        size_t          m_indexOfShapeSet{};
        size_t          m_shapeIndexInSet{};
        Position        m_position{};
        size_t          m_callDepth{};
        TableShape      m_tableResult;
    };

    using Solution  = vector<SolutionStep>;
    using Riddle    = Solution;

    class Tesselation {
    public:
    // constructors
        Tesselation (const TableLayout      & tableLayout,
                     const ShapeCollection  & shapes,
                     const Date             & date);

    // accessors
        inline const Date   & GetDate   () const;
        string              Result      () const;
    // modifiers
        void                Solve       (Riddle     & riddle);
    private:
    // data members
        const TableLayout       & m_tableLayout;
        const ShapeCollection   & m_shapes;
        Date                    m_date;
        TableShape              m_tableShape;
        MonthMap                m_monthMap;
        DayMap                  m_dayMap;
        ShapeSets               m_shapeSets;
        TableShape              m_tableResult;
        Solution                m_solution;
        bool                    m_isSolved;
    };

    std::ostream& operator << (std::ostream         & os,
                               const Tesselation    & tesselation);
} // namespace Nessie
