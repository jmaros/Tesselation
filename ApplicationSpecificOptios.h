//
// ApplicationSpecificOptios.h
//
#pragma once
#include "Date.h"

#include <set>
#include <string>
#include <utility>

namespace Nessie {

    using std::pair;
    using std::set;
    using std::string;

    using ArgsMap = map<char, pair<int, string>>;

    struct Opted {
        bool        m_isValid{ true };
        bool        m_askedForHelp{};
        bool        m_calculateAll{};
        bool        m_verbose{};
        bool        m_useLetters{};
        Date        m_date{ Date::GetCurrentDate() };
    };

    class ApplicationSpecificOptios {
    public:
    // constructors
        ApplicationSpecificOptios ()
            : m_longNamesMap   {{"year"       , 'y'},
                                {"month"      , 'm'},
                                {"day"        , 'd'},
                                {"all"        , 'a'},
                                {"help"       , 'h'},
                                {"verbose"    , 'v'}}
            , m_singleCharOpts  {'a', 'h', 'v', 'A' }
            , m_definedOptChars {}
            , m_opted           ()
        {
            for (const auto& mi : m_longNamesMap) {         // create the {short --> long} inverse mapping
                m_definedOptChars[mi.second] = mi.first;
            }
            for (auto ch : m_singleCharOpts) {              // add the short name options that have
                auto it = m_definedOptChars.find(ch);       // no long names to the inverse mapping
                if (it == m_definedOptChars.end()) {        // there could be other categories defined
                    m_definedOptChars[ch] = string{};       // for one or more additional parameters
                }
            }
        }
    // accessors
        const map<string, char> & GetLongNamesMap       ()  const;
        const set<char>         & GetSingleCharOpts     ()  const;
        const map<char, string> & GetDefinedOptChars    ()  const;
        const Opted             & GetOpted              ()  const;
        bool                    IsValid                 ()  const;
        bool                    AskedForHelp            ()  const;
         // modifiers
        void                    SetOpted (const ArgsMap& argsMap);
        void                    Invalidate              ();

    private:
    // data members
        map<string, char>   m_longNamesMap;
        set<char>           m_singleCharOpts;
        map<char, string>   m_definedOptChars;
        Opted               m_opted;
    };

    // accessors
    const map<string, char>     & ApplicationSpecificOptios::GetLongNamesMap    ()  const
    {
        return m_longNamesMap;
    }

    const set<char>             & ApplicationSpecificOptios::GetSingleCharOpts  ()  const
    {
        return m_singleCharOpts;
    }

    const map<char, string>     & ApplicationSpecificOptios::GetDefinedOptChars  () const
    {
        return m_definedOptChars;
    }

    const Opted& ApplicationSpecificOptios::GetOpted ()    const
    {
        return m_opted;
    }

    bool ApplicationSpecificOptios::IsValid ()    const
    {
        return m_opted.m_isValid;
    }

    bool ApplicationSpecificOptios::AskedForHelp ()    const
    {
        return m_opted.m_askedForHelp;
    }

    // modifiers
    void ApplicationSpecificOptios::Invalidate ()
    {
        m_opted.m_isValid = false;
    }

    void ApplicationSpecificOptios::SetOpted (const ArgsMap& argsMap)
    {
        for (auto am : argsMap) {
            switch (am.first) {
                case 'a':
                    // calculate all
                    m_opted.m_calculateAll = true;
                    break;
                case 'h':
                    // help
                    m_opted.m_askedForHelp = true;
                    break;
                case 'v':
                    // verbose
                    m_opted.m_verbose = true;
                    break;
                case 'A':
                    // Use letters [A-H] to display the shapes
                    m_opted.m_useLetters = true;
                    break;
                case 'y':
                    // Override "year"
                    m_opted.m_date.SetYear(am.second.second);
                    break;
                case 'm':
                    // Override "month"
                    m_opted.m_date.SetMonth(am.second.second);
                    break;
                case 'd':
                    // Override "day"
                    m_opted.m_date.SetDay(am.second.second);
                    break;
                default:
                    // Unhandled option
                    m_opted.m_isValid = false;
                    break;
            }
        }
    }
} // namespace Nessie
