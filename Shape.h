#pragma once
//
// Shape.h
//
#include "Matrix.h"

namespace Nessie {
namespace Geom {
// declarations
    // forward declarations
    template <typename T, typename U = T>
    class Shape;

    // global operators
    template <typename T, typename U>
    ostream& operator << (ostream            & os,
                          const Shape<T, U>  & shape);

    template <typename T, typename U>
    bool operator < (const Shape<T, U>  & lShape,
                     const Shape<T, U>  & rShape);

    template <typename T, typename U>
    class Shape {
    public:
    // constructors
        Shape () = delete;
        Shape (size_t   numRows,
               size_t   numCols,
               const T  & initialValue ={});

        Shape (initializer_list<initializer_list<T>>    ili);

    // accessors
        size_t                          NumRows                     ()              const;
        size_t                          NumCols                     ()              const;

        inline bool                     CanAccomodate  (const Position     & pos,
                                                        const Shape<T, U>  & shape) const;

        inline const T                  Value                       (Position  pos) const;
        inline const LinAlg::Matrix<T>  & Matrix                    ()              const;
        inline bool                     ShowZeros                   ()              const;
        Shape<T, U>                     CreateRotatedBy45Deg        ()              const;
        Shape<T, U>                     CreateTransposed            ()              const;
        Shape<T, U>                     CreateHorizontallyFlipped   ()              const;
        Shape<T, U>                     CreateVerticallyFlipped     ()              const;
        inline bool                     SetMutableShowZeros         (bool    bShow) const;
        const string                    & GetShapeName              ()              const;

        // operators
        const LinAlg::Row<T>            & operator []               (size_t  index) const;

    // modifiers
        inline void     SetShapeName        (const string       & shana,
                                             size_t             index = 0);

        inline bool     SetData             (Position           pos,
                                             const T            & newValue);

        inline bool Accomodate              (const Position     & pos,
                                            const Shape<T, U>   & shape);

        inline void     SetOutOfBoundValue  (const T            & newValue);

        // operators
        LinAlg::Row<T>  & operator []       (size_t             index);

    private:
    // data members
        string              m_shapeName{ "Shape" };
        LinAlg::Matrix<T>   m_matrix;
        mutable bool        m_showZeros{};
    };

// definitions
    // constructors

    template <typename T, typename U>
    Shape<T, U>::Shape (size_t     numRows,
                        size_t     numCols,
                        const T    & initialValue)
        : m_shapeName       ("Shape")
        , m_matrix          (numRows,
                             numCols,
                             initialValue)
        , m_showZeros       ()
    {
    }

    template <typename T, typename U>
    Shape<T, U>::Shape (initializer_list<initializer_list<T>>    ili)
     : m_shapeName      ("Shape")
     , m_matrix         (0u,0u)
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
    }

    // accessors

    template <typename T, typename U>
    size_t Shape<T, U>::NumRows ()                                      const
    {
        return m_matrix.RowSize();
    }

    template <typename T, typename U>
    size_t Shape<T, U>::NumCols ()                                      const
    {
        return m_matrix.ColSize();
    }

    template <typename T, typename U>
    inline bool Shape<T, U>::CanAccomodate (const Position     & pos,
                                            const Shape<T, U>  & shape) const
    {
        return m_matrix.CanAccomodate(pos, shape.m_matrix);
    }

    template <typename T, typename U>
    inline const T    Shape<T, U>::Value (Position  pos)                const
    {
        return m_matrix.Value(pos);
    }

    template <typename T, typename U>
    inline const LinAlg::Matrix<T>  & Shape<T, U>::Matrix ()            const
    {
        return m_matrix;
    }

    template <typename T, typename U>
    inline bool     Shape<T, U>::ShowZeros ()                           const
    {
        if (m_showZeros) {
            return m_showZeros;
        } else {
            return m_showZeros;
        }
    }

    template <typename T, typename U>
    Shape<T, U> Shape<T, U>::CreateRotatedBy45Deg ()                    const
    {
        auto transposed = *this;
        transposed.m_matrix = m_matrix.CreateRotatedBy45Deg();
        return transposed;
    }

    template <typename T, typename U>
    Shape<T, U> Shape<T, U>::CreateTransposed ()                        const
    {
        auto transposed = *this;
        transposed.m_matrix = m_matrix.CreateTransposed();
        return transposed;
    }

    template <typename T, typename U>
    Shape<T, U> Shape<T, U>::CreateHorizontallyFlipped ()               const
    {
        auto horizontallyFlipped = *this;
        horizontallyFlipped.m_matrix = m_matrix.CreateHorizontallyFlipped();
        return horizontallyFlipped;
    }

    template <typename T, typename U>
    Shape<T, U> Shape<T, U>::CreateVerticallyFlipped ()                 const
    {
        auto verticallyFlipped = *this;
        verticallyFlipped.m_matrix = m_matrix.CreateVerticallyFlipped();
        return verticallyFlipped;
    }

    template <typename T, typename U>
    inline bool Shape<T, U>::SetMutableShowZeros (bool     bShow)       const
    {
        bool bPrev{ m_showZeros };
        m_showZeros = bShow;
        return bPrev;
    }

    template <typename T, typename U>
    const string& Shape<T, U>::GetShapeName ()                          const
    {
        return m_shapeName;

    }

    // operators
    template <typename T, typename U>
    const LinAlg::Row<T>   & Shape<T, U>::operator [] (size_t  index)   const
    {
        return m_matrix[index];
    }

    // modifiers
    template <typename T, typename U>
    inline void Shape<T, U>::SetShapeName (const string    & shana,
                                           size_t          index)
    {
        stringstream    ass;
        ass << shana;
        if (index) {
            ass << "-" << setw(3) << right << setfill('0') << index << ".";
        }
        m_shapeName = ass.str();
    }

    template <typename T, typename U>
    inline bool Shape<T, U>::SetData (Position     pos,
                                      const T      & newValue)
    {
        return m_matrix.SetData(pos, newValue);
    }

    template <typename T, typename U>
    inline bool Shape<T, U>::Accomodate (const Position     & pos,
                                         const Shape<T, U>  & shape)
    {
        return m_matrix.Accomodate(pos, shape.m_matrix);
    }

    template <typename T, typename U>
    inline void Shape<T, U>::SetOutOfBoundValue (const T    & newValue)
    {
        return m_matrix.SetOutOfBoundValue(newValue);
    }

    // operators
    template <typename T, typename U>
    LinAlg::Row<T>& Shape<T, U>::operator [] (size_t    index)
    {
        return m_matrix[index];
    }

    // global operators
    template <typename T, typename U>
    ostream& operator << (ostream            & os,
                          const Shape<T, U>  & shape)
    {
        constexpr bool IsBool{ std::is_same<T, bool>::value };
        constexpr bool IsChar{ std::is_same<T, char>::value };
        constexpr size_t sw = (IsBool || IsChar) ? 1 : 4;
        stringstream sos;
        sos << shape.GetShapeName() << " = \n";
        string prep{ "          " };
        for (auto& row : shape.Matrix().GetRows()) {
            sos << prep;
            for (auto element : row.Elements()) {
                if (shape.ShowZeros() || element) {
                    sos << setw(sw) << U(element);
                } else {
                    sos << setw(sw) << " ";
                }
            }
            sos << endl;
        }
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
