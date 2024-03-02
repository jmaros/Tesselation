#pragma once
//
// Tesselation.h
//
#include "ApplicationSpecificOptions.h"
#include "Shape.h"
#include "Date.h"


#include <set>

namespace Nessie {

    using std::set;
    using std::ostream;

    using TableLayout       = Geom::Shape<int>;
    using TableResult       = Geom::Shape<bool>;
    using ShapeCollection   = vector<TableResult>;
    using MonthMap          = map<MonthEnum, Position>;
    using DayMap            = map<int, Position>;
    using ShapeSet          = set<TableResult>;
    using ShapeCollections  = vector<ShapeCollection>;
    using TableResultChars  = Geom::Shape<char>;

        struct SolutionStep {
        explicit SolutionStep (const TableResult& tableResult)
         :  m_indexOfShapeSet   ()
         ,  m_shapeIndexInSet   ()
         ,  m_position          ()
         ,  m_tableResult       (tableResult)
        {
        }

        size_t          m_indexOfShapeSet{};
        size_t          m_shapeIndexInSet{};
        Position        m_position{};
        TableResult     m_tableResult;
    };

    using Solution  = vector<SolutionStep>;
    using Solutions = vector<Solution>;
    using Riddle    = Solution;

    class Tesselation {
    public:
    // constructors
        Tesselation (const TableLayout                  & tableLayout,
                     const ShapeCollection              & shapes,
                     const ApplicationSpecificOptions   & options);

    // accessors
        inline const Date   & GetDate               ()                              const;
        string              GetTableResult          (const Solution     & solution,
                                                     size_t             index = 0u) const;
        string              Result                  ()                              const;
        bool                SetTableShapeShowZeros  (bool               bShow)      const;
        const TableResult&  GetTableShape           ()                              const;
        size_t              RandomValue             (size_t             maxValue)   const;

     // modifiers
        bool                Solve                   (Riddle     & riddle);
    private:
    // data members
        const TableLayout           & m_tableLayout;
        const ShapeCollection       & m_shapes;
        ApplicationSpecificOptions  m_options;
        TableResult                 m_tableShape;
        MonthMap                    m_monthMap;
        DayMap                      m_dayMap;
        ShapeCollections            m_shapeCollections;
        TableResult                 m_tableResult;
        Solutions                   m_solutions;
    };

    ostream     & operator << (const ostream        & os,
                               const Tesselation    & tesselation);
} // namespace Nessie
