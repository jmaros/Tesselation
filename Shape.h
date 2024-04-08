#pragma once
//
// Shape.h
//
#include "Matrix.h"

namespace Nessie {
namespace Geom {
// declarations
    // forward declarations
    template <typename T,
              typename U    = T,
              typename CT   = size_t>
    class Shape;

    // global operators
    template <typename T, typename U, typename CT>
    ostream& operator << (ostream                & os,
                          const Shape<T, U, CT>  & shape);

    template <typename T, typename U, typename CT>
    bool operator < (const Shape<T, U, CT>  & lShape,
                     const Shape<T, U, CT>  & rShape);

    template <typename T, typename U, typename CT>
    class Shape {
    public:
    // constructors
        Shape () = delete;
        Shape (CT       numRows,
               CT       numCols,
               const T  & initialValue ={});

        Shape (initializer_list<initializer_list<T>>    ili);

    // accessors
        CT                                  NumRows                     ()                  const;
        CT                                  NumCols                     ()                  const;

        inline bool                         CanAccomodate  (const Position<CT>  & pos,
                                                            const Shape<T, U>   & shape)    const;

        inline const T                      Value                       (Position<CT>  pos) const;
        inline const LinAlg::Matrix<T, CT>  & Matrix                    ()                  const;
        inline bool                         ShowZeros                   ()                  const;
        Shape<T, U, CT>                     CreateRotatedBy60Deg        ()                  const;
        Shape<T, U, CT>                     CreateTransposed            ()                  const;
        Shape<T, U, CT>                     CreateHorizontallyFlipped   ()                  const;
        Shape<T, U, CT>                     CreateVerticallyFlipped     ()                  const;
        inline bool                         SetMutableShowZeros         (bool    bShow)     const;
        const string                        & GetShapeName              ()                  const;

        // operators
        const LinAlg::Row<T>                & operator []               (CT      index)     const;

    // modifiers
        inline void     SetShapeName        (const string           & shana,
                                             CT                     index = 0);

        inline bool     SetData             (Position<CT>           pos,
                                             const T                & newValue);

        inline bool Accomodate              (const Position<CT>     & pos,
                                            const Shape<T, U, CT>   & shape);

        inline void     SetOutOfBoundValue  (const T                & newValue);

        // operators
        LinAlg::Row<T>  & operator []       (CT                     index);

    private:
    // data members
        string                  m_shapeName{ "Shape" };
        LinAlg::Matrix<T, CT>   m_matrix;
        mutable bool            m_showZeros{};
    };

// definitions
    // constructors

    template <typename T, typename U, typename CT>
    Shape<T, U, CT>::Shape (CT          numRows,
                            CT          numCols,
                            const T    & initialValue)
        : m_shapeName       ("Shape")
        , m_matrix          (numRows,
                             numCols,
                             initialValue)
        , m_showZeros       ()
    {
    }

    template <typename T, typename U, typename CT>
    Shape<T, U, CT>::Shape (initializer_list<initializer_list<T>>    ili)
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

    template <typename T, typename U, typename CT>
    CT Shape<T, U, CT>::NumRows () const
    {
        return m_matrix.RowSize();
    }

    template <typename T, typename U, typename CT>
    CT Shape<T, U, CT>::NumCols () const
    {
        return m_matrix.ColSize();
    }

    template <typename T, typename U, typename CT>
    inline bool Shape<T, U, CT>::CanAccomodate (const Position<CT> & pos,
                                                const Shape<T, U>  & shape) const
    {
        return m_matrix.CanAccomodate(pos, shape.m_matrix);
    }

    template <typename T, typename U, typename CT>
    inline const T    Shape<T, U, CT>::Value (Position<CT>  pos) const
    {
        return m_matrix.Value(pos);
    }

    template <typename T, typename U, typename CT>
    inline const LinAlg::Matrix<T, CT>  & Shape<T, U, CT>::Matrix () const
    {
        return m_matrix;
    }

    template <typename T, typename U, typename CT>
    inline bool     Shape<T, U, CT>::ShowZeros () const
    {
        if (m_showZeros) {
            return m_showZeros;
        } else {
            return m_showZeros;
        }
    }

    template <typename T, typename U, typename CT>
    Shape<T, U, CT> Shape<T, U, CT>::CreateRotatedBy60Deg () const
    {
        auto rotated = *this;
        rotated.m_matrix = m_matrix.CreateRotatedBy60Deg();
        return rotated;
    }

    template <typename T, typename U, typename CT>
    Shape<T, U, CT> Shape<T, U, CT>::CreateTransposed () const
    {
        auto transposed = *this;
        transposed.m_matrix = m_matrix.CreateTransposed();
        return transposed;
    }

    template <typename T, typename U, typename CT>
    Shape<T, U, CT> Shape<T, U, CT>::CreateHorizontallyFlipped () const
    {
        auto horizontallyFlipped = *this;
        horizontallyFlipped.m_matrix = m_matrix.CreateHorizontallyFlipped();
        return horizontallyFlipped;
    }

    template <typename T, typename U, typename CT>
    Shape<T, U, CT> Shape<T, U, CT>::CreateVerticallyFlipped () const
    {
        auto verticallyFlipped = *this;
        verticallyFlipped.m_matrix = m_matrix.CreateVerticallyFlipped();
        return verticallyFlipped;
    }

    template <typename T, typename U, typename CT>
    inline bool Shape<T, U, CT>::SetMutableShowZeros (bool     bShow)  const
    {
        bool bPrev{ m_showZeros };
        m_showZeros = bShow;
        return bPrev;
    }

    template <typename T, typename U, typename CT>
    const string& Shape<T, U, CT>::GetShapeName () const
    {
        return m_shapeName;

    }

    // operators
    template <typename T, typename U, typename CT>
    const LinAlg::Row<T>   & Shape<T, U, CT>::operator [] (CT  index)   const
    {
        return m_matrix[index];
    }

    // modifiers
    template <typename T, typename U, typename CT>
    inline void Shape<T, U, CT>::SetShapeName (const string     & shana,
                                               CT               index)
    {
        stringstream    ass;
        ass << shana;
        if (index) {
            ass << "-" << setw(3) << right << setfill('0') << index << ".";
        }
        m_shapeName = ass.str();
    }

    template <typename T, typename U, typename CT>
    inline bool Shape<T, U, CT>::SetData (Position<CT>  pos,
                                          const T       & newValue)
    {
        return m_matrix.SetData(pos, newValue);
    }

    template <typename T, typename U, typename CT>
    inline bool Shape<T, U, CT>::Accomodate (const Position<CT>     & pos,
                                             const Shape<T, U, CT>  & shape)
    {
        return m_matrix.Accomodate(pos, shape.m_matrix);
    }

    template <typename T, typename U, typename CT>
    inline void Shape<T, U, CT>::SetOutOfBoundValue (const T    & newValue)
    {
        return m_matrix.SetOutOfBoundValue(newValue);
    }

    // operators
    template <typename T, typename U, typename CT>
    LinAlg::Row<T>& Shape<T, U, CT>::operator [] (CT    index)
    {
        return m_matrix[index];
    }

    // global operators
    template <typename T, typename U, typename CT>
    ostream& operator << (ostream               & os,
                          const Shape<T, U, CT> & shape)
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
                    sos << setw(sw) << ".";
                }
            }
            sos << endl;
        }
        os << sos.str();
        return os;
    }

    template <typename T, typename U, typename CT>
    bool operator < (const Shape<T, U, CT>     & lShape,
                     const Shape<T, U, CT>     & rShape)
    {
        auto numRows = lShape.NumRows();
        if (numRows != rShape.NumRows()) {
            return numRows < rShape.NumRows ();
        }
        auto numCols = lShape.NumCols();
        if (numCols != rShape.NumCols()) {
            return numCols < rShape.NumCols ();
        }
        for (CT idx = 0; idx < numRows; ++idx) {
            for (CT idy = 0; idy < numCols; ++idy) {
                Position<CT> pos(idx, idy);
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
