#pragma once
//
// ResizableMatrix.hpp
//
#include<vector>
#include<iostream>
#include<iomanip>
using namespace std;

template <class T>
class Matrix {
public:
    Matrix() = default;

    bool AddItem (unsigned		r,
				  unsigned 		c,
				  T 			value)
    {
        if (r >= Rows_count) {
            Rows.resize(r + 1);
            Rows_count = r + 1;
        } else {
            Rows.resize(Rows_count);
        }
        if (c >= Columns_Count ) {
            for (std::vector<T>& row : Rows) {
                row.resize(c + 1);
            }
            Columns_Count = c + 1;
        } else {
            for (std::vector<T>& row : Rows) {
                row.resize(Columns_Count);
            }
        }
        if (r < Rows.size()) {
            if (c < static_cast<std::vector<T>>(Rows.at(r)).size()) {
                (Rows.at(r)).at(c) = value;
            } else {
                cout << Rows.at(r).size() << " greater than " << c << endl;
            }
		} else {
            cout << "ERROR" << endl;
		}
        return true;
    }

    void Show()
    {
        std::cout << "*****************"<<std::endl;
        for (std::vector<T> r : Rows) {
            for (auto& c : r) {
                std::cout << " " <<setw(5)<< c;
			}
            std::cout << std::endl;
        }
        std::cout << "*****************" << std::endl;
    }

    void Show(size_t n)
    {
        std::cout << "*****************" << std::endl;
        for (std::vector<T> r : Rows) {
            for (auto& c : r) {
                std::cout << " " << setw(n) << c;
			}
            std::cout << std::endl;
        }
        std::cout << "*****************" << std::endl;
    }

protected:
    std::vector<std::vector<T>> Rows;
    unsigned					Rows_count;
    unsigned 					Columns_Count;
};
