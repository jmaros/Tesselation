// See https://aka.ms/new-console-template for more information
using Tile;

Console.WriteLine("Hello, Hexa Calendar!");
Tile6 tile6 = new Tile6();
// single thread:
// tile6.ExecuteAllShape(11,8,6); // int month, int mday, int wday (1,1,1) => jan, 1, Sunday - (12,31,7) => dec, 31, Saturday
// multi thread:
tile6.Execute(11,8,6); // int month, int mday, int wday (1,1,1) => jan, 1, Sunday - (12,31,7) => dec, 31, Saturday
Console.WriteLine("Done");
