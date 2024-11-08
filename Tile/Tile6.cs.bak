using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class Tile6
    {
        int[,] table = new int[9, 9];

        List<int> shape1 = new List<int> { 1, 1, 1, 1, 1 };
        List<int> shape2 = new List<int> { 3, 5, 6, 6 };
        List<int> shape3 = new List<int> { 6, 6, 1, 6 };
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

        private void InitTable ()
        {
            pairsInhibit.Add(new Pair<int>(0, 0));
            pairsInhibit.Add(new Pair<int>(0, 1));
            pairsInhibit.Add(new Pair<int>(0, 7));
            pairsInhibit.Add(new Pair<int>(0, 8));

            pairsInhibit.Add(new Pair<int>(1, 0));
            pairsInhibit.Add(new Pair<int>(1, 7));
            pairsInhibit.Add(new Pair<int>(1, 8));

            pairsInhibit.Add(new Pair<int>(2, 0));
            pairsInhibit.Add(new Pair<int>(2, 8));

            pairsInhibit.Add(new Pair<int>(3, 8));

            pairsInhibit.Add(new Pair<int>(5, 8));

            pairsInhibit.Add(new Pair<int>(6, 0));
            pairsInhibit.Add(new Pair<int>(6, 8));

            pairsInhibit.Add(new Pair<int>(7, 0));
            pairsInhibit.Add(new Pair<int>(7, 7));
            pairsInhibit.Add(new Pair<int>(7, 8));

            pairsInhibit.Add(new Pair<int>(8, 0));
            pairsInhibit.Add(new Pair<int>(8, 1));
            pairsInhibit.Add(new Pair<int>(8, 7));
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

        private void SetTable (int value, Pair<int> pair)
        {
            table[pair.Value1, pair.Value2] = value;
        }

        private void DumpTable()
        {
            Console.WriteLine("Dump table - begin");
            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (y % 2 == 0)
                    {
                        Console.Write($"{table[y,x].ToString("X")}  ");
                    } else
                    {
                        Console.Write($" {table[y, x].ToString("X")} ");
                    }
                }
                Console.WriteLine("");
            }
            Console.WriteLine("Dump table - end");
        }

        private bool PlaceShape(int shapeIndex, List<int> shape, int x, int y, int r)
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
                switch(icLoc)
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
            foreach (Pair<int>  pair in pairList)
            {
                table[pair.Value2, pair.Value1] = shapeIndex;
            }
            // remove later begin
            DumpTable();
            // remove later end
            return ret;
        }

        private bool PlaceShape (int shapeIndex, List<int> shape, int x, int y)
        {
            bool ret = false;
            for (int r = 0; r < 6; ++r)
            {
                ret = PlaceShape(shapeIndex, shape, x, y, r);
                if (ret)
                    break;
            }
            return ret;
        }

        private bool PlaceShape(int shapeIndex, List<int> shape)
        {
            bool ret = false;
            for (int y = 0; y < 9; ++y)
            {
                for (int x = 0; x < 9; ++x)
                {
                    if (table[y, x] == 0)
                    {
                        ret = PlaceShape (shapeIndex, shape, x, y);
                        if (ret)
                            break;
                    }
                }
                if (ret)
                    break;
            }
            return ret;
        }

        private bool PlaceAllShapes ()
        {
            bool ret = false;
            int shapeIndex = 1;
            foreach (List<int> shape in listOfShapes)
            {
                ret = PlaceShape(shapeIndex, shape);
                shapeIndex++;
            }
            Console.WriteLine($"PlaceShape:{ret}");
            return ret;
        }

        public void Execute()
        {
            InitTable();
            DumpTable();
            bool result = PlaceAllShapes();
            Console.WriteLine($"Execute:{result}");
            DumpTable();
        }
    }
}
