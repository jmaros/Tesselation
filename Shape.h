#pragma once
//
// Shape.h
//
#include "Matrix.h"

namespace Nessie {
namespace Geom {
// declarations
    // forward declarations
    template <typename T>
    class Shape;

    // global operators
    template <typename T>
    ostream& operator << (ostream            & os,
                          const Shape<T>     & shape);

    template <typename T>
    bool operator < (const Shape<T>     & lShape,
                     const Shape<T>     & rShape);

    template <typename T>
    class Shape {
    public:
    // constructors
        Shape () = delete;
        Shape (size_t    numCols,
               size_t    numRows);

        Shape (initializer_list<initializer_list<T>>    ili);

    // accessors
        size_t                          NumCols                     ()              const;
        size_t                          NumRows                     ()              const;

        inline bool                     CanAccomodate  (const Position     & pos,
                                                        const Shape<T>     & shape) const;

        inline const T                  Value                       (Position  pos) const;
        inline const LinAlg::Matrix<T>  & Matrix                    ()              const;
        inline bool                     ShowZeros                   ()              const;
        Shape<T>                        CreateTransposed            ()              const;
        Shape<T>                        CreateHorizontallyFlipped   ()              const;
        Shape<T>                        CreateVerticallyFlipped     ()              const;

        // operators
        const LinAlg::Row<T>              & operator [] (size_t index)  const;

    // modifiers
        inline bool     SetData             (Position       pos,
                                             const T        & newValue);

        inline bool Accomodate              (const Position & pos,
                                            const Shape<T>  & shape);

        inline void     SetOutOfBoundValue  (const T        & newValue);
        inline bool     SetShowZeros        (bool           bShow);

        // operators
        LinAlg::Row<T>  & operator []       (size_t         index);

    private:
    // data members
        LinAlg::Matrix<T>   m_matrix;
        bool                m_showZeros{};
    };

// definitions
    // constructors

    template <typename T>
    Shape<T>::Shape (size_t    numCols,
                     size_t    numRows)
     : m_matrix         (numCols,
                         numRows)
        , m_showZeros   ()
    {
    }

#if defined (VERBOSE)
    namespace {
        int shapeNumber{};
    }
#endif

    template <typename T>
    Shape<T>::Shape (initializer_list<initializer_list<T>>    ili)
     : m_matrix         (0u,0u)
     , m_showZeros      ()
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
        if (shapeNumber == 0) {
            cout << "Extended Table";
        } else {
            cout << shapeNumber << '.';
        }
        ++shapeNumber;
        cout << *this;
#endif
    }

    // accessors

    template <typename T>
    size_t Shape<T>::NumRows ()                                     const
    {
        return m_matrix.RowSize();
    }

    template <typename T>
    size_t Shape<T>::NumCols ()                                     const
    {
        return m_matrix.ColSize();
    }

    template <typename T>
    inline bool Shape<T>::CanAccomodate (const Position     & pos,
                                          const Shape<T>    & shape) const
    {
        return m_matrix.CanAccomodate(pos, shape.m_matrix);
    }

    template <typename T>
    inline const T    Shape<T>::Value (Position  pos)               const
    {
        return m_matrix.Value(pos);
    }

    template <typename T>
    inline const LinAlg::Matrix<T>  & Shape<T>::Matrix ()           const
    {
        return m_matrix;
    }
    
    template <typename T>
    inline bool     Shape<T>::ShowZeros ()                          const
    {
        if (m_showZeros) {
            return m_showZeros;
        } else {
            return m_showZeros;
        }
    }

    template <typename T>
    Shape<T> Shape<T>::CreateTransposed            ()              const
    {
        auto transposed = *this;
        transposed.m_matrix = m_matrix.CreateTransposed();
        return transposed;
    }

    template <typename T>
    Shape<T> Shape<T>::CreateHorizontallyFlipped   ()              const
    {
        auto horizontallyFlipped = *this;
        horizontallyFlipped.m_matrix = m_matrix.CreateHorizontallyFlipped();
        return horizontallyFlipped;
    }

    template <typename T>
    Shape<T> Shape<T>::CreateVerticallyFlipped     ()              const
    {
        auto verticallyFlipped = *this;
        verticallyFlipped.m_matrix = m_matrix.CreateVerticallyFlipped();
        return verticallyFlipped;
    }

    // operators
    template <typename T>
    const LinAlg::Row<T>   & Shape<T>::operator [] (size_t  index)  const
    {
        return m_matrix[index];
    }

    // modifiers
    template <typename T>
    inline bool Shape<T>::SetData (Position     pos,
                                   const T      & newValue)
    {
        return m_matrix.SetData(pos, newValue);
    }

    template <typename T>
    inline bool Shape<T>::Accomodate (const Position    & pos,
                                      const Shape<T>    & shape)
    {
        return m_matrix.Accomodate(pos, shape.m_matrix);
    }

    template <typename T>
    inline void Shape<T>::SetOutOfBoundValue (const T    & newValue)
    {
        return m_matrix.SetOutOfBoundValue(newValue);
    }

    template <typename T>
    inline bool Shape<T>::SetShowZeros (bool     bShow)
    {
        bool bPrev{ m_showZeros };
        m_showZeros = bShow;
        return bPrev;
    }

        // operators
    template <typename T>
    LinAlg::Row<T>& Shape<T>::operator [] (size_t    index)
    {
        return m_matrix[index];
    }

    // global operators
    template <typename T>
    ostream& operator << (ostream            & os,
                          const Shape<T>     & shape)
    {
        stringstream sos;
        sos << "Shape = \n";
        string prep{ "          " };
        for (auto& row : shape.Matrix().GetRows()) {
            sos << prep;
            for (auto element : row.Elements()) {
                if (shape.ShowZeros() || element) {
                             sos << setw(3) << element;
                } else {
                    sos << setw(3) << " ";
                }
            }
            sos << endl;
        }
        sos << endl;
        os << sos.str();
        return os;
    }

    template <typename T>
    bool operator < (const Shape<T>     & lShape,
                     const Shape<T>     & rShape)
    {
        auto numRows = lShape.NumRows();
        if (numRows != rShape.NumRows()) {
            return numRows < rShape.NumRows ();
        }
        auto numCols = lShape.NumCols();
        if (numCols != rShape.NumCols()) {
            return numCols < rShape.NumCols ();
        }
        for (size_t idx = 0; idx < numRows; ++idx) {
            for (size_t idy = 0; idy < numCols; ++idy) {
                Position pos(idx, idy);
                auto lValue = lShape.Value(pos);
                auto rValue = rShape.Value(pos);
                if (lValue != rValue) {
                    return lValue < rValue;
                }
            }
        }
        return false;
    }
} // namespace Geom
} // namespace Nessie
