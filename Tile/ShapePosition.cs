using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class ShapePosition
    {
        public int orientation { get; }
        public Pair<int> position { get; }
        public ShapePosition() { orientation = 0; position = new Pair<int>(0, 0); }
        public ShapePosition(int ori, Pair<int> pos) { orientation = ori; position = pos; }
    }
}
