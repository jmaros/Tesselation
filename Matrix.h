#pragma once
//
// Matrix.h
//
#include "Row.h"

namespace Nessie {
namespace LinAlg {

// declarations

    template <typename T> 
    class Matrix {
    public:
        Matrix (size_t        numRows,
                size_t        numCols,
                const T       & initialValue = T{});

    // accessors
        constexpr size_t MinRow     ()                      const;
        constexpr size_t MinCol     ()                      const;
        inline size_t RowSize       ()                      const;
        inline size_t ColSize       (size_t row = 0)        const;
        inline size_t RowUpperLimit ()                      const;
        inline size_t ColUpperLimit ()                      const;
        inline const T  Value (const Position   & pos)      const;
        inline const vector<Row<T>> & Rows              ()  const;
        Matrix<T>                   CreateTransposed    ()  const;

    // operators
        const Row<T>& operator [] (size_t    index) const;

    // modifiers
        inline bool SetData     (Position      pos,
                                 const T       & newValue);

        inline void PushBack    (const Row<T>  & row);
        
    // operators
        Row<T>& operator [] (size_t         index);

    private:
        vector<Row<T>>        m_rows;
    };


// definitions
    // constructors
    template <typename T>
    Matrix<T>::Matrix (size_t    numRows,
                       size_t    numCols,
                       const T   & initialValue)
        : m_rows      ()
    {
        Row<T> defRow(numCols, initialValue);
        m_rows.resize(numRows, defRow);
    }

    // accessors
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
    inline size_t  Matrix<T>::ColSize (size_t row) const
    {
        return (RowSize() > row) ? m_rows[row].Size() : 0;
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
    inline const T    Matrix<T>::Value (const Position      & pos) const
    {
        return m_rows[pos.GetRowIndex()].Value(pos.GetColIndex());
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
    inline bool Matrix<T>::SetData  (Position      pos,
                                     const T       & newValue)
    {
        size_t row = pos.GetRowIndex();
        size_t col = pos.GetColIndex();
        bool succ = row < RowSize() &&
                    col < operator[](row).Size();
        if (succ) {
            succ = m_rows[row].SetValue(col, newValue);
        }
        return succ;
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
