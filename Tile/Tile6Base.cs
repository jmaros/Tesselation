using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class Tile6Base
    {
        int[,] table = new int[9, 9];

        List<int> shape1 = new List<int> { 1, 1, 1, 1 };
        List<int> shape2 = new List<int> { 3, 3, 2, 6 };
        List<int> shape3 = new List<int> { 3, 4, 3, 3 };
        List<int> shape4 = new List<int> { 1, 1, 1, 6 };
        List<int> shape5 = new List<int> { 6, 6, 2, 6, 6 };
        List<int> shape6 = new List<int> { 1, 1, 3, 4, 4 };
        List<int> shape7 = new List<int> { 1, 2, 4, 4 };
        List<int> shape8 = new List<int> { 1, 6, 6, 1 };
        List<int> shape9 = new List<int> { 6, 6, 1, 1 };
        List<int> shape10 = new List<int> { 2, 6, 6, 2 };
        List<int> shape11 = new List<int> { 1, 1, 3, 3, 5 };
        List<List<int>> listOfShapes = new List<List<int>>();

        List<Pair<int>> pairsInhibit = new List<Pair<int>>();

        List<ShapeStatus> shapeStatusList = new List<ShapeStatus>();

        bool stepBack = false;
        int stepBackCounter = 0;
        ShapeStatus stepBackStatus = new ShapeStatus();

        protected TimerTest timerTest { get; }
        protected long elapsedTotal;

        protected bool needFeedBack = false;

        protected int lastX = 0;
        protected int lastY = 0;
        protected int lastR = 0;

        protected List<Pair<int>> lastTerritory = new List<Pair<int>>();

        public Tile6Base ()
        {
            timerTest = new TimerTest();
            elapsedTotal = 0;
        }

        protected void InitTableBase()
        {
            pairsInhibit.Add(new Pair<int>(0, 0));
            pairsInhibit.Add(new Pair<int>(1, 0));
            pairsInhibit.Add(new Pair<int>(7, 0));
            pairsInhibit.Add(new Pair<int>(8, 0));
            pairsInhibit.Add(new Pair<int>(0, 1));
            pairsInhibit.Add(new Pair<int>(7, 1));
            pairsInhibit.Add(new Pair<int>(8, 1));
            pairsInhibit.Add(new Pair<int>(0, 2));
            pairsInhibit.Add(new Pair<int>(8, 2));
            pairsInhibit.Add(new Pair<int>(8, 3));
            pairsInhibit.Add(new Pair<int>(8, 5));
            pairsInhibit.Add(new Pair<int>(0, 6));
            pairsInhibit.Add(new Pair<int>(8, 6));
            pairsInhibit.Add(new Pair<int>(0, 7));
            pairsInhibit.Add(new Pair<int>(7, 7));
            pairsInhibit.Add(new Pair<int>(8, 7));
            pairsInhibit.Add(new Pair<int>(0, 8));
            pairsInhibit.Add(new Pair<int>(1, 8));
            pairsInhibit.Add(new Pair<int>(7, 8));
            pairsInhibit.Add(new Pair<int>(8, 8));

            foreach (var pair in pairsInhibit)
            {
                SetTable(0xf, pair);
            }

            listOfShapes.Add(shape1);
            listOfShapes.Add(shape2);
            listOfShapes.Add(shape3);
            listOfShapes.Add(shape4);
            listOfShapes.Add(shape5);
            listOfShapes.Add(shape6);
            listOfShapes.Add(shape7);
            listOfShapes.Add(shape8);
            listOfShapes.Add(shape9);
            listOfShapes.Add(shape10);
            listOfShapes.Add(shape11);
        }

        protected void SetTable(int value, Pair<int> pair)
        {
            table[pair.Value2, pair.Value1] = value;
        }

        protected void DumpTable()
        {
            Console.WriteLine("Dump table - begin");
            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (y % 2 == 0)
                    {
                        Console.Write($"{table[y, x].ToString("X")}  ");
                    }
                    else
                    {
                        Console.Write($" {table[y, x].ToString("X")} ");
                    }
                }
                Console.WriteLine("");
            }
            Console.WriteLine("Dump table - end");
        }

        bool TestTableIndex(int x, int y)
        {
            if (x < 0 || y < 0 || x >= 9 || y >= 9)
                return false;
            return true;
        }

        int TestTableValue(int x, int y)
        {
            if (TestTableIndex(x, y))
                return table[y, x];
            else
                return -1;
        }

        int ToHashValue(int x, int y)
        {
            return x + y * 10;
        }

        int CheckNeighbours(ref HashSet<int> tableHashSet, ref List<Pair<int>> neighboursList, int x, int y)
        {
            int x1 = x;
            if (y % 2 == 0)
            {
                x1--;
            }

            neighboursList.Clear();

            if (TestTableValue(x1, y - 1) == 0 && !tableHashSet.Contains(ToHashValue(x1, y - 1)))
            {
                neighboursList.Add(new Pair<int>(x1, y - 1));
            }
            if (TestTableValue(x1 + 1, y - 1) == 0 && !tableHashSet.Contains(ToHashValue(x1 + 1, y - 1)))
            {
                neighboursList.Add(new Pair<int>(x1 + 1, y - 1));
            }
            if (TestTableValue(x - 1, y) == 0 && !tableHashSet.Contains(ToHashValue(x1 - 1, y)))
            {
                neighboursList.Add(new Pair<int>(x - 1, y));
            }
            if (TestTableValue(x + 1, y) == 0 && !tableHashSet.Contains(ToHashValue(x1 + 1, y)))
            {
                neighboursList.Add(new Pair<int>(x + 1, y));
            }
            if (TestTableValue(x1, y + 1) == 0 && !tableHashSet.Contains(ToHashValue(x1, y + 1)))
            {
                neighboursList.Add(new Pair<int>(x1, y + 1));
            }
            if (TestTableValue(x1 + 1, y + 1) == 0 && !tableHashSet.Contains(ToHashValue(x1 + 1, y + 1)))
            {
                neighboursList.Add(new Pair<int>(x1 + 1, y + 1));
            }
            return neighboursList.Count;
        }

        bool CheckIslands()
        {
            HashSet<int> tableHashSetCheck = new HashSet<int>();
            HashSet<int> tableHashSetAdd = new HashSet<int>();


            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (!tableHashSetCheck.Contains(ToHashValue(x, y)) && TestTableValue(x, y) == 0)
                    {
                        List<Pair<int>> pairList = new List<Pair<int>>();
                        List<Pair<int>> neighboursList = new List<Pair<int>>();

                        tableHashSetCheck.Add(ToHashValue(x, y));
                        tableHashSetAdd.Add(ToHashValue(x, y));
                        pairList.Add(new Pair<int>(x, y));

                        CheckNeighbours(ref tableHashSetCheck, ref neighboursList, x, y);

                        while (neighboursList.Count > 0)
                        {

                            foreach (Pair<int> pair in neighboursList)
                            {
                                if (!tableHashSetAdd.Contains(ToHashValue(pair.Value1, pair.Value2)))
                                {
                                    pairList.Add(pair);
                                    tableHashSetAdd.Add(ToHashValue(pair.Value1, pair.Value2));
                                }
                            }
                            neighboursList.Clear();

                            //foreach (Pair<int> pair in pairList)
                            {
                                //tableHashSet
                            }
                            foreach (Pair<int> pair in pairList)
                            {
                                if (!tableHashSetCheck.Contains(ToHashValue(pair.Value1, pair.Value2)))
                                {
                                    tableHashSetCheck.Add(ToHashValue(pair.Value1, pair.Value2));
                                    CheckNeighbours(ref tableHashSetCheck, ref neighboursList, pair.Value1, pair.Value2);
                                    if (neighboursList.Count > 0)
                                        break;
                                }
                            }
                        }
                        if (pairList.Count < 5)
                        {
                            //Console.WriteLine($"CheckIslands: {x}, {y} = {pairList.Count}");
                            //DumpTable();
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        bool PlaceShape(int shapeIndex, List<int> shape, int x, int y, int r)
        {
            if (table[y, x] != 0)
                return false;

            bool ret = true;
            List<Pair<int>> pairList = new List<Pair<int>>();
            pairList.Add(new Pair<int>(x, y));
            int x1 = x;
            int y1 = y;
            foreach (int ic in shape)
            {
                int x2 = x1;
                int y2 = y1;
                int icLoc = (ic + r - 1) % 6;
                switch (icLoc)
                {
                    case 0:
                        x2 = x1 + 1;
                        break;
                    case 1:
                        if (y1 % 2 != 0)
                            x2 = x1 + 1;
                        y2 = y1 + 1;
                        break;
                    case 2:
                        if (y1 % 2 == 0)
                            x2 = x1 - 1;
                        y2 = y1 + 1;
                        break;
                    case 3:
                        x2 = x1 - 1;
                        break;
                    case 4:
                        if (y1 % 2 == 0)
                            x2 = x1 - 1;
                        y2 = y1 - 1;
                        break;
                    case 5:
                        if (y1 % 2 != 0)
                            x2 = x1 + 1;
                        y2 = y1 - 1;
                        break;
                }
                if (x2 < 0 || x2 >= 9 || y2 < 0 || y2 >= 9)
                    return false;
                if (table[y2, x2] != 0)
                    return false;
                pairList.Add(new Pair<int>(x2, y2));
                x1 = x2;
                y1 = y2;
            }
            foreach (Pair<int> pair in pairList)
            {
                SetTable(shapeIndex, pair);
            }
            if (CheckIslands() == false)
            {
                foreach (Pair<int> pair in pairList)
                {
                    SetTable(0, pair);
                }
                return false;
            }

            shapeStatusList.Add(new ShapeStatus(shapeIndex, new Pair<int>(x, y), r));

            if (needFeedBack)
            {
                lastX = x;
                lastY = y;
                lastR = r;
                lastTerritory = pairList;
            }
            // remove later begin
            //DumpTable();
            // remove later end
            return ret;
        }

        bool PlaceShape(int shapeIndex, List<int> shape, int x, int y)
        {
            bool ret = false;
            for (int r = 0; r < 6; ++r)
            {
                if (stepBack)
                {
                    r = stepBackStatus.rotation;
                    stepBack = false;
                    continue;
                }
                ret = PlaceShape(shapeIndex, shape, x, y, r);
                if (ret)
                    break;
            }
            return ret;
        }

        protected bool PlaceShape(int shapeIndex, List<int> shape)
        {
            bool ret = false;
            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (stepBack)
                    {
                        x = stepBackStatus.position.Value1;
                        y = stepBackStatus.position.Value2;
                    }
                    if (table[y, x] == 0)
                    {
                        ret = PlaceShape(shapeIndex, shape, x, y);
                        if (ret)
                            break;
                    }
                }
                if (ret)
                    break;
            }
            return ret;
        }

        void RemoveShapeFromTable(int shapeIndex)
        {
            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (table[y, x] == shapeIndex)
                    {
                        table[y, x] = 0;
                    }
                }
            }
        }

        bool StepBack()
        {
            bool ret = true;
            if (shapeStatusList.Count == 0)
                return false;
            stepBackStatus = shapeStatusList[shapeStatusList.Count - 1];
            stepBack = true;
            stepBackCounter++;
            shapeStatusList.RemoveAt(shapeStatusList.Count - 1);
            RemoveShapeFromTable(stepBackStatus.shapeIndex);

            if (stepBackCounter % 500000 == 0)
            {
                Console.WriteLine($"StepBack {stepBackCounter}");
                foreach (ShapeStatus stat in shapeStatusList)
                {
                    Console.WriteLine(stat.ToString());
                }
                Console.WriteLine($"stepBackStatus: {stepBackStatus.ToString()}");
                DumpTable();

                long ms = timerTest.Check();

                elapsedTotal += ms;
                Console.WriteLine($"Timer: {ms} / {elapsedTotal}");
            }

            return ret;
        }

        protected bool PlaceOneShape(int shapeIndex,
            ref List<ShapePosition> shapePositionList,
            ref List<List<Pair<int>>> territoryList)
        {
            bool ret = false;
            int x = 0, y = 0, r = 0;
            HashSet<long> territoryHashSet = new HashSet<long>();
            for (int i = 0; i < 50 * 6; ++i)
            {
                needFeedBack = true;
                if (shapeIndex < 1 || shapeIndex > listOfShapes.Count)
                    return false;
                List<int> shape = listOfShapes[shapeIndex - 1];
                ret = PlaceShape(shapeIndex, shape);
                if (ret)
                {
                    long hashActual = HashUtility.GenerateHash(lastTerritory);
                    if (territoryHashSet.Contains(hashActual))
                    {
                        //DumpTable();
                        //Console.WriteLine($"PlaceOneShape index: {shapeIndex}, nr: {shapePositionList.Count} already have it");
                    }
                    else
                    {
                        territoryHashSet.Add(hashActual);
                        ShapePosition pos = new ShapePosition(lastR, new Pair<int>(lastX, lastY));
                        shapePositionList.Add(pos);
                        territoryList.Add(lastTerritory);
                        //Console.WriteLine($"PlaceOneShape index: {shapeIndex}, nr: {shapePositionList.Count}");
                    }
                }
                StepBack();
                if (lastX == x && lastY == y && lastR == r)
                {
                    break;
                }
                x = lastX;
                y = lastY;
                r = lastR;
            }
            return ret;
        }


        protected bool PlaceAllShapes()
        {
            bool ret = false;
            needFeedBack = false;

            for (int shapeIndex = 1; shapeIndex <= 11 && shapeIndex > 0; shapeIndex++)
            {
                List<int> shape = listOfShapes[shapeIndex - 1];
                ret = PlaceShape(shapeIndex, shape);
                if (!ret)
                {
                    StepBack();
                    shapeIndex -= 2;
                }
            }
            Console.WriteLine($"PlaceShape:{ret}");
            return ret;
        }

    }
}
