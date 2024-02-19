#pragma once
//
// Matrix.h
//

#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

namespace Nessie {

    using std::cout;
    using std::endl;
    using std::map;
    using std::ostream;
    using std::size_t;
    using std::initializer_list;
    using std::setw;
//    using std::shared_ptr;
    using std::string;
    using std::stringstream;
    using std::vector;

namespace LinAlg {
    

// declarations

    // Single row of the matrix
    template <typename T>
    class Row {
    public:
    // constructors
        explicit Row (size_t    numCols,
                      const T    & initialValue = T{});

    // accessors
        inline const T      Value       (size_t    numCol)  const;
        inline size_t       Size        ()                  const;
        const vector<T>     & Elements  ()                  const;

    // modifiers
        inline bool SetValue (size_t        col,
                              T             value);

        inline void PushBack (const T       element);
    private:
        vector<T>   m_row;
    };

    template <typename T> 
    class Matrix {
    public:
        Matrix<T>(size_t        numRows,
                  size_t        numCols,
                  const T       & initialValue = T{});

    // accessors
        constexpr size_t MinRow ()                  const;
        constexpr size_t MinCol ()                  const;
        inline size_t RowSize ()                    const;
        inline size_t ColSize ()                    const;
        inline size_t RowUpperLimit ()              const;
        inline size_t ColUpperLimit ()              const;

        inline const T  Value (size_t   rowIndex,
                               size_t   colIndex)   const;

        inline size_t                   Size ()     const;
        inline const vector<Row<T>>     & Rows ()   const;

        Matrix<T> CreateTransposed ()               const;

    // operators
        const Row<T>& operator [] (size_t    index) const;

    // modifiers
        inline bool SetValue(size_t         col,
                             T              value);

        inline void PushBack (const Row<T>  & row);
        
        inline T& DataRef (size_t           rowIndex,
                           size_t           colIndex);

    // operators
        Row<T>& operator [] (size_t         index);

    private:
        vector<Row<T>>        m_rows;
    };


// definitions
    // constructors
    template <typename T>
    Row<T>::Row (size_t     numCols,
                 const T    & initialValue)
        : m_row   (numCols,
                   initialValue)
    {
    }

    template <typename T>
    Matrix<T>::Matrix (size_t    numRows,
                       size_t    numCols,
                       const T   & initialValue)
        : m_rows    (numRows,
                     Row<T> (numCols,
                             initialValue))
    {
    }

    // accessors
    template <typename T>
    inline const T Row<T>::Value (size_t        numCol) const
    {
        return m_row[numCol];
    }

    template <typename T>
    inline size_t Row<T>::Size () const
    {
        return m_row.size();
    }

    template <typename T>
    inline const vector<T> & Row<T>::Elements  () const
    {
        return m_row;
    }

    template <typename T>
    constexpr size_t  Matrix<T>::MinRow () const
    {
        return 0u;
    }

    template <typename T>
    constexpr size_t  Matrix<T>::MinCol () const
    {
        return 0u;
    }

    template <typename T>
    inline size_t  Matrix<T>::RowSize () const
    {
        return m_rows.size();
    }

    template <typename T>
    inline size_t  Matrix<T>::ColSize () const
    {
        return (RowSize() > 0) ? m_rows[0].Size() : 0;
    }

    template <typename T>
    inline const vector<Row<T>>     & Matrix<T>::Rows ()   const
    {
        return m_rows;
    }

    template <typename T>
    inline size_t  Matrix<T>::RowUpperLimit () const
    {
        return RowSize() + MinRow();
    }

    template <typename T>
    inline size_t  Matrix<T>::ColUpperLimit () const
    {
        return ColSize() + MinCol();
    }

    template <typename T>
    inline const T    Matrix<T>::Value (size_t        rowIndex,
                                        size_t        colIndex) const
    {
        return m_rows[rowIndex].Value(colIndex);
    }

    template <typename T>
    const Row<T>& Matrix<T>::operator [] (size_t    rowIndex) const
    {
        return m_rows[rowIndex];
    }

    template <typename T>
    Matrix<T> Matrix<T>::CreateTransposed () const
    {
        static_assert(MinRow() == 0 && MinCol() == 0, "Unexpected configuration!");
        Matrix<T> transposed(ColSize(), RowSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                transposed[colIndex][rowIndex] = Value(rowIndex, colIndex);
            }
        }
        return transposed;
    }

// modifiers
    template <typename T>
    inline bool Row<T>::SetValue (size_t    col,
                                  T         value)
    {
        bool succ = col < Size();
        if (succ) {
            this->m_row[col] = value;
        }
        return succ;
    }

    template <typename T>
    inline void Row<T>::PushBack (const T        element)
    {
        m_row.push_back(element);
    }
    template <typename T>
    inline void Matrix<T>::PushBack (const Row<T>        & row)
    {
        m_rows.push_back(row);
    }

    template <typename T>
    Row<T>& Matrix<T>::operator [] (size_t    rowIndex)
    {
        return m_rows[rowIndex];
    }

} // namespace LinAlg
} // namespace Nessie
