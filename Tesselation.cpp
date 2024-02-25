//
// Tesselation.cpp
//  : This file contains the 'main' function. Program execution begins and ends there.
//
// You can compile this code by using Visual Studio (2022 Community Edition)
//
// or you can compile on Windows using MinGW and a specific c++ version:
// test Tesselation c++23
//
// or you can compile on Linux using a specific c++ version:
// ./test.sh Tesselation c++23
//
/* You might have to convert crlf to lf in test.sh using sed like this:
sed -i 's/\r
/
/g' test.sh
*/
// or you can use
// dos2unix test.sh
// for the same.
//

#if defined (_MSC_VER) && _MSC_VER >= 1900 && !defined (_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Tesselation.h"
#include "ElapsedTime.h"

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
 
    //const string    ShapeChars{ "ABCDEFGH" };
    const string    ShapeChars{ "@#$*+ox&" };

    // constructors
    Tesselation::Tesselation (const TableLayout         & table,
                              const ShapeCollection     & shapes,
                              const Date                & date)
     : m_tableLayout        (table)
     , m_shapes             (shapes)
     , m_date               (date)
     , m_tableShape         (table.NumRows(),
                             table.NumCols())
     , m_monthMap           ()
     , m_dayMap             ()
     , m_shapeCollections   ()
     , m_tableResult        (table.NumRows(),
                             table.NumCols())
     , m_solution           ()
     , m_isSolved           ()
    {
        // fill m_shapesSet so that it will contain all the different shapes
        for (auto   & shape : m_shapes) {
            ShapeSet    shapeSet;
            bool succi = shapeSet.insert(shape).second;
            if (succi) {
                auto horiFlipped = shape.CreateHorizontallyFlipped();
                auto vertFlipped = shape.CreateVerticallyFlipped();
                auto bothFlipped = horiFlipped.CreateVerticallyFlipped();
                (void) shapeSet.insert(horiFlipped);
                (void) shapeSet.insert(vertFlipped);
                (void) shapeSet.insert(bothFlipped);
                (void) shapeSet.insert(shape.CreateTransposed());
                (void) shapeSet.insert(horiFlipped.CreateTransposed());
                (void) shapeSet.insert(vertFlipped.CreateTransposed());
                (void) shapeSet.insert (bothFlipped.CreateTransposed());
                ShapeCollection shapeCollection;
                for (auto& uniqueShape : shapeSet) {
                    shapeCollection.push_back(uniqueShape);
                }
                m_shapeCollections.push_back(shapeCollection);
            }
        }

#if defined (VERBOSE)
        cout << "\nMain Table:" << table;
        int shNum {};
        int shsNum {};
        for (auto   & shapeCollection : m_shapeCollections) {
            cout << ++shsNum << ". ShapeSet:\n";
            int shssNum{};
            for (auto   & shape : shapeCollection) {
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
    }

    // accessors
    inline const Date & Tesselation::GetDate ()   const
    {
        return m_date;
    }

    string    Tesselation::Result () const
    {
        TableResultChars    result(m_tableResult.NumRows(),
                                   m_tableResult.NumCols());
        stringstream srs;

        for (auto& solStep : m_solution) {
            auto& shapes = m_shapeCollections[solStep.m_indexOfShapeSet];
            const TableResult &theShape{ shapes[solStep.m_shapeIndexInSet] };
            TableResultChars chrep(theShape.NumRows(),
                                   theShape.NumCols());

            for (size_t r = 0; r < chrep.NumRows(); ++r) {
                for (size_t c = 0; c < chrep.NumCols(); ++c) {
                    Position chPos(r, c);
                    if (theShape.Value(chPos)) {
                        chrep.SetData(chPos, ShapeChars[solStep.m_indexOfShapeSet]);
                    }
                }
            }

            result.Accomodate(solStep.m_position, chrep);
        }
        result.SetShapeName("Solution");
        srs << result;
        return srs.str();
    }

    bool Tesselation::SetTableShapeShowZeros (bool  bShow) const
    {
        return m_tableShape.SetMutableShowZeros(bShow);
    }

    const TableResult& Tesselation::GetTableShape () const
    {
        return m_tableShape;
    }


    // modifiers
    void Tesselation::Solve (Riddle     &riddle)
    {
        if (!m_isSolved) {
            if (riddle.size() == 0) {
                // prepare the first step
                riddle.push_back(SolutionStep(m_tableResult));
            }
            for (auto row = 0u; row < m_tableResult.NumRows(); ++row) {
                for (auto col = 0u; col < m_tableResult.NumCols(); ++col) {
                    // Set all the possible positions
                    auto    ioss{ riddle.back().m_indexOfShapeSet };
                    auto    & currentShapeSet{ m_shapeCollections[ioss] };
                    for (size_t shIdxIS = 0; shIdxIS < currentShapeSet.size(); ++shIdxIS) {
                        // Try to use the current shape's all possible rotations
                        riddle.back().m_shapeIndexInSet = shIdxIS;
                        const auto& currentShape = currentShapeSet[shIdxIS];
                        Riddle  remainingRiddle{ riddle };
                        SolutionStep& step{ remainingRiddle.back() };
                        Position& pos{ step.m_position };
                        pos.SetPosition(row, col);
                        if (step.m_tableResult.CanAccomodate(pos, currentShape)) {
                            step.m_tableResult.Accomodate(pos, currentShape);
                            // prepare the next step
                            if (remainingRiddle.size() < m_shapeCollections.size()) {
                                remainingRiddle.push_back(step);
                                remainingRiddle.back().m_indexOfShapeSet += 1;
                                Solve(remainingRiddle);
                            } else {
                                // we are done, and happy with the first solution found!
                                m_isSolved      = true;
                                m_solution      = remainingRiddle;
                                return;
                            }
                        }
                    }
                }
            }
            return;
        }
        return;
    }

    // global operators
    ostream& operator << (ostream            & os,
                          const Tesselation  & tessy)
    {
        bool bPrev{ tessy.SetTableShapeShowZeros(true) };
        os << "Table" << tessy.GetTableShape();
        (void)tessy.SetTableShapeShowZeros(bPrev);
        os << "Tesselation Date = " << tessy.GetDate().DateStr() << endl;
        os << tessy.Result();
        return os;
    }
} // namespace Nessie

using namespace Nessie;

int main ()
{
    ElapsedTime et;

    int         year            = 2024;
    MonthEnum   month           = MonthEnum::February;
    int         day             = 29;
    Date        date (year,
                      month,
                      day);

    bool        useCurrentTime = true;

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
