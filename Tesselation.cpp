//
// Tesselation.cpp
//  : This file contains the 'main' function. Program execution begins and ends there.
//

#if defined (_MSC_VER) && _MSC_VER >= 1900 && !defined (_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Tesselation.h"

namespace Nessie {

    // The zeros representing the excluded parts
    // The negative numbers are placeholders for the monthes
    // The positive numbers specify the position of the given day on the table
    const TableLayout MainTable = {
        {  -1,  -2,  -3,  -4,  -5,  -7,   0 },
        {  -7,  -8,  -9, -10, -11, -12,   0 },
        {  +1,  +2,  +3,  +4,  +5,  +6,  +7 },
        {  +8,  +9, +10, +11, +12, +13, +14 },
        { +15, +16, +17, +18, +19, +20, +21 },
        { +22, +23, +24, +25, +26, +27, +28 },
        { +29, +30, +31,   0,   0,   0,   0 }
    };

    const ShapeCollection Shapes = {

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
 
    const string ShapeChars { "@#$*+=x&" };

    // constructors
    Tesselation::Tesselation (const TableLayout         & table,
                              const ShapeCollection     & shapes,
                              const Date                & date)
     : m_tableLayout    (table)
     , m_shapes         (shapes)
     , m_date           (date)
     , m_tableShape     (table.NumRows(),
                         table.NumCols())
     , m_monthMap       ()
     , m_dayMap         ()
     , m_shapeSets      ()
     , m_tableResult    (table.NumRows(),
                         table.NumCols())
     , m_solution       ()
     , m_isSolved       ()
    {
        // fill m_shapesSet so that it will contain all the different shapes
        for (auto   & shape : m_shapes) {
            ShapeSet    shapesSet;
            bool succi = shapesSet.insert(shape).second;
            if (succi) {
                auto horiFlipped = shape.CreateHorizontallyFlipped();
                auto vertFlipped = shape.CreateVerticallyFlipped();
                auto bothFlipped = horiFlipped.CreateVerticallyFlipped();
                (void) shapesSet.insert(horiFlipped);
                (void) shapesSet.insert(vertFlipped);
                (void) shapesSet.insert(bothFlipped);
                (void) shapesSet.insert(shape.CreateTransposed());
                (void) shapesSet.insert(horiFlipped.CreateTransposed());
                (void) shapesSet.insert(vertFlipped.CreateTransposed());
                (void) shapesSet.insert (bothFlipped.CreateTransposed());
                m_shapeSets.push_back(shapesSet);
            }
        }

#if defined (VERBOSE)
        int shNum {};
        int shsNum {};
        for (auto   & shapeSet : m_shapeSets) {
            cout << ++shsNum << ". ShapeSet:\n";
            ShapeSet    shapesSet;
            int shssNum{};
            for (auto   & shape : shapeSet) {
                cout << ++shNum << ".(" << shsNum << '.' << ++shssNum << ")." << shape;
            }
        }
#endif

        // Fill up m_tableShape, and create the month and day maps to position:
        for (size_t row = 0; row < m_tableLayout.NumRows(); ++row) {
            for (size_t col = 0; col < m_tableLayout.NumCols(); ++col) {
                Position pos(row, col);
                auto newValue = m_tableLayout.Value(pos);
                m_tableShape.SetData(pos, newValue == 0);
                if (newValue < 0) {
                    m_monthMap[MonthEnum(-newValue)] = pos;
                } else
                if (newValue > 0) {
                    m_dayMap[newValue] = pos;
                }
            }
        }
        m_monthMap[MonthEnum::LeapFebr] = m_monthMap[MonthEnum::February];

        // Setting the additional element cells:
        m_tableShape.SetData(m_monthMap[m_date.Month()], true);
        m_tableShape.SetData(m_dayMap[m_date.Day()], true);

        // Setting the initial valu of the resulting table and the
        // required return value for the out of bound positions
        m_tableResult = m_tableShape;
        m_tableResult.SetOutOfBoundValue(true);
//#if defined (VERBOSE)
        bool bPrev{ m_tableShape.SetShowZeros(true) };
        cout << "Table" << m_tableShape;
        (void) m_tableShape.SetShowZeros(bPrev);
//#endif
    }

    // accessors
    inline const Date & Tesselation::GetDate ()   const
    {
        return m_date;
    }

    string    Tesselation::Result () const
    {
        stringstream srs;
#if defined (VERBOSE)
        for (auto& solStep : m_solution) {
            srs << (&solStep - &m_solution[0]) + 1;
            srs << ". step, positon = (";
            srs << solStep.m_position.GetRowIndex();
            srs << ", ";
            srs << solStep.m_position.GetColIndex();
            srs << ")\n";
            srs << "ShapeSetItem = ";
            srs << solStep.m_indexOfShapeSet;
            srs << ".";
            srs << solStep.m_shapeIndexInSet;
            srs << ")\n";
            srs << solStep.m_tableResult;
        }
#endif
        return srs.str();
    }

    // modifiers
    void Tesselation::Solve (Riddle     &riddle)
    {
        if (!m_isSolved) {
            if (riddle.size() == 0) {
                //prepare the first step...
                riddle.push_back(SolutionStep (m_tableResult));
                for (auto row = 0; row < m_tableResult.NumCols(); ++row) {
                     for (auto col = 0; col < m_tableResult.NumCols(); ++col) {
                         Position   pos(row, col);
                         if (m_tableResult.Value(pos) == false) {
                             riddle.back().m_FreePositions.emplace_back(pos);
                         }
                    }
                }
                for (auto frp : riddle.back().m_FreePositions) {
                    // TODO:
                    //If (riddle.back().m_tableResult.Matrix().Accomodates(frp, this->m_shapeSets...))
                }
                Solve (riddle);
            } else {
                if (riddle.size () < this->m_shapeSets.size ()) {
                    // TODO:
                    //for ()
                    riddle.push_back(SolutionStep (riddle.back().m_tableResult));
                    Solve(riddle);
                } else {
                    m_isSolved = true;
                    m_solution = riddle;
                }
            }
        }
    }

   // global operators
    ostream& operator << (ostream            & os,
                          const Tesselation  & tessy)
    {
        os << "Tesselation Date = " << tessy.GetDate().DateStr() << endl;
        os << tessy.Result();
        return os;
    }
} // namespace Nessie

using namespace Nessie;

int main ()
{
    int         year            = 2024;
    MonthEnum   month           = MonthEnum::February;
    int         day             = 29;
    Date        date (year,
                      month,
                      day);

    bool        useCurrentTime  = true;

    if (useCurrentTime) {
        date = Date::GetCurrentDate();
    }

    if (date.IsValidDate()) {
        Tesselation tesselation (MainTable,
                                 Shapes,
                                 date);
        Riddle riddle;
        tesselation.Solve (riddle);
        cout << tesselation << "\n";
    } else {
        cout << "No such date as " << date.DateStr() << endl;
    }
}
// Visual Studio (2022 Community Edition) Tips:
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
