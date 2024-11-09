using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tile
{
    internal class ShapePossiblePosition : Tile6Base
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

        public bool Calculate ()
        {
            InitTableBase();
            return true;
        }

        public void ExecuteOneShape()
        {
            InitTableBase();
            //DumpTable();
            bool result = PlaceOneShape(shapeIndex, ref shapePositionList, ref territoryList);
            Console.WriteLine($"Execute:{result}");
            //long elapsedTotalLocal = elapsedTotal + timerTest.Check();
            //Console.WriteLine($"Timer total: {elapsedTotalLocal}");

            //DumpTable();
        }
    }
}
