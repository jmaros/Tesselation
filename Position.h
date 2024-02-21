#pragma once
//
// Position.h
//
#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

namespace Nessie {

    using std::cout;
    using std::endl;
    using std::map;
    using std::ostream;
    using std::size_t;
    using std::initializer_list;
    using std::setw;
    //    using std::shared_ptr;
    using std::string;
    using std::stringstream;
    using std::vector;

class Position {
public:
    // constructors
    Position () = default;
    Position (size_t rowIndex,
              size_t colIndex)
        : m_rowIndex (rowIndex)
        , m_colIndex (colIndex)
    {
    }

    // accessors
    size_t GetRowIndex () const
    {
        return m_rowIndex;
    }

    size_t GetColIndex () const
    {
        return m_colIndex;
    }

    // modifiers
    void SetRowIndex (size_t    rowIndex)
    {
        m_rowIndex = rowIndex;
    }

    void SetColIndex (size_t    colIndex)
    {
        m_colIndex = colIndex;
    }

    void IncRow ()
    {
        ++m_rowIndex;
    }

    void IncCol ()
    {
        ++m_colIndex;
    }

private:
    size_t m_rowIndex{};
    size_t m_colIndex{};
};

// global operator
Position operator+ (const Position  & lPos,
                    const Position  & rPos)
{
    return Position (lPos.GetRowIndex() + rPos.GetRowIndex(),
                     lPos.GetColIndex() + rPos.GetColIndex());
}

} // namespace Nessie
