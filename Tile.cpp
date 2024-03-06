//
// Tile.cpp
//  : This file contains the 'main' function. Program execution begins and ends there.
//
// You can compile this code by using Visual Studio (2022 Community Edition)
//
// or you can compile on Windows using MinGW and a specific c++ version:
// test Tile c++23
//
// or you can compile on Linux using a specific c++ version:
// ./test.sh Tile c++23
//
// (optionally you can add " -D VERBOSE" at the end of the command line)
//
/* You might have to convert crlf to lf in test.sh using sed like this:
sed -i 's/\r
/
/g' test.sh
*/
// or you can use
// dos2unix test.sh
// for the same reason, and
// chmod +x test.sh
//
// to make it executable after cloning the repository.
//

#if defined (_MSC_VER) && _MSC_VER >= 1900 && !defined (_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Tile.h"
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

    const string    ShapeLetters{ "ABCDEFGH" };
    const string    ShapeChars{ "@#$*+ox&" };

    // constructors
    Tile::Tile (const TableLayout                 & table,
                              const ShapeCollection             & shapes,
                              const ApplicationSpecificOptions  & options)
     : m_tableLayout        (table)
     , m_shapes             (shapes)
     , m_options            (options)
     , m_tableShape         (table.NumRows(),
                             table.NumCols())
     , m_monthMap           ()
     , m_dayMap             ()
     , m_shapeCollections   ()
     , m_tableResult        (table.NumRows(),
                             table.NumCols())
     , m_solutions          ()
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

        if (m_options.Verbose()) {
            cout << "\nMain Table:" << table;
            int shNum {};
            int shsNum {};
            for (auto   & shapeColl : m_shapeCollections) {
                cout << ++shsNum << ". ShapeSet:\n";
                int shssNum{};
                for (auto   & shape : shapeColl) {
                    cout << ++shNum << ".(" << shsNum << '.' << ++shssNum << ")." << shape;
                }
            }
        }

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
        m_tableShape.SetData(m_monthMap[m_options.GetDate().GetMonth()], true);
        m_tableShape.SetData(m_dayMap[m_options.GetDate().GetDay()], true);

        // Setting the initial valu of the resulting table and the
        // required return value for the out of bound positions
        m_tableResult = m_tableShape;
        m_tableResult.SetOutOfBoundValue(true);
    }

    // accessors
    inline const Date & Tile::GetDate ()   const
    {
        return m_options.GetDate();
    }

    string  Tile::GetTableResultStr (const Solution     & solution,
                                            size_t             index) const
    {
        stringstream        srs;
        TableResultChars    result(m_tableResult.NumRows(),
                                   m_tableResult.NumCols(),
                                   ' ');

        for (auto& solStep : solution) {
            auto& shapes = m_shapeCollections[solStep.m_indexOfShapeSet];
            const TableResult& theShape{ shapes[solStep.m_shapeIndexInSet] };
            TableResultChars chrep(theShape.NumRows(),
                                   theShape.NumCols(),
                                   ' ');

            for (size_t r = 0; r < chrep.NumRows(); ++r) {
                for (size_t c = 0; c < chrep.NumCols(); ++c) {
                    Position chPos(r, c);
                    if (theShape.Value(chPos)) {
                        if (m_options.GetOpted().m_useLetters) {
                            chrep.SetData(chPos, ShapeLetters[solStep.m_indexOfShapeSet]);
                        } else {
                            chrep.SetData(chPos, ShapeChars[solStep.m_indexOfShapeSet]);
                        }
                    }
                }
            }
            bool bSucc = result.Accomodate(solStep.m_position, chrep);
            if (!bSucc) {
                srs << "Failed to place the shape\n" << chrep << "at position: " << solStep.m_position;
                break;
            }
        }
        result.SetShapeName("Solution", index);
        srs << result;
        return srs.str();
    }

    string    Tile::Result () const
    {
        if (m_solutions.empty()) {
            return "No solution found!\n";
        }

        size_t             index{};
        stringstream        srs;

        for (auto& solution : m_solutions) {
            srs << GetTableResultStr (solution, ++index);
        }
        return srs.str();
    }

    bool Tile::SetTableShapeShowZeros (bool  bShow) const
    {
        return m_tableShape.SetMutableShowZeros(bShow);
    }

    const TableResult& Tile::GetTableShape () const
    {
        return m_tableShape;
    }

    const TableResult& Tile::GetTableResult () const
    {
        return m_tableResult;
    }

    size_t Tile::RandomValue (size_t     maxValue) const
    {
        static std::mt19937              mt(random_device{}());
        uniform_int_distribution<size_t> value(0, maxValue);
        return value(mt);
    }

    // modifiers
    bool Tile::Solve (Riddle     &riddle)
    {
        SolutionStep* actualStepPtr = riddle.GetSolutionStepPtr();

        if (actualStepPtr == nullptr) {
            cout << "Unexpected failure!" << endl;
            return false;
        }

        SolutionStep & actualStep = *actualStepPtr;
        size_t actualLevel = riddle.GetLevel();

        if (riddle.GetLevel() == 0) {
            // prepare the first step
            if (m_options.Random()) {
                RandomShuffle(m_shapeCollections);
            }
        }
        TableResult savedTableResult = actualStep.m_tableResult;
        for (auto row = 0u; row < m_tableResult.NumRows(); ++row) {
            for (auto col = 0u; col < m_tableResult.NumCols(); ++col) {
                // Set all the possible positions
                auto    ioss{ actualStep.m_indexOfShapeSet };
                auto    & currentShapeSet{ m_shapeCollections[ioss] };
                for (size_t shIdxIS = 0; shIdxIS < currentShapeSet.size(); ++shIdxIS) {
                    // Try to use the current shape's all possible rotations
                    actualStep.m_shapeIndexInSet = shIdxIS;
                    const auto& currentShape = currentShapeSet[shIdxIS];
                    Position& pos{ actualStep.m_position };
                    pos.SetPosition(row, col);
                    if (actualStep.m_tableResult.CanAccomodate(pos, currentShape)) {
                        actualStep.m_tableResult.Accomodate(pos, currentShape);
#if defined (VERBOSE)
                        cout << riddle.GetLevel() + 1 << ". " << actualStep.m_tableResult;
#endif
                        // prepare the next step
                        if (riddle.GetLevel() + 1 < m_shapeCollections.size()) {
                            riddle.SetNextLevel();
                            bool justSolved = Solve(riddle);
                            if (justSolved) {
                                if (!m_options.CalculateAll()) {
                                    return true;
                                }
                            }
                            riddle.RestorePreviousLevel(actualLevel, savedTableResult);
                        } else {
                            // be happy, we just have found a solution!
                            m_solutions.push_back(riddle.GetSolution());
                            if (m_options.CalculateAll()) {
                                cout << GetTableResultStr(riddle.GetSolution(), m_solutions.size());
                            }
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    // global operators
    ostream& operator << (ostream            & os,
                          const Tile  & tessy)
    {
        bool bPrev{ tessy.SetTableShapeShowZeros(true) };
        os << "Table" << tessy.GetTableShape();
        (void)tessy.SetTableShapeShowZeros(bPrev);
        os << "Tile Date = " << tessy.GetDate().DateStr() << endl;
        os << tessy.Result();
        return os;
    }
} // namespace Nessie

using namespace Nessie;

int main (int argc,
          char *argv[],
          char *arge[])
{
    ElapsedTime et;

    ApplicationSpecificOptions options(argc, argv, arge);
    options.Evaluate();
    options.SetOpted();
    if (options.AskedForHelp()) {
        cout << "Usage:\n"
            << "  Tile [options]\n"
            << " where options can be:\n"
            << "  -h or --help for this help\n"
            << "  -r or --random to find a random solution\n"
            << "  -v or --verbose for more detailed output\n"
            << "  -a or --all for finding all solution\n"
            << "  -y or --year followed by yyyy\n"
            << "  -m or --month followed by [m]m\n"
            << "  -d or --day followed by [d]d\n"
            << "  -@ to use special characters\n"
            << "     instead of letters [A-H] for the shapes\n"
            << endl;
    } else {
        if (options.IsValid()) {
            Date date{ options.GetDate() };
            if (date.IsValidDate()) {
                Tile tile (MainTable,
                           Shapes,
                           options);
                Riddle riddle(tile.GetTableResult());
                tile.Solve (riddle);
                cout << tile << "\n";
            } else {
                cout << "No such date as " << date.DateStr() << endl;
            }
        }else {
          cout << "Error: Invalid commandline option(s)!" << endl;
        }
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
