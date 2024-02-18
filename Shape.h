#pragma once
//
// Shape.h
//
#include "Matrix.h"

#include <string>

namespace geom {
// declarations
    using std::cout;
    using std::endl;
    using std::initializer_list;
	using std::setw;
	using std::string;

    template <typename T>
    class Shape {
    public:
    // constructors
        Shape (size_t	numCols,
               size_t	numRows);

        Shape (initializer_list< initializer_list<T> >	ili);

    // accessors
        size_t NumCols ()	const;
        size_t NumRows ()	const;

        inline const T	Value (size_t		rowIndex,
                               size_t		colIndex) const;

        // operators
        const LinAlg::Row<T>& operator [] (size_t	index) const;

    // modifiers
        inline T DataRef (size_t		rowIndex,
                          size_t		colIndex);

        // operators
        LinAlg::Row<T>& operator [] (size_t	index);

    private:
    // data members
        LinAlg::Matrix<T>	m_matrix;
    };

// definitions
    // constructors

    template <typename T>
    Shape<T>::Shape (size_t	numCols,
                     size_t	numRows)
     : m_matrix	(numCols,
                 numRows)
    {
    }

#if defined (VERBOSE)
	namespace {
		int shapeNumber{};
	}
#endif

    template <typename T>
    Shape<T>::Shape (initializer_list<initializer_list<T>>	ili)
        : m_matrix (0,0)
    {
		size_t roi{};
		for (auto it1 = ili.begin(); it1 != ili.end(); ++roi, ++it1) {
			m_matrix.PushBack(LinAlg::Row<T>(0));
			size_t coi{};
			for (auto it2 = it1->begin(); it2 != it1->end(); ++coi, ++it2) {
				m_matrix[roi].PushBack(*it2);
			}
		}
		vector<vector<T>> init;
        size_t r{};
        for (auto it1 = ili.begin(); it1 != ili.end(); ++r, ++it1) {
            init.push_back(vector<T>{});         
            size_t c{};
            for (auto it2 = it1->begin(); it2 != it1->end(); ++c, ++it2) {
                init[r].push_back(*it2);
            }
        }
#if defined (VERBOSE)
        cout << "Shape " << shapeNumber++;
        string prep{ " = " };
		size_t ps0{prep.size()};
        for (auto row : init) {
            cout << prep;
			if (prep.size() == ps0) {
				prep = "          ";
			}
            for (auto element : row) {
				if (element) {
					cout << setw(3) << element;
				} else {
					cout << setw(3) << " ";
				}
            }
            cout << endl;
        }
        cout << endl;
#endif
    }

    // accessors

    template <typename T>
    size_t Shape<T>::NumRows ()	const
    {
        return m_matrix.RowSize();
    }

    template <typename T>
    size_t Shape<T>::NumCols ()	const
    {
        return m_matrix.ColSize();
    }

    template <typename T>
    inline const T	Shape<T>::Value (size_t		rowIndex,
                                     size_t		colIndex) const
    {
        return m_matrix.Value(rowIndex,
                              colIndex);
    }

    // operators
    template <typename T>
    const LinAlg::Row<T>& Shape<T>::operator [] (size_t	index) const
    {
        return m_matrix[index];
    }

    // modifiers
        template <typename T>
        inline T Shape<T>::DataRef (size_t		rowIndex,
                                     size_t		colIndex)
        {
            return m_matrix.DataRef(rowIndex, colIndex);
        }
    // operators
    template <typename T>
    LinAlg::Row<T>& Shape<T>::operator [] (size_t	index)
    {
        return m_matrix[index];
    }
} // namespace geom
