# Tile

There is a table and there are 8 shapes, each of whitch can be rotated and mirrored.
The first 12 positions on the table (from -1 to -12) are for the monthes (from Jan. do Dec.),
and below that the numbers of the month. There is a tesselation (at least one) for each valid
month - day pair, where only that month and day remains visible, and all the others are covered
by these shapes. The aim of the program is to find such arrangement of these shapes for any valid
month - day pair.
```
Table   =  -1 -2 -3 -4 -5 -7
           -7 -8 -9-10-11-12
            1  2  3  4  5  6  7
            8  9 10 11 12 13 14
           15 16 17 18 19 20 21
           22 23 24 25 26 27 28
           29 30 31
```
```
Shape 1 =   1     1
            1  1  1
```
```
Shape 2 =   1  1  1
            1  1  1
```
```
Shape 3 =   1  1  1
            1  1
```
```
Shape 4 =   1
            1
            1  1  1
```
```
Shape 5 =   1  1
               1
               1  1
```
```
Shape 6 =   1  1  1  1
               1
```
```
Shape 7 =   1  1  1
                  1  1
```
```
Shape 8 =   1  1  1  1
            1
```
An example of the output of test-run:
```
2024-03-06 13:30:15
TableShape =
          0010001
          0000001
          0000010
          0000000
          0000000
          0000000
          0001111
Tile Date = 2024 March 6
Solution =
          AA BBB
          AGGBBB
          AAGGG F
          DDDCCFF
          DEECCCF
          DEHHHHF
          EEH


 Elapsed Time: 1.58236s
```
