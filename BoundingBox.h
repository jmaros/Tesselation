#pragma once
//
// BoundingBox.h
//
#include "Position.h"

#include <limits>

namespace Nessie {

    using std::numeric_limits;

template <typename S>
class BoundingBox {
public:
    // constructors
    BoundingBox () = default;

    template <typename ST>
    explicit BoundingBox (const Position<ST> & pos)
     : m_minRow    (pos.GetRowIndex())
     , m_maxRow    (pos.GetRowIndex())
     , m_minCol    (pos.GetColIndex())
     , m_maxCol    (pos.GetColIndex())
    {
    }

    // accessors
    S GetMinRow () const
    {
        return m_minRow;
    }

    S GetMaxRow () const
    {
        return m_maxRow;
    }

    S GetMinCol () const
    {
        return m_minCol;
    }

    S GetMaxCol () const
    {
        return m_maxCol;
    }

    S RowNum () const
    {
        S result{};
        if (m_minRow <= m_maxRow) {
            result = m_maxRow - m_minRow;
            ++result;
        }
        return result;
    }

    S ColNum () const
    {
        S result{};
        if (m_minCol <= m_maxCol) {
            result = m_maxCol - m_minCol;
            ++result;
        }
        return result;
    }

    // modifiers
    void AddPosition (S    row,
                      S    col)
    {
        if (row < m_minRow) {
            m_minRow = row;
        }
        if (row > m_maxRow) {
            m_maxRow = row;
        }
        if (col < m_minCol) {
            m_minCol = row;
        }
        if (col > m_maxCol) {
            m_maxCol = col;
        }
    }
    template <typename ST>
    void AddPosition (const Position<ST> & pos)
    {
        AddPosition (pos.GetRowIndex(),
                     pos.GetColIndex());
    }

private:
    S m_minRow{numeric_limits<S>::max()};
    S m_maxRow{numeric_limits<S>::min()};
    S m_minCol{numeric_limits<S>::max()};
    S m_maxCol{numeric_limits<S>::min()};
};

} // namespace Nessie
