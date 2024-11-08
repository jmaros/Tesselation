using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Tile
{
    internal class TimerTest
    {
        Stopwatch stopwatch = new Stopwatch();

        public TimerTest()
        {
            stopwatch = Stopwatch.StartNew ();
        }

        public long Check()
        {
            stopwatch.Stop();

            long elapsedMilliseconds = stopwatch.ElapsedMilliseconds;
            stopwatch.Restart ();
            return elapsedMilliseconds;
        }
    }
}
