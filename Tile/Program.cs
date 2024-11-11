// See https://aka.ms/new-console-template for more information

using System;

using Tile;

internal class Program {
    private static void Main (string[] args)
    {
        Console.WriteLine("Hello, Hexa Calendar!");
        new Tile6().Execute();
        Console.WriteLine("Done");
    }
}