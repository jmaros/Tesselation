//using System;
using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

namespace Tile
{
    internal class HashUtility
    {
        static long[] prims = {
            1087,     2131,   3083,   4073,   5099,   6143,   7001,   9311,  99371,  193939,
          199933,   319993, 331999, 391939, 393919, 919393, 933199, 939193, 939391,  993319,
          999331
        };
        public static long GenerateHash(List<Pair<int>> listPair)
        {
            int numPrims = prims.Length;
            long hash = prims[0];
            for (int i = 0; i < listPair.Count; ++i)
            {
                hash += (listPair[i].Value1 * prims[((2 * i) + 0) % numPrims]) +
                        (listPair[i].Value2 * prims[((2 * i) + 1) % numPrims]);
            }
            return hash;
        }
    }
}
