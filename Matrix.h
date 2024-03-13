#pragma once
//
// Matrix.h
//
#include "Row.h"
#include "BoundingBox.h"

namespace Nessie {
namespace LinAlg {

    // declarations

    template <typename T, typename ST>
    class Matrix {
    public:
        Matrix (ST        numRows,
                ST        numCols,
                const T   & initialValue    = T{});

    // accessors
        inline ST                   MinRow                      ()                                  const;
        inline ST                   MinCol                      ()                                  const;
        inline ST                   RowSize                     ()                                  const;
        inline ST                   ColSize                     (ST                     row = 0)    const;
        inline ST                   RowUpperLimit               ()                                  const;
        inline ST                   ColUpperLimit               ()                                  const;
        inline bool                 IsLegalPosition             (const Position<ST>& pos)           const;
        inline bool                 IsEmptyPosition             (const Position<ST>& pos)           const;

        inline bool                 CanAccomodate               (const Position<ST>     & pos,
                                                                 const Matrix<T, ST>    & mat)      const;

        inline const T              Value                       (const Position<ST>     & pos)      const;
        inline const vector<Row<T>> & GetRows                   ()                                  const;

    // creators
        Matrix<T, ST>               CreateSubMatrix             (const BoundingBox<ST>  & bb)       const;
        Matrix<T, ST>               CreateRotatedBy45Deg        ()                                  const;
        Matrix<T, ST>               CreateTransposed            ()                                  const;
        Matrix<T, ST>               CreateHorizontallyFlipped   ()                                  const;
        Matrix<T, ST>               CreateVerticallyFlipped     ()                                  const;

    // operators
        const Row<T>& operator []       (ST                     index)                              const;

    // modifiers
        inline bool SetData             (Position<ST>           pos,
                                         const T                & newValue);

        inline void SetOutOfBoundValue  (const T                & newValue);
        inline void PushBack            (const Row<T>           & row);

        inline bool Accomodate          (const Position<ST>     & pos,
                                         const Matrix<T, ST>    & mat);

        // operators
        Row<T>& operator [] (ST         index);

    private:
        vector<Row<T>>        m_rows;
        T                     m_initialValue;
        T                     m_outOfBoundValue;
    };


// definitions
    // constructors
    template <typename T,
              typename ST>
    Matrix<T, ST>::Matrix (ST       numRows,
                           ST       numCols,
                           const T   & initialValue)
     : m_rows               ()
     , m_initialValue       (initialValue)
     , m_outOfBoundValue    ()
    {
        Row<T> defRow(numCols, initialValue);
        m_rows.resize(numRows, defRow);
    }

    // accessors
    template <typename T,
              typename ST>
    inline ST  Matrix<T, ST>::MinRow () const
    {
        return 0;
    }

    template <typename T, typename ST>
    inline ST  Matrix<T, ST>::MinCol () const
    {
        return 0;
    }

    template <typename T, typename ST>
    inline ST  Matrix<T, ST>::RowSize () const
    {
        return m_rows.size();
    }

    template <typename T, typename ST>
    inline ST  Matrix<T, ST>::ColSize (ST row) const
    {
        return (RowSize() > row) ? m_rows[row].Size() : 0;
    }

    template <typename T, typename ST>
    inline const vector<Row<T>> & Matrix<T, ST>::GetRows () const
    {
        return m_rows;
    }

    template <typename T, typename ST>
    inline ST  Matrix<T, ST>::RowUpperLimit () const
    {
        return RowSize() + MinRow();
    }

    template <typename T, typename ST>
    inline ST  Matrix<T, ST>::ColUpperLimit () const
    {
        return ColSize() + MinCol();
    }

    template <typename T, typename ST>
    inline bool Matrix<T, ST>::IsLegalPosition (const Position<ST>  & pos) const
    {
        return (MinRow () <= pos.GetRowIndex() && pos.GetRowIndex() < RowUpperLimit () &&
                MinCol () <= pos.GetColIndex() && pos.GetColIndex() < ColUpperLimit ());
    }

    template <typename T, typename ST>
    inline bool Matrix<T, ST>::IsEmptyPosition (const Position<ST>& pos) const
    {
        bool isEmpty = false;
        if (IsLegalPosition(pos)) {
            isEmpty = (m_initialValue == m_rows[pos.GetRowIndex()].Value(pos.GetColIndex()));
        }
        return isEmpty;
    }

    template <typename T, typename ST>
    inline bool Matrix<T, ST>::CanAccomodate (const Position<ST>    & pos,
                                              const Matrix<T, ST>   & mat) const
    {
        const T     EVI{mat.m_initialValue};
        const T     EVO{ m_initialValue };
        const ST    ru{mat.RowUpperLimit ()};
        const ST    cu{mat.ColUpperLimit ()};

        for (ST row = 0; row < ru; ++row) {
            for (ST col = 0; col < cu; ++col) {
                Position<ST> inPos(row, col);
                Position<ST> ouPos{pos + inPos};
                if (Value(ouPos) != EVO && mat.Value(inPos) != EVI) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T, typename ST>
    inline const T    Matrix<T, ST>::Value (const Position<ST>      & pos) const
    {
        T retVal{ m_outOfBoundValue };
        if (IsLegalPosition(pos)) {
            retVal = m_rows[pos.GetRowIndex()].Value(pos.GetColIndex());
        }
        return retVal;
    }

    template <typename T, typename ST>
    const Row<T>& Matrix<T, ST>::operator [] (ST    rowIndex) const
    {
        return m_rows[rowIndex];
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateSubMatrix (const BoundingBox<ST>     & bb) const
    {
            // Extract non-empty submatrix by using the bounding box of the non-empty elements
        Matrix<T, ST> result(bb.RowNum(), bb.ColNum(), m_initialValue);
        Position<ST> posOffset(bb.GetMinRow(), bb.GetMinCol());
        for (ST dstRowIndex = 0; dstRowIndex < bb.RowNum(); ++dstRowIndex) {
            for (ST dstColIndex = 0; dstColIndex < bb.ColNum(); ++dstColIndex) {
                Position<ST>    dstPos(dstRowIndex, dstColIndex);
                Position<ST>    srcPos{ dstPos + posOffset };
                if (IsLegalPosition(srcPos)) {
                    result.SetData(dstPos, Value(srcPos));
                }
            }
        }
        return result;
    }

    // Extract non-empty submatrix of the rotated matrix by using the bounding box of the non-empty elements
    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateRotatedBy45Deg () const
    {
        ST rs = (RowSize() + 1) / 2;
        ST cs = (ColSize() + 1) / 2;
        ST r0 = rs - 1;
        ST c0 = 0;
        ST rotSize = (RowSize() + ColSize() + 1) / 2;
        bool verbose = true;
        if (verbose) {
            cout << " RowSize() = " << RowSize()
                << " ColSize() = " << ColSize()
                << " r0        = " << r0
                << " rs        = " << rs
                << " cs        = " << cs
                << " rotSize   = " << rotSize << endl;
        }
        Matrix<T, ST> rotatedBy45Deg(rotSize, rotSize, m_initialValue);
        BoundingBox<ST> dstBB;
        for (ST srcRowIndex = MinRow(); srcRowIndex < RowUpperLimit(); srcRowIndex += 2) {
            for (ST srcColIndex = MinCol(); srcColIndex < ColUpperLimit(); srcColIndex += 2) {
                Position<ST>    srcPos(srcRowIndex, srcColIndex);
                ST dstRowIndex = (2 * r0 + srcColIndex - srcRowIndex) / 2;
                ST dstColIndex = (2 * c0 + srcColIndex + srcRowIndex) / 2;
                Position<ST>    dstPos(dstRowIndex, dstColIndex);
                if (verbose) {
                    cout << " src-" << srcPos
                    << " dst-" << dstPos << endl;
                }
                if (!IsEmptyPosition(srcPos)) {
                    dstBB.AddPosition(dstPos);
                    bool isRot45Succ = rotatedBy45Deg.SetData(dstPos, Value(srcPos));
                    if (!isRot45Succ) {
                        cout << "Rotate failed!" << endl;
                        return rotatedBy45Deg;
                    }
                }
            }
        }
        return rotatedBy45Deg.CreateSubMatrix(dstBB);
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateTransposed () const
    {
        Matrix<T, ST> transposed(ColSize(), RowSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<ST>    srcPos(rowIndex, colIndex);
                Position<ST>    dstPos(colIndex, rowIndex);
                transposed.SetData(dstPos, Value(srcPos));
            }
        }
        return transposed;
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateHorizontallyFlipped () const
    {
        Matrix<T, ST> horizontallyFlipped(RowSize(), ColSize());
        for (ST rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (ST colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<ST>    srcPos(rowIndex, colIndex);
                Position<ST>    dstPos(RowUpperLimit() - (rowIndex + 1), colIndex);
                horizontallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return horizontallyFlipped;
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateVerticallyFlipped () const
    {
        Matrix<T, ST> verticallyFlipped(RowSize(), ColSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<ST>    srcPos(rowIndex, colIndex);
                Position<ST>    dstPos(rowIndex, ColUpperLimit() - (colIndex + 1));
                verticallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return verticallyFlipped;
    }

// modifiers
    template <typename T, typename ST>
    inline bool Matrix<T, ST>::SetData  (Position<ST>  pos,
                                         const T       & newValue)
    {
        ST      row{pos.GetRowIndex()};
        ST      col{pos.GetColIndex()};
        bool    bOk{row < RowSize() &&
                    col < operator[](row).Size()};
        if (bOk) {
            bOk = m_rows[row].SetValue(col, newValue);
        } else {
            cout << "Error: SetData out of bound position: " << pos << " value: " << newValue << endl;
        }
        return bOk;
    }

    template <typename T, typename ST>
    inline void Matrix<T, ST>::SetOutOfBoundValue  (const T     & newValue)
    {
        m_outOfBoundValue = newValue;
    }

    template <typename T, typename ST>
    inline void Matrix<T, ST>::PushBack (const Row<T>        & row)
    {
        m_rows.push_back(row);
    }

    template <typename T, typename ST>
    inline bool Matrix<T, ST>::Accomodate (const Position<ST>   & pos,
                                           const Matrix<T, ST>  & mat)
    {
        const T     EVI{ mat.m_initialValue };
        const T     EVO{ m_initialValue };
        const ST    ru{ mat.RowUpperLimit () };
        const ST    cu{ mat.ColUpperLimit () };

        for (ST row = 0u; row < ru; ++row) {
            for (ST col = 0u; col < cu; ++col) {
                Position<ST> inPos(row, col);
                Position<ST> ouPos{ pos + inPos };
                if (Value(ouPos) != EVO && mat.Value(inPos) != EVI) {
                    cout << "Unexpexted collision at " << pos << endl;
                    return false;
                }
                if (Value(ouPos) == EVO && mat.Value(inPos) != EVI) {
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

    template <typename T, typename ST>
    Row<T>& Matrix<T, ST>::operator [] (ST    rowIndex)
    {
        return m_rows[rowIndex];
    }

} // namespace LinAlg
} // namespace Nessie
