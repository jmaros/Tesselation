#pragma once
//
// Matrix.h
//
#include "Row.h"
#include "BoundingBox.h"

namespace Nessie {
namespace LinAlg {

    // declarations

    template <typename T, typename CT>
    class Matrix {
    public:
        Matrix (CT        numRows,
                CT        numCols,
                const T   & initialValue    = T{});

    // accessors
        inline CT                   MinRow                      ()                                  const;
        inline CT                   MinCol                      ()                                  const;
        inline CT                   RowSize                     ()                                  const;
        inline CT                   ColSize                     (CT                     row = 0)    const;
        inline CT                   RowUpperLimit               ()                                  const;
        inline CT                   ColUpperLimit               ()                                  const;
        inline bool                 IsLegalPosition             (const Position<CT>& pos)           const;
        inline bool                 IsEmptyPosition             (const Position<CT>& pos)           const;

        inline bool                 CanAccomodate               (const Position<CT>     & pos,
                                                                 const Matrix<T, CT>    & mat)      const;

        inline const T              Value                       (const Position<CT>     & pos)      const;
        inline const vector<Row<T>> & GetRows                   ()                                  const;

    // creators
        Matrix<T, CT>               CreateSubMatrix             (const BoundingBox<CT>  & bb)       const;
        Matrix<T, CT>               CreateRotatedBy45Deg        ()                                  const;
        Matrix<T, CT>               CreateTransposed            ()                                  const;
        Matrix<T, CT>               CreateHorizontallyFlipped   ()                                  const;
        Matrix<T, CT>               CreateVerticallyFlipped     ()                                  const;

    // operators
        const Row<T>& operator []       (CT                     index)                              const;

    // modifiers
        inline bool SetData             (Position<CT>           pos,
                                         const T                & newValue);

        inline void SetOutOfBoundValue  (const T                & newValue);
        inline void PushBack            (const Row<T>           & row);

        inline bool Accomodate          (const Position<CT>     & pos,
                                         const Matrix<T, CT>    & mat);

        // operators
        Row<T>& operator [] (CT         index);

    private:
        vector<Row<T>>        m_rows;
        T                     m_initialValue;
        T                     m_outOfBoundValue;
    };


// definitions
    // constructors
    template <typename T,
              typename CT>
    Matrix<T, CT>::Matrix (CT       numRows,
                           CT       numCols,
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
              typename CT>
    inline CT  Matrix<T, CT>::MinRow () const
    {
        return 0;
    }

    template <typename T, typename CT>
    inline CT  Matrix<T, CT>::MinCol () const
    {
        return 0;
    }

    template <typename T, typename CT>
    inline CT  Matrix<T, CT>::RowSize () const
    {
        return m_rows.size();
    }

    template <typename T, typename CT>
    inline CT  Matrix<T, CT>::ColSize (CT row) const
    {
        return (RowSize() > row) ? m_rows[row].Size() : 0;
    }

    template <typename T, typename CT>
    inline const vector<Row<T>> & Matrix<T, CT>::GetRows () const
    {
        return m_rows;
    }

    template <typename T, typename CT>
    inline CT  Matrix<T, CT>::RowUpperLimit () const
    {
        return RowSize() + MinRow();
    }

    template <typename T, typename CT>
    inline CT  Matrix<T, CT>::ColUpperLimit () const
    {
        return ColSize() + MinCol();
    }

    template <typename T, typename CT>
    inline bool Matrix<T, CT>::IsLegalPosition (const Position<CT>  & pos) const
    {
        return (MinRow () <= pos.GetRowIndex() && pos.GetRowIndex() < RowUpperLimit () &&
                MinCol () <= pos.GetColIndex() && pos.GetColIndex() < ColUpperLimit ());
    }

    template <typename T, typename CT>
    inline bool Matrix<T, CT>::IsEmptyPosition (const Position<CT>& pos) const
    {
        bool isEmpty = false;
        if (IsLegalPosition(pos)) {
            isEmpty = (m_initialValue == m_rows[pos.GetRowIndex()].Value(pos.GetColIndex()));
        }
        return isEmpty;
    }

    template <typename T, typename CT>
    inline bool Matrix<T, CT>::CanAccomodate (const Position<CT>    & pos,
                                              const Matrix<T, CT>   & mat) const
    {
        const T     EVI{mat.m_initialValue};
        const T     EVO{ m_initialValue };
        const CT    ru{mat.RowUpperLimit ()};
        const CT    cu{mat.ColUpperLimit ()};

        for (CT row = 0; row < ru; ++row) {
            for (CT col = 0; col < cu; ++col) {
                Position<CT> inPos(row, col);
                Position<CT> ouPos{pos + inPos};
                if (Value(ouPos) != EVO && mat.Value(inPos) != EVI) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T, typename CT>
    inline const T    Matrix<T, CT>::Value (const Position<CT>      & pos) const
    {
        T retVal{ m_outOfBoundValue };
        if (IsLegalPosition(pos)) {
            retVal = m_rows[pos.GetRowIndex()].Value(pos.GetColIndex());
        }
        return retVal;
    }

    template <typename T, typename CT>
    const Row<T>& Matrix<T, CT>::operator [] (CT    rowIndex) const
    {
        return m_rows[rowIndex];
    }

    template <typename T, typename CT>
    Matrix<T, CT> Matrix<T, CT>::CreateSubMatrix (const BoundingBox<CT>     & bb) const
    {
            // Extract non-empty submatrix by using the bounding box of the non-empty elements
        Matrix<T, CT> result(bb.RowNum(), bb.ColNum(), m_initialValue);
        Position<CT> posOffset(bb.GetMinRow(), bb.GetMinCol());
        for (CT dstRowIndex = 0; dstRowIndex < bb.RowNum(); ++dstRowIndex) {
            for (CT dstColIndex = 0; dstColIndex < bb.ColNum(); ++dstColIndex) {
                Position<CT>    dstPos(dstRowIndex, dstColIndex);
                Position<CT>    srcPos{ dstPos + posOffset };
                if (IsLegalPosition(srcPos)) {
                    result.SetData(dstPos, Value(srcPos));
                }
            }
        }
        return result;
    }

    // Extract non-empty submatrix of the rotated matrix by using the bounding box of the non-empty elements
    template <typename T, typename CT>
    Matrix<T, CT> Matrix<T, CT>::CreateRotatedBy45Deg () const
    {
        CT rs = (RowSize() + 1) / 2;
        CT cs = (ColSize() + 1) / 2;
        CT r0 = rs - 1;
        CT c0 = 0;
        CT rotSize = (RowSize() + ColSize() + 1) / 2;
        bool verbose = true;
        if (verbose) {
            cout << " RowSize() = " << RowSize()
                << " ColSize() = " << ColSize()
                << " r0        = " << r0
                << " rs        = " << rs
                << " cs        = " << cs
                << " rotSize   = " << rotSize << endl;
        }
        Matrix<T, CT> rotatedBy45Deg(rotSize, rotSize, m_initialValue);
        BoundingBox<CT> dstBB;
        for (CT srcRowIndex = MinRow(); srcRowIndex < RowUpperLimit(); srcRowIndex += 2) {
            for (CT srcColIndex = MinCol(); srcColIndex < ColUpperLimit(); srcColIndex += 2) {
                Position<CT>    srcPos(srcRowIndex, srcColIndex);
                CT dstRowIndex = (2 * r0 + srcColIndex - srcRowIndex) / 2;
                CT dstColIndex = (2 * c0 + srcColIndex + srcRowIndex) / 2;
                Position<CT>    dstPos(dstRowIndex, dstColIndex);
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

    template <typename T, typename CT>
    Matrix<T, CT> Matrix<T, CT>::CreateTransposed () const
    {
        Matrix<T, CT> transposed(ColSize(), RowSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<CT>    srcPos(rowIndex, colIndex);
                Position<CT>    dstPos(colIndex, rowIndex);
                transposed.SetData(dstPos, Value(srcPos));
            }
        }
        return transposed;
    }

    template <typename T, typename CT>
    Matrix<T, CT> Matrix<T, CT>::CreateHorizontallyFlipped () const
    {
        Matrix<T, CT> horizontallyFlipped(RowSize(), ColSize());
        for (CT rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (CT colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<CT>    srcPos(rowIndex, colIndex);
                Position<CT>    dstPos(RowUpperLimit() - (rowIndex + 1), colIndex);
                horizontallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return horizontallyFlipped;
    }

    template <typename T, typename CT>
    Matrix<T, CT> Matrix<T, CT>::CreateVerticallyFlipped () const
    {
        Matrix<T, CT> verticallyFlipped(RowSize(), ColSize());
        for (auto rowIndex = MinRow(); rowIndex < RowUpperLimit(); ++rowIndex) {
            for (auto colIndex = MinCol(); colIndex < ColUpperLimit(); ++colIndex) {
                Position<CT>    srcPos(rowIndex, colIndex);
                Position<CT>    dstPos(rowIndex, ColUpperLimit() - (colIndex + 1));
                verticallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return verticallyFlipped;
    }

// modifiers
    template <typename T, typename CT>
    inline bool Matrix<T, CT>::SetData  (Position<CT>  pos,
                                         const T       & newValue)
    {
        CT      row{pos.GetRowIndex()};
        CT      col{pos.GetColIndex()};
        bool    bOk{row < RowSize() &&
                    col < operator[](row).Size()};
        if (bOk) {
            bOk = m_rows[row].SetValue(col, newValue);
        } else {
            cout << "Error: SetData out of bound position: " << pos << " value: " << newValue << endl;
        }
        return bOk;
    }

    template <typename T, typename CT>
    inline void Matrix<T, CT>::SetOutOfBoundValue  (const T     & newValue)
    {
        m_outOfBoundValue = newValue;
    }

    template <typename T, typename CT>
    inline void Matrix<T, CT>::PushBack (const Row<T>        & row)
    {
        m_rows.push_back(row);
    }

    template <typename T, typename CT>
    inline bool Matrix<T, CT>::Accomodate (const Position<CT>   & pos,
                                           const Matrix<T, CT>  & mat)
    {
        const T     EVI{ mat.m_initialValue };
        const T     EVO{ m_initialValue };
        const CT    ru{ mat.RowUpperLimit () };
        const CT    cu{ mat.ColUpperLimit () };

        for (CT row = 0u; row < ru; ++row) {
            for (CT col = 0u; col < cu; ++col) {
                Position<CT> inPos(row, col);
                Position<CT> ouPos{ pos + inPos };
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

    template <typename T, typename CT>
    Row<T>& Matrix<T, CT>::operator [] (CT    rowIndex)
    {
        return m_rows[rowIndex];
    }

} // namespace LinAlg
} // namespace Nessie
