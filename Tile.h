#pragma once
//
// Tile.h
//
#include "TileOptions.h"
#include "Shape.h"
#include "Date.h"


#include <set>
#include <random>

namespace Nessie {

    using std::ostream;
    using std::random_device;
    using std::set;
    using std::shuffle;
    using std::uniform_int_distribution;

    using TableLayout       = Geom::Shape<int8_t, int16_t>; // int16_t - the second type -
                                                            // determines the print format
                                                            // (to print as int instead of char)
    using TableResult       = Geom::Shape<bool>;
    using ShapeCollection   = vector<TableResult>;
    using MonthMap          = map<MonthEnum, Position<size_t>>;
    using DayMap            = map<int, Position<size_t>>;
    using DayOfWeekMap      = map<DayOfWeekEnum, Position<size_t>>;
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

        size_t              m_indexOfShapeSet{};
        size_t              m_shapeIndexInSet{};
        Position<size_t>    m_position{};
        TableResult         m_tableResult;
    };

    using Solution      = vector<SolutionStep>;
    using Solutions     = vector<Solution>;

    class Riddle {
    public:
    // constructors
        explicit Riddle (const TableResult      & tableResult,
                         const size_t           numberOfShapes)
        {
            SolutionStep solutionStep(tableResult);
            for (size_t idx = 0; idx < numberOfShapes; ++idx) {
                m_solution.push_back(solutionStep);
            }
        }
 
        // accessors
        size_t GetLevel () const
        {
            return m_level;
        }

        const Solution& GetSolution () const
        {
            return m_solution;
        }

    // modifiers
        void SetNextLevel ()
        {
            size_t actLevel = m_level;
            if (actLevel + 1 < m_solution.size()) {
                m_level = actLevel + 1;
                m_solution[m_level].m_indexOfShapeSet   = m_solution[actLevel].m_indexOfShapeSet + 1;
                m_solution[m_level].m_tableResult       = m_solution[actLevel].m_tableResult;
                m_solution[m_level].m_shapeIndexInSet   = 0;
            } else {
                cout << "Internal error at SetNextLevel!\n";
            }
        }

        void RestorePreviousLevel (const size_t         actLevel,
                                   const TableResult    & savedTableResult)
        {
            m_level = actLevel;
            m_solution[m_level].m_tableResult = savedTableResult;
        }

        SolutionStep    * GetSolutionStepPtr ()
        {
            SolutionStep    * currentStep;
            if (m_level < m_solution.size()) {
                currentStep = &m_solution[m_level];
            } else {
                currentStep = nullptr;
            }
            return currentStep;
        }

    private:
        Solution    m_solution;
        size_t      m_level{};
    };

    class Tile {
    public:
    // constructors
        Tile (const TableLayout         & tableLayout,
              const ShapeCollection     & shapes,
              const TileOptions         & options);

    // accessors
        inline const Date           & GetDate               ()                                  const;
        inline const TileOptions    & GetOptions            ()                                  const;
        string                      GetTableResultStr       (const Solution     & solution,
                                                             size_t             index = 0u)     const;
        string                      Result                  ()                                  const;
        bool                        SetTableShapeShowZeros  (bool               bShow)          const;
        const TableResult           & GetTableShape         ()                                  const;
        const TableResult           & GetTableResult        ()                                  const;
        size_t                      RandomValue             (size_t             maxValue)       const;

    // static template members
        template<typename Container>
        static void                 RandomShuffle           (Container          & container);

     // modifiers
        bool                        Solve                   (Riddle     & riddle);
    private:
    // data members
        const TableLayout   & m_tableLayout;
        ShapeCollection     m_shapes;
        TileOptions         m_options;
        TableResult         m_tableShape;
        MonthMap            m_monthMap;
        DayMap              m_dayMap;
        DayOfWeekMap        m_dayOfWeekMap;
        ShapeCollections    m_shapeCollections;
        TableResult         m_tableResult;
        Solutions           m_solutions;
    };

    template<typename Container>
    void   Tile::RandomShuffle (Container   & container)
    {
        static std::mt19937  mt(random_device{}());
        shuffle(container.begin(), container.end(), mt);
    }

    ostream     & operator << (const ostream    & os,
                               const Tile       & tile);
} // namespace Nessie
