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

template <typename STP>
class Position {
public:
    // constructors
    Position () = default;
    Position (STP rowIndex,
              STP colIndex)
        : m_rowIndex (rowIndex)
        , m_colIndex (colIndex)
    {
    }

    // accessors
    STP GetRowIndex () const
    {
        return m_rowIndex;
    }

    STP GetColIndex () const
    {
        return m_colIndex;
    }

    // modifiers
    void SetRowIndex (STP    rowIndex)
    {
        m_rowIndex = rowIndex;
    }

    void SetColIndex (STP    colIndex)
    {
        m_colIndex = colIndex;
    }
    void SetPosition (STP    rowIndex,
                      STP    colIndex)
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
    STP m_rowIndex{};
    STP m_colIndex{};
};

// global operator
template <typename STP>
ostream& operator << (ostream             & os,
                      const Position<STP>  & pos)
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

template <typename STP>
Position<STP> operator+ (const Position<STP>  & lPos,
                         const Position<STP>  & rPos)
{
    return Position<STP> (lPos.GetRowIndex() + rPos.GetRowIndex(),
                         lPos.GetColIndex() + rPos.GetColIndex());
}

} // namespace Nessie
