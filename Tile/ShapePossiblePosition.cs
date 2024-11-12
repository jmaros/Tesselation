using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class ShapePossiblePosition
    {
        public int shapeIndex { get; }
        public List<ShapePosition> shapePositionList;
        public List<List<Pair<int>>> territoryList;
        public ShapePossiblePosition(int shapeIndex)
        {
            shapePositionList = new List<ShapePosition>();
            territoryList = new List<List<Pair<int>>>();
            this.shapeIndex = shapeIndex; 
        }

    }
}
