#pragma once
//
// Position.h
//
#include <functional>
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
    using std::function;
    using std::map;
    using std::ostream;
    using std::size_t;
    using std::initializer_list;
    using std::is_same;
    using std::setw;
    //    using std::shared_ptr;
    using std::string;
    using std::stringstream;
    using std::vector;

template <typename TS>
class Position {
public:
    // constructors
    Position () = default;
    Position (TS rowIndex,
              TS colIndex)
        : m_rowIndex (rowIndex)
        , m_colIndex (colIndex)
    {
    }

    // accessors
    TS GetRowIndex () const
    {
        return m_rowIndex;
    }

    TS GetColIndex () const
    {
        return m_colIndex;
    }

    // modifiers
    void SetRowIndex (TS    rowIndex)
    {
        m_rowIndex = rowIndex;
    }

    void SetColIndex (TS    colIndex)
    {
        m_colIndex = colIndex;
    }
    void SetPosition (TS    rowIndex,
                      TS    colIndex)
    {
        m_rowIndex = rowIndex;
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
    TS m_rowIndex{};
    TS m_colIndex{};
};

// global operator
template <typename TS>
ostream& operator << (ostream             & os,
                      const Position<TS>  & pos)
{
    stringstream sos;
    sos << "Position = (";
    sos << pos.GetRowIndex();
    sos << ", ";
    sos << pos.GetColIndex();
    sos << ")";
    sos << endl;
    os << sos.str();
    return os;
}

template <typename TS>
Position<TS> operator+ (const Position<TS>  & lPos,
                        const Position<TS>  & rPos)
{
    return Position<TS> (lPos.GetRowIndex() + rPos.GetRowIndex(),
                         lPos.GetColIndex() + rPos.GetColIndex());
}

} // namespace Nessie
