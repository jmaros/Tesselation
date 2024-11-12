using System;
using System.Collections;
using System.Collections.Generic;

namespace Tile
{
    internal class HashUtility
    {
        static long[] prims = {1087, 2131, 3083, 4073, 5099, 6143, 7001, 9311,  99371,
            193939, 199933, 319993, 331999, 391939, 393919, 919393, 933199, 939193,
            939391, 993319, 999331 };
        public static long GenerateHash(List<Pair<int>> listPair) 
        {
            int numPrims = prims.Length;
            long hash = prims[0];
            for (int i = 0; i < listPair.Count; ++i)
            {
                hash += listPair[i].Value1 * prims[(2 * i) % numPrims] + listPair[i].Value2 * prims[((2 * i) + 1) % numPrims];
            }
            return hash;
        }

        public static bool TestGenerateHashListPair()
        {
            List<Pair<int>> listPair1 = new List<Pair<int>>();
            List<Pair<int>> listPair2 = new List<Pair<int>>();

            listPair1.Add(new Pair<int>(2, 1));
            listPair1.Add(new Pair<int>(3, 1));
            listPair1.Add(new Pair<int>(4, 1));
            listPair1.Add(new Pair<int>(5, 1));
            listPair1.Add(new Pair<int>(6, 1));

            listPair2.Add(new Pair<int>(6, 1));
            listPair2.Add(new Pair<int>(5, 1));
            listPair2.Add(new Pair<int>(4, 1));
            listPair2.Add(new Pair<int>(3, 1));
            listPair2.Add(new Pair<int>(2, 1));

            //listPair2.Sort(Pair<int>CompareTo);
            listPair2.Sort((pair1, pair2) =>
            {
                int firstComparison = pair1.Value1.CompareTo(pair2.Value1);
                return firstComparison != 0 ? firstComparison : pair1.Value2.CompareTo(pair2.Value2);
            });

            long hash1 = GenerateHash(listPair1);
            long hash2 = GenerateHash(listPair2);

            return hash1 == hash2;

        }
    }
}
