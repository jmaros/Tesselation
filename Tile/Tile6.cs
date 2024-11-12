using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Tile
{
    internal class Tile6 : Tile6Base
    {

        List<Pair<int>> pairsMonthList = new List<Pair<int>>();
        List<Pair<int>> pairsWeekDayList = new List<Pair<int>>();
        List<Pair<int>> pairsDayList = new List<Pair<int>>();
        ShapePossiblePosition shapePossiblePosition = new ShapePossiblePosition (0);
        List<ShapePossiblePosition> shapePossiblePositionList = new List<ShapePossiblePosition>();

        const int ThreadNumber = 11;
        public int month { get; set; }
        public int mday { get; set; }
        public int wday { get; set; }

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

        public void ExecuteAllShape(int monthIn, int mdayIn, int wdayIn)
        {
            InitTable(monthIn, mdayIn, wdayIn);
            DumpTable();
            bool result = PlaceAllShapes();
            Console.WriteLine($"Execute:{result}");
            long elapsedTotalLocal = elapsedTotal + timerTest.Check();
            Console.WriteLine($"Timer total: {elapsedTotalLocal}");

            DumpTable();
        }

        public bool CalculateOneShape()
        {
            InitTableBase();
            return true;
        }

        public void ExecuteOneShape(int shapeIndex)
        {
            //DumpTable();

            //List<ShapePosition> spList = new List<ShapePosition>();
            //List<List<Pair<int>>> tList = new List<List<Pair<int>>>();
            List<ShapePosition> spList = shapePossiblePosition.shapePositionList;
            List<List<Pair<int>>> tList = shapePossiblePosition.territoryList;

            bool result = PlaceOneShape(shapeIndex, ref spList, ref tList);
            Console.WriteLine($"Execute:{result}");

            //long elapsedTotalLocal = elapsedTotal + timerTest.Check();
            //Console.WriteLine($"Timer total: {elapsedTotalLocal}");

            //DumpTable();
            
        }


        public void PossiblePositionCall()
        {
            Console.WriteLine($"MultiTaskCall is:{shapePossiblePosition.shapeIndex}");
            ExecuteOneShape(shapePossiblePosition.shapeIndex);
        }

        public void CalculatePossiblePosition(Tile6[] tile6Array)
        {
            TimerTest timerTest = new TimerTest();
            Thread[] threads = new Thread[ThreadNumber];

            
            for (int shapeIndex = 1; shapeIndex <= 11;)
            {
                for (int tn = 0; tn < ThreadNumber && shapeIndex <= 11; tn++)
                {
                    int currentThreadIndex = tn;
                    int currentSi = shapeIndex;
                    tile6Array[currentThreadIndex] = new Tile6();
                    tile6Array[currentThreadIndex].InitTableBase ();
                    tile6Array[currentThreadIndex].tile6ID = shapeIndex;
                    tile6Array[currentThreadIndex].shapePossiblePosition = new ShapePossiblePosition(shapeIndex);
                    threads[currentThreadIndex] = new Thread(() => tile6Array[currentThreadIndex].PossiblePositionCall());
                    threads[currentThreadIndex].Start();
                    shapeIndex++;
                }
                foreach (var thread in threads)
                {
                    thread.Join();
                }
                foreach (var tile in tile6Array)
                {
                    Console.WriteLine($"ShapePossiblePosition: {tile.shapePossiblePosition.shapeIndex}, {tile.shapePossiblePosition.shapePositionList.Count}");
                    shapePossiblePositionList.Add(tile.shapePossiblePosition);
                }

            }

            Console.WriteLine($"ShapePossiblePosition time:{timerTest.Check()}");
        }
 
        public void PlaceAllShapesCall(ref List<ShapePossiblePosition> sppList)
        {
            TimerTest timerTest = new TimerTest();
            //Tile6 tileThread = new Tile6();
            InitTable(month, mday, wday);
            //tileThread.DumpTable();
            bool result = PlaceAllShapes(stepIndex, ref sppList);
            //Console.WriteLine($"Execute:{result}");
            if (result)
            {
                Console.WriteLine($"Done step index: {stepIndex}");
                long elapsedTotalLocal = timerTest.Check();
                Console.WriteLine($"Timer total: {elapsedTotalLocal}");
                DumpTable();
            }

        }

        public void Execute(int monthIn, int mdayIn, int wdayIn)
        {
            month = monthIn;
            mday = mdayIn;
            wday = wdayIn;

            //HashUtility.TestGenerateHashListPair();

            //const int threadNums = ShapeNumber;
            TimerTest timerTest = new TimerTest();
            Tile6[] tile6Array = new Tile6[ThreadNumber];
            CalculatePossiblePosition (tile6Array);

            Thread[] threads = new Thread[ThreadNumber];

            List<ShapePossiblePosition> sppList = new List<ShapePossiblePosition>();
            sppList = shapePossiblePositionList;

            int stepIndex = 0;
            do {
                for (int ti = 0; ti < ThreadNumber; ti++)
                {
                    int currentTi = ti;
                    int currentStepIndex = stepIndex;
                    tile6Array[currentTi] = new Tile6();
                    tile6Array[currentTi].stepIndex = currentStepIndex;
                    tile6Array[currentTi].tile6ID = currentStepIndex;
                    tile6Array[currentTi].month = month;
                    tile6Array[currentTi].mday = mday;
                    tile6Array[currentTi].wday = wday;
                    threads[currentTi] = new Thread(() => tile6Array[currentTi].PlaceAllShapesCall(ref sppList));
                    threads[currentTi].Start();
                    stepIndex++;
                }

                foreach (var thread in threads)
                {
                    thread.Join();
                }
                foreach(Tile6 tile in tile6Array)
                {
                    Console.WriteLine($"stepIndex: {tile.stepIndex}");
                    Console.WriteLine($"id: {tile.tile6ID}");
                    Console.WriteLine($"CntPlaceShape: {tile.GetCntPlaceShape()}");
                    Console.WriteLine($"SipiListCountMax: {tile.GetSipiListCountMax()}");
                    tile.DumpTable();
                }

                {
                    long ms = timerTest.Check();
                    elapsedTotal += ms;

                    if (elapsedTotal % 1000 == 0)
                    {
                        Console.WriteLine($"stepIndex: {stepIndex}");
                        Console.WriteLine($"Timer: {elapsedTotal}");
                    }

                }

            } while ( stepIndex < shapePossiblePositionList[0].territoryList.Count);
        }
    }
}
