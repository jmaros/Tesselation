using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class Pair<T>
    {
        public T Value1 { get; }
        public T Value2 { get; }

        public Pair(T value1, T value2)
        {
            Value1 = value1;
            Value2 = value2;
        }

        public override string ToString()
        {
            return $"({Value1}, {Value2})";
        }
    }
}
