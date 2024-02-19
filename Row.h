#pragma once
//
// Row.h
//
#include "Position.h"

namespace Nessie {
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

// definitions
    // constructors
    template <typename T>
    Row<T>::Row (size_t     numCols,
                 const T    & initialValue)
        : m_row   ()
    {
        m_row.resize(numCols,
                     initialValue);
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

// modifiers
    template <typename T>
    inline bool Row<T>::SetValue (size_t    col,
                                  T         value)
    {
        bool succ = col < Size();
        if (succ) {
            m_row[col] = value;
        }
        return succ;
    }

    template <typename T>
    inline void Row<T>::PushBack (const T        element)
    {
        m_row.push_back(element);
    }

} // namespace LinAlg
} // namespace Nessie
