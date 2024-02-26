#pragma once
//
// Tesselation.h
//
#include "Shape.h"
#include "Date.h"
#include "Options.h"

#include <set>

namespace Nessie {

    using std::set;

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
    using Riddle    = Solution;

    class Tesselation {
    public:
    // constructors
        Tesselation (const TableLayout      & tableLayout,
                     const ShapeCollection  & shapes,
                     const Options          & options);

    // accessors
        inline const Date   & GetDate   ()          const;
        string              Result      ()          const;
        bool SetTableShapeShowZeros (bool  bShow)   const;
        const TableResult& GetTableShape ()         const;
     // modifiers
        void                Solve       (Riddle     & riddle);
    private:
    // data members
        const TableLayout       & m_tableLayout;
        const ShapeCollection   & m_shapes;
        Options                 m_options;
        TableResult             m_tableShape;
        MonthMap                m_monthMap;
        DayMap                  m_dayMap;
        ShapeCollections        m_shapeCollections;
        TableResult             m_tableResult;
        Solution                m_solution;
        bool                    m_isSolved;
    };

    std::ostream& operator << (std::ostream         & os,
                               const Tesselation    & tesselation);
} // namespace Nessie
