#pragma once
//
// Matrix.h
//
#include "BoundingBox.h"
#include "P3Vector.h"
#include "Row.h"

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
        inline ST                   RowSize                     ()                                  const;
        inline ST                   ColSize                     (ST                     row = 0)    const;
        inline bool                 IsLegalPosition             (const Position<ST>     & pos)      const;
        inline bool                 IsEmptyPosition             (const Position<ST>     & pos)      const;

        inline bool                 CanAccomodate               (const Position<ST>     & pos,
                                                                 const Matrix<T, ST>    & mat)      const;

        inline const T              Value                       (const Position<ST>     & pos)      const;
        inline const vector<Row<T>> & GetRows                   ()                                  const;

    // creators                                                                       
        Matrix<T, ST>               CreateSubMatrix             (const BoundingBox<ST>  & bb)       const;
        Matrix<T, ST>               CreateRotatedBy60Deg        ()                                  const;
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
    inline bool Matrix<T, ST>::IsLegalPosition (const Position<ST>  & pos) const
    {
        return (0 <= pos.GetRowIndex() && pos.GetRowIndex() < RowSize () &&
                0 <= pos.GetColIndex() && pos.GetColIndex() < ColSize ());
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
        const ST    ru{mat.RowSize ()};
        const ST    cu{mat.ColSize ()};

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
        ST rLow = 0;
        ST cLow = 0;
        // Extract non-empty submatrix by using the bounding box of the non-empty elements
        Matrix<T, ST> result(bb.RowNum() + rLow, bb.ColNum() + cLow, m_initialValue);
        Position<ST> posOffset(bb.GetMinRow(), bb.GetMinCol());
        for (ST dstRowIndex = 0; dstRowIndex < bb.RowNum(); ++dstRowIndex) {
            for (ST dstColIndex = 0; dstColIndex < bb.ColNum(); ++dstColIndex) {
                Position<ST>    dstPos(dstRowIndex + rLow, dstColIndex + cLow);
                Position<ST>    srcPos{ dstPos + posOffset };
                if (IsLegalPosition(srcPos)) {
                    result.SetData(dstPos, Value(srcPos));
                }
            }
        }
        return result;
    }

    // Extract non-empty submatrix of the rotated matrix
    // by using the bounding box of the non-empty elements
    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateRotatedBy60Deg () const
    {
        ST rotSize = (RowSize() + (RowSize() % 2) + ColSize() + (RowSize() % 2));
        bool verbose = true;
        if (verbose) {
            cout << " RowSize() = " << RowSize()
                 << " ColSize() = " << ColSize()
                 << " rotSize = " << rotSize << endl;
        }
        Matrix<T, ST> rotatedBy60Deg(2 * rotSize, 2 * rotSize, m_initialValue);
        BoundingBox<ST> dstBB;
        double          xFactor{ cos(Ang60InRad) };
        double          yFactor{ sin(Ang60InRad) };
        for (ST srcRowIndex = 0; srcRowIndex < RowSize(); ++srcRowIndex) {
            for (ST srcColIndex = 0; srcColIndex < ColSize(); ++srcColIndex) {
                Position<ST>    srcPos(srcRowIndex, srcColIndex);
                if (!IsEmptyPosition(srcPos)) {
                    P3Vector        originalPoint{ srcColIndex * xFactor, srcRowIndex * yFactor };
                    P3Vector        rotatedPoint { originalPoint.Rotate2D(Ang60InRad) };
                    ST dstRowIndex = ST((rotatedPoint.Y() / yFactor) + rotSize + 0.5);
                    ST dstColIndex = ST((rotatedPoint.X() / xFactor) + rotSize + 0.5);
                    Position<ST>    dstPos(dstRowIndex, dstColIndex);
                    if (verbose) {
                        cout << " originalPoint-{" << originalPoint.X() << ", " << originalPoint.Y() << "}"
                             << " rotatedPoint-{"  << rotatedPoint.X()  << ", " << rotatedPoint.Y()  << "}"
                             << endl;
                        cout << " src-" << srcPos
                             << " dst-" << dstPos
                             << endl;
                    }
                    dstBB.AddPosition(dstPos);
                    bool isRot60Succ = rotatedBy60Deg.SetData(dstPos, Value(srcPos));
                    if (!isRot60Succ) {
                        cout << "Rotate failed!" << endl;
                        return rotatedBy60Deg;
                    }
                }
            }
        }
        return rotatedBy60Deg.CreateSubMatrix(dstBB);
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateTransposed () const
    {
        Matrix<T, ST> transposed(ColSize(), RowSize());
        for (ST rowIndex = 0; rowIndex < RowSize(); ++rowIndex) {
            for (ST colIndex = 0; colIndex < ColSize(); ++colIndex) {
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
        for (ST rowIndex = 0; rowIndex < RowSize(); ++rowIndex) {
            for (ST colIndex = 0; colIndex < ColSize(); ++colIndex) {
                Position<ST>    srcPos(rowIndex, colIndex);
                Position<ST>    dstPos(RowSize() - (rowIndex + 1), colIndex);
                horizontallyFlipped.SetData(dstPos, Value(srcPos));
            }
        }
        return horizontallyFlipped;
    }

    template <typename T, typename ST>
    Matrix<T, ST> Matrix<T, ST>::CreateVerticallyFlipped () const
    {
        Matrix<T, ST> verticallyFlipped(RowSize(), ColSize());
        for (ST rowIndex = 0; rowIndex < RowSize(); ++rowIndex) {
            for (ST colIndex = 0; colIndex < ColSize(); ++colIndex) {
                Position<ST>    srcPos(rowIndex, colIndex);
                Position<ST>    dstPos(rowIndex, ColSize() - (colIndex + 1));
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
        const ST    ru{ mat.RowSize () };
        const ST    cu{ mat.ColSize () };

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
