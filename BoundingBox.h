#pragma once
//
// BoundingBox.h
//
#include "Position.h"

#include <limits>

namespace Nessie {

    using std::numeric_limits;

template <typename STB>
class BoundingBox {
public:
    // constructors
    BoundingBox () = default;

    template <typename STP>
    explicit BoundingBox (const Position<STP> & pos)
     : m_minRow    (pos.GetRowIndex())
     , m_maxRow    (pos.GetRowIndex())
     , m_minCol    (pos.GetColIndex())
     , m_maxCol    (pos.GetColIndex())
    {
    }

    // accessors
    STB GetMinRow () const
    {
        return m_minRow;
    }

    STB GetMaxRow () const
    {
        return m_maxRow;
    }

    STB GetMinCol () const
    {
        return m_minCol;
    }

    STB GetMaxCol () const
    {
        return m_maxCol;
    }

    STB RowNum () const
    {
        STB result{};
        if (m_minRow <= m_maxRow) {
            result = m_maxRow - m_minRow;
            ++result;
        }
        return result;
    }

    STB ColNum () const
    {
        STB result{};
        if (m_minCol <= m_maxCol) {
            result = m_maxCol - m_minCol;
            ++result;
        }
        return result;
    }

    // modifiers
    void AddPosition (STB    row,
                      STB    col)
    {
        if (row < m_minRow) {
            m_minRow = row;
        }
        if (row > m_maxRow) {
            m_maxRow = row;
        }
        if (col < m_minCol) {
            m_minCol = col;
        }
        if (col > m_maxCol) {
            m_maxCol = col;
        }
    }
    template <typename STP>
    void AddPosition (const Position<STP> & pos)
    {
        AddPosition (pos.GetRowIndex(),
                     pos.GetColIndex());
    }

private:
    STB m_minRow{numeric_limits<STB>::max()};
    STB m_maxRow{numeric_limits<STB>::min()};
    STB m_minCol{numeric_limits<STB>::max()};
    STB m_maxCol{numeric_limits<STB>::min()};
};

} // namespace Nessie
