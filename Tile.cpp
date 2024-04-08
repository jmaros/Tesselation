//
// Tile.cpp
//  : This file contains the 'main' function. Program execution begins and ends there.
//
// You can compile this code by using Visual Studio (2022 Community Edition)
//
// or you can compile on Windows using MinGW and a specific c++ version:
// test Tile P3Vector.cpp c++23
//
// or you can compile on Linux using a specific c++ version:
// ./test.sh Tile P3Vector.cpp c++23
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

    // The same as before, plus 100 for not used, 101 for Sunday, 102 for Monday, ...., 107 for Saturday
    const TableLayout MainTable6 = {
        {   0,   0,   0,   0,  -5,   0,  -6,   0, 100,   0,  -7,   0,  -8,   0,   0,   0,   0 },
        {   0,   0,   0,  -4,   0, 101,   0, 102,   0, 103,   0, 104,   0,  -9,   0,   0,   0 },
        {   0,   0,  -3,   0, 100,   0, 105,   0, 106,   0, 107,   0, 100,   0, -10,   0,   0 },
        {   0,  -2,   0,   1,   0,   2,   0,   3,   0,   4,   0,   5,   0,   6,   0, -11,   0 },
        {  -1,   0,   7,   0,   8,   0,   9,   0,  10,   0,  11,   0,  12,   0,  13,   0, -12 },
        {   0, 100,   0,  14,   0,  15,   0,  16,   0,  17,   0,  18,   0,  19,   0, 100,   0 },
        {   0,   0, 100,   0,  20,   0,  21,   0,  22,   0,  23,   0,  24,   0, 100,   0,   0 },
        {   0,   0,   0, 100,   0,  25,   0,  26,   0,  27,   0,  28,   0, 100,   0,   0,   0 },
        {   0,   0,   0,   0, 100,   0,  29,   0,  30,   0,  31,   0, 100,   0,   0,   0,   0 },
    };

    const ShapeCollection Shapes = {

        {{ 1, 0, 1 },
         { 1, 1, 1 }},      // sh01

        {{ 1, 1, 1 },
         { 1, 1, 1 }},      // sh02

        {{ 1, 1, 1 },
         { 1, 1, 0 }},      // sh03

        {{ 1, 0, 0 },
         { 1, 0, 0 },
         { 1, 1, 1 }},      // sh04

        {{ 1, 1, 0 },
         { 0, 1, 0 },
         { 0, 1, 1 }},      // sh05

        {{ 1, 1, 1, 1 },
         { 0, 1, 0, 0 }},   // sh06

        {{ 1, 1, 1, 0 },
         { 0, 0, 1, 1 }},   // sh07

        {{ 1, 1, 1, 1 },
         { 1, 0, 0, 0 }}    // sh08
    };

    const ShapeCollection Shape6s = {

        {{ 1, 0, 1, 0, 1, 0, 1, 0, 1 }},    // sh01

        {{ 0, 0, 0, 1, 0, 1 },
         { 1, 0, 1, 0, 1, 0 }},             // sh02

        {{ 0, 0, 0, 1, 0, 1, 0, 1 },
         { 1, 0, 1, 0, 0, 0, 0, 0 }},       // sh03

        {{ 1, 0, 1, 0, 1, 0, 1, 0 },
         { 0, 0, 0, 0, 0, 0, 0, 1 }},       // sh04

        {{ 1, 0, 1, 0, 1, 0, 0, 0 },
         { 0, 0, 0, 1, 0, 1, 0, 1 }},       // sh05

        {{ 0, 1, 0, 1, 0, 1 },
         { 1, 0, 1, 0, 1, 0 }},             // sh06

        {{ 0, 1, 0, 1, 0 },
         { 1, 0, 1, 0, 1 }},                // sh07

        {{ 1, 0, 0, 0, 0, 0, 0 },
         { 0, 1, 0, 1, 0, 1, 0 },
         { 0, 0, 0, 0, 0, 0, 1 }},          // sh08

        {{ 1, 0, 0, 0, 0, 0, 0 },
         { 0, 1, 0, 0, 0, 0, 0 },
         { 0, 0, 1, 0, 1, 0, 1 }},          // sh09

        {{ 1, 0, 1 },
         { 0, 1, 0 },
         { 1, 0, 1 }},                      // sh10

        {{ 0, 0, 1, 0, 0 },
         { 0, 1, 0, 1, 0 },
         { 1, 0, 1, 0, 1 }}                 // sh11

    };

    const string    ShapeLetters{ "ABCDEFGHIJKL" };
    const string    ShapeChars  { "@#$*+ox&abcd" };

    // constructors
    Tile::Tile (const TableLayout       & table,
                const ShapeCollection   & shapes,
                const TileOptions       & options)
     : m_tableLayout        (table)
     , m_shapes             (shapes)
     , m_options            (options)
     , m_tableShape         (table.NumRows(),
                             table.NumCols())
     , m_monthMap           ()
     , m_dayMap             ()
     , m_dayOfWeekMap       ()
     , m_shapeCollections   ()
     , m_tableResult        (table.NumRows(),
                             table.NumCols())
     , m_solutions          ()
    {
        // fill m_shapesSet so that it will contain all the different shapes
        size_t seti{};
        for (auto   & originShape : m_shapes) {
            ShapeSet    shapeSet;
            bool succi = shapeSet.insert(originShape).second;
            if (succi) {
                // originShape is unique in shapeSet
                if (options.IsHexagonal()) {
                    auto    AddRotated60  = [&shapeSet] (const TableResult  & shape) -> TableResult
                    {
                        auto shapeRot60 = shape.CreateRotatedBy60Deg();
                        (void)shapeSet.insert(shapeRot60);
                        return shapeRot60;
                    };
 
                    auto    AddFiveMoreRotatedShapes = [&](const TableResult& oriShape) -> void
                        {
                            auto oriShapeRot060 = AddRotated60(oriShape);
                            auto oriShapeRot120 = AddRotated60(oriShapeRot060);
                            auto oriShapeRot180 = AddRotated60(oriShapeRot120);
                            auto oriShapeRot240 = AddRotated60(oriShapeRot180);
                            auto oriShapeRot300 = AddRotated60(oriShapeRot240);
                        };

                    bool verbose = true;
                    if (verbose) {
                        cout << "Hexagonal Shape Set-" << ++seti
                             << "(" << shapeSet.size()
                             << ((shapeSet.size() > 1) ? " shapes)" : " shape)")
                             << endl;
                    }
                    AddFiveMoreRotatedShapes(originShape);
                    auto horiFlipped = originShape.CreateHorizontallyFlipped();
                    (void)shapeSet.insert(horiFlipped);
                    AddFiveMoreRotatedShapes(horiFlipped);

                    auto vertFlipped = originShape.CreateVerticallyFlipped();
                    (void)shapeSet.insert(vertFlipped);
                      
                    AddFiveMoreRotatedShapes(vertFlipped);
                } else {
                    auto horiFlipped = originShape.CreateHorizontallyFlipped();
                    auto vertFlipped = originShape.CreateVerticallyFlipped();
                    auto bothFlipped = horiFlipped.CreateVerticallyFlipped();
                    (void)shapeSet.insert(horiFlipped);
                    (void)shapeSet.insert(vertFlipped);
                    (void)shapeSet.insert(bothFlipped);
                    (void)shapeSet.insert(originShape.CreateTransposed());
                    (void)shapeSet.insert(horiFlipped.CreateTransposed());
                    (void)shapeSet.insert(vertFlipped.CreateTransposed());
                    (void)shapeSet.insert(bothFlipped.CreateTransposed());
                }
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
                Position<size_t> pos(row, col);
                auto newValue = m_tableLayout.Value(pos);
                m_tableShape.SetData(pos, newValue == 0);
                if (newValue < 0) {
                    m_monthMap[MonthEnum(-newValue)] = pos;
                } else
                if (newValue > 0) {
                    if (newValue < 100) {
                        m_dayMap[newValue] = pos;
                    }
                    if (newValue > 100) {
                        m_dayOfWeekMap[DayOfWeekEnum(newValue - 100)] = pos;
                    }
                }
            }
        }
        m_monthMap[MonthEnum::LeapFebr] = m_monthMap[MonthEnum::February];

        // Setting the additional element cells:
        m_tableShape.SetData(m_monthMap[m_options.GetDate().GetMonth()], true);
        m_tableShape.SetData(m_dayMap[m_options.GetDate().GetDay()], true);
        if (m_options.IsHexagonal()) {
            m_tableShape.SetData(m_dayOfWeekMap[m_options.GetDate().GetDayOfWeek()], true);
        }

        // Setting the initial valu of the resulting table and the
        // required return value for the out of bound positions
        m_tableResult = m_tableShape;
        m_tableResult.SetOutOfBoundValue(true);
    }

    // accessors
    inline const Date   & Tile::GetDate ()      const
    {
        return m_options.GetDate();
    }

    inline const TileOptions    & Tile::GetOptions ()   const
    {
        return m_options;
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
                    Position<size_t> chPos(r, c);
                    if (theShape.Value(chPos) &&
                        (m_options.Steps() == 0 ||
                         solStep.m_indexOfShapeSet < m_options.Steps())) {
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

        if (m_solutions.size() == 1) {
            Solution solout = m_solutions[0];
            srs << GetTableResultStr (solout);
        } else {
            for (auto& solution : m_solutions) {
                srs << GetTableResultStr (solution, index);
            }
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
                for (auto& sc : m_shapeCollections) {
                    RandomShuffle(sc);
                }
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
                    Position<size_t>& pos{ actualStep.m_position };
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
    ostream& operator << (ostream       & os,
                          const Tile    & tessy)
    {
        if (!tessy.GetOptions().Random() &&
            !tessy.GetOptions().CalculateAll()) {
            bool bPrev{ tessy.SetTableShapeShowZeros(true) };
            os << "Table" << tessy.GetTableShape();
            (void)tessy.SetTableShapeShowZeros(bPrev);
        }
        os << "Tile Date = " << tessy.GetDate().DateStr() << endl;
        if (tessy.GetOptions().Random()) {
            os << "Random ";
        }
        if (!tessy.GetOptions().CalculateAll()) {
            os << tessy.Result();
        }
        return os;
    }
} // namespace Nessie

using namespace Nessie;

int main (int   argc,
          char  * argv[],
          char  * arge[])
{
    ElapsedTime et;

    TileOptions options(argc, argv, arge);
    options.Evaluate();
    options.SetOpted();
    if (options.AskedForHelp()) {
        cout << "Usage:\n"
            << "  Tile [options]\n"
            << " where options can be:\n"
            << "  -h or --help for this help\n"
            << "  -6 or --hexagonal to use the hexagonal tiling riddle\n"
            << "  -r or --random to find a random solution\n"
            << "  -s or --steps followed by n, the number of requested hints\n"
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
                if (options.IsHexagonal()) {
                    Tile tile (MainTable6,
                               Shape6s,
                               options);
                    cout << tile.GetTableResult();
                    Riddle riddle(tile.GetTableResult(), Shape6s.size());
                    tile.Solve (riddle);
                    cout << tile << "\n";
                } else {
                    Tile tile (MainTable,
                               Shapes,
                               options);
                    Riddle riddle(tile.GetTableResult(), Shapes.size());
                    tile.Solve (riddle);
                    cout << tile << "\n";
                }
            } else {
                cout << "No such date as " << date.DateStr() << endl;
            }
        } else {
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
