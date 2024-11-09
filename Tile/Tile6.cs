using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class Tile6 : Tile6Base
    {

        List<Pair<int>> pairsMonthList = new List<Pair<int>>();
        List<Pair<int>> pairsWeekDayList = new List<Pair<int>>();
        List<Pair<int>> pairsDayList = new List<Pair<int>>();

        void InitTable(int month, int day, int weekDay)
        {
            InitTableBase();

            pairsMonthList.Add(new Pair<int>(0, 4)); // 1
            pairsMonthList.Add(new Pair<int>(0, 3)); // 2
            pairsMonthList.Add(new Pair<int>(1, 2)); // 3
            pairsMonthList.Add(new Pair<int>(1, 1)); // 4
            pairsMonthList.Add(new Pair<int>(2, 0)); // 5
            pairsMonthList.Add(new Pair<int>(3, 0)); // 6
            
            pairsMonthList.Add(new Pair<int>(5, 0)); // 7
            pairsMonthList.Add(new Pair<int>(6, 0)); // 8
            pairsMonthList.Add(new Pair<int>(6, 1)); // 9
            pairsMonthList.Add(new Pair<int>(7, 2)); // 10
            pairsMonthList.Add(new Pair<int>(7, 3)); // 11
            pairsMonthList.Add(new Pair<int>(8, 4)); // 12

            pairsWeekDayList.Add(new Pair<int>(2, 1));
            pairsWeekDayList.Add(new Pair<int>(3, 1));
            pairsWeekDayList.Add(new Pair<int>(4, 1));
            pairsWeekDayList.Add(new Pair<int>(5, 1));
            pairsWeekDayList.Add(new Pair<int>(3, 2));
            pairsWeekDayList.Add(new Pair<int>(4, 2));
            pairsWeekDayList.Add(new Pair<int>(5, 2));

            pairsDayList.Add(new Pair<int>(1, 3));
            pairsDayList.Add(new Pair<int>(2, 3));
            pairsDayList.Add(new Pair<int>(3, 3));
            pairsDayList.Add(new Pair<int>(4, 3));
            pairsDayList.Add(new Pair<int>(5, 3));
            pairsDayList.Add(new Pair<int>(6, 3));

            pairsDayList.Add(new Pair<int>(1, 4));
            pairsDayList.Add(new Pair<int>(2, 4));
            pairsDayList.Add(new Pair<int>(3, 4));
            pairsDayList.Add(new Pair<int>(4, 4));
            pairsDayList.Add(new Pair<int>(5, 4));
            pairsDayList.Add(new Pair<int>(6, 4));
            pairsDayList.Add(new Pair<int>(7, 4));

            pairsDayList.Add(new Pair<int>(1, 5));
            pairsDayList.Add(new Pair<int>(2, 5));
            pairsDayList.Add(new Pair<int>(3, 5));
            pairsDayList.Add(new Pair<int>(4, 5));
            pairsDayList.Add(new Pair<int>(5, 5));
            pairsDayList.Add(new Pair<int>(6, 5));

            pairsDayList.Add(new Pair<int>(2, 6));
            pairsDayList.Add(new Pair<int>(3, 6));
            pairsDayList.Add(new Pair<int>(4, 6));
            pairsDayList.Add(new Pair<int>(5, 6));
            pairsDayList.Add(new Pair<int>(6, 6));

            pairsDayList.Add(new Pair<int>(2, 7));
            pairsDayList.Add(new Pair<int>(3, 7));
            pairsDayList.Add(new Pair<int>(4, 7));
            pairsDayList.Add(new Pair<int>(5, 7));

            pairsDayList.Add(new Pair<int>(3, 8));
            pairsDayList.Add(new Pair<int>(4, 8));
            pairsDayList.Add(new Pair<int>(5, 8));

            var pairMonth = pairsMonthList[month - 1];
            var pairDay = pairsDayList[day - 1];
            var pairWeekDay = pairsWeekDayList[weekDay - 1];

            SetTable(0xe, pairMonth);
            SetTable(0xe, pairDay);
            SetTable(0xe, pairWeekDay);

        }


        public void Execute()
        {
            InitTable(11,9,7);
            DumpTable();
            bool result = PlaceAllShapes();
            Console.WriteLine($"Execute:{result}");
            long elapsedTotalLocal = elapsedTotal + timerTest.Check();
            Console.WriteLine($"Timer total: {elapsedTotalLocal}");

            DumpTable();
        }
    }
}
