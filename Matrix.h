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
                const T       & initialValue    = T{});

    // accessors
        inline size_t               MinRow                      ()                              const;
        inline size_t               MinCol                      ()                              const;
        inline size_t               RowSize                     ()                              const;
        inline size_t               ColSize                     (size_t             row = 0)    const;
        inline size_t               RowUpperLimit               ()                              const;
        inline size_t               ColUpperLimit               ()                              const;
        inline bool                 IsLegalPosition             (const Position     & pos)      const;

        inline bool                 CanAccomodate               (const Position     & pos,
                                                                 const Matrix<T>    & mat)      const;

        inline const T              Value                       (const Position     & pos)      const;
        inline const vector<Row<T>> & GetRows                   ()                              const;

    // creators
        Matrix<T>                   CreateTransposed            ()                              const;
        Matrix<T>                   CreateHorizontallyFlipped   ()                              const;
        Matrix<T>                   CreateVerticallyFlipped     ()                              const;

    // operators
        const Row<T>& operator []       (size_t    index) const;

    // modifiers
        inline bool SetData             (Position      pos,
                                         const T       & newValue);

        inline void SetOutOfBoundValue  (const T       & newValue);
        inline void PushBack            (const Row<T>  & row);

        inline bool Accomodate          (const Position     & pos,
                                         const Matrix<T>    & mat);

        // operators
        Row<T>& operator [] (size_t         index);

    private:
        vector<Row<T>>        m_rows;
        T                     m_outOfBoundValue;
    };


// definitions
    // constructors
    template <typename T>
    Matrix<T>::Matrix (size_t    numRows,
                       size_t    numCols,
                       const T   & initialValue)
     : m_rows               ()
     , m_outOfBoundValue    ()
    {
        Row<T> defRow(numCols, initialValue);
        m_rows.resize(numRows, defRow);
    }

    // accessors
    template <typename T>
    inline size_t  Matrix<T>::MinRow () const
    {
        return 0u;
    }

    template <typename T>
    inline size_t  Matrix<T>::MinCol () const
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
    inline const vector<Row<T>> & Matrix<T>::GetRows () const
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
    inline bool Matrix<T>::IsLegalPosition (const Position      & pos) const
    {
        return (MinRow () <= pos.GetRowIndex() && pos.GetRowIndex() < RowUpperLimit () &&
                MinCol () <= pos.GetColIndex() && pos.GetColIndex() < ColUpperLimit ());
    }

    template <typename T>
    inline bool Matrix<T>::CanAccomodate (const Position     & pos,
                                          const Matrix<T>    & mat) const
    {
        const T         EV{};
        const size_t    ru{mat.RowUpperLimit ()};
        const size_t    cu{mat.ColUpperLimit ()};

        for (size_t row = 0u; row < ru; ++row) {
            for (size_t col = 0u; col < cu; ++col) {
                Position inPos(row, col);
                Position ouPos{pos + inPos};
                if (Value(ouPos) != EV && mat.Value(inPos) != EV) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T>
    inline const T    Matrix<T>::Value (const Position      & pos) const
    {
        T retVal{ m_outOfBoundValue };
        if (IsLegalPosition(pos)) {
            retVal = m_rows[pos.GetRowIndex()].Value(pos.GetColIndex());
        }
        return retVal;
    }

    template <typename T>
    const Row<T>& Matrix<T>::operator [] (size_t    rowIndex) const
    {
        return m_rows[rowIndex];
    }

    template <typename T>
    Matrix<T> Matrix<T>::CreateTransposed () const
    {
        Matrix<T> transposed(ColSize(), RowSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position    srcPos(rowIndex, colIndex);
                Position    dstPos(colIndex, rowIndex);
                transposed.SetData(dstPos, Value(srcPos));
            }
        }
        return transposed;
    }

    template <typename T>
    Matrix<T> Matrix<T>::CreateHorizontallyFlipped () const
    {
        Matrix<T> horizontallyFlipped(RowSize(), ColSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position    srcPos(rowIndex, colIndex);
                Position    dstPos(RowUpperLimit() - (rowIndex + 1), colIndex);
                horizontallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return horizontallyFlipped;
    }

    template <typename T>
    Matrix<T> Matrix<T>::CreateVerticallyFlipped () const
    {
        Matrix<T> verticallyFlipped(RowSize(), ColSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position    srcPos(rowIndex, colIndex);
                Position    dstPos(rowIndex, ColUpperLimit() - (colIndex + 1));
                verticallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return verticallyFlipped;
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
    inline void Matrix<T>::SetOutOfBoundValue  (const T     & newValue)
    {
        m_outOfBoundValue = newValue;
    }

    template <typename T>
    inline void Matrix<T>::PushBack (const Row<T>        & row)
    {
        m_rows.push_back(row);
    }

    template <typename T>
    inline bool Matrix<T>::Accomodate (const Position     & pos,
                                       const Matrix<T>    & mat)
    {
        const T         EV{};
        const size_t    ru{mat.RowUpperLimit ()};
        const size_t    cu{mat.ColUpperLimit ()};

        for (size_t row = 0u; row < ru; ++row) {
            for (size_t col = 0u; col < cu; ++col) {
                Position inPos(row, col);
                Position ouPos{pos + inPos};
                if (Value(ouPos) != EV && mat.Value(inPos) != EV) {
                    cout << "Unexpexted collision at " << pos << endl;
                    return false;
                }
                if (Value(ouPos) == EV && mat.Value(inPos) != EV) {
                    bool bSucc = SetData(ouPos, mat.Value(inPos));
                    if (!bSucc) {
                        cout << "Failed to write to " << ouPos
                             << " = " << pos << " + " << inPos << endl;
                    }
                }
            }
        }
        return true;
    }

    template <typename T>
    Row<T>& Matrix<T>::operator [] (size_t    rowIndex)
    {
        return m_rows[rowIndex];
    }

} // namespace LinAlg
} // namespace Nessie
