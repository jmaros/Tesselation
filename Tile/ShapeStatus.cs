using System;

namespace Tile
{
    internal class ShapeStatus
    {
        public int shapeIndex { get; }
        public Pair<int> position { get; }
        public int rotation { get; }
        public bool valid { get; }

        public ShapeStatus(int index, Pair<int> pos, int rot) { shapeIndex = index; position = pos; rotation = rot; valid = true; }
        public ShapeStatus() { shapeIndex = 0; position = new Pair<int>(0,0); rotation = 0; valid = false; }

        public override string ToString()
        {
            return $"({shapeIndex}, {position}, {rotation}, {valid})";
        }
    }
}
