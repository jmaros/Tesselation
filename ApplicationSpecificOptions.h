//
// ApplicationSpecificOptions.h
//
#pragma once

#include "Options.h"

namespace Nessie {

    struct Opted {
        bool        m_isValid{ true };
        bool        m_askedForHelp{};
        bool        m_calculateAll{};
        bool        m_random{};
        bool        m_verbose{};
        bool        m_useLetters{ true };
        Date        m_date{ Date::GetCurrentDate() };
    };

    class ApplicationSpecificOptions : public Options {
    public:
    // constructors
        ApplicationSpecificOptions (int         argc,
                                    const char  * const argv[],
                                    const char  * const arge[])
         :     Options(argc,
                       argv,
                       arge)
         , m_longNamesMap   {{"year"            , 'y'},
                             {"month"           , 'm'},
                             {"day"             , 'd'},
                             {"all"             , 'a'},
                             {"help"            , 'h'},
                             {"random"          , 'r'},
                             {"verbose"         , 'v'}}
         , m_singleCharOpts  {'a', 'h', 'r', 'v', '@' }
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
        const map<string, char> & GetLongNamesMap       ()  const override;
        const set<char>         & GetSingleCharOpts     ()  const override;
        const map<char, string> & GetDefinedOptChars    ()  const override;
        inline const Opted      & GetOpted              ()  const;
        bool                    IsValid                 ()  const override;
        inline bool             AskedForHelp            ()  const;
        inline bool             CalculateAll            ()  const;
        inline bool             Random                  ()  const;
        inline bool             Verbose                 ()  const;
        const Date&             GetDate                 ()  const override;

        // modifiers
        void                    SetOpted                ();
        void                    Invalidate              () override;

    private:
    // data members
        map<string, char>   m_longNamesMap;
        set<char>           m_singleCharOpts;
        map<char, string>   m_definedOptChars;
        Opted               m_opted;
    };

    // accessors
    const map<string, char>     & ApplicationSpecificOptions::GetLongNamesMap    ()  const
    {
        return m_longNamesMap;
    }

    const set<char>             & ApplicationSpecificOptions::GetSingleCharOpts  ()  const
    {
        return m_singleCharOpts;
    }

    const map<char, string>     & ApplicationSpecificOptions::GetDefinedOptChars  () const
    {
        return m_definedOptChars;
    }

    inline const Opted& ApplicationSpecificOptions::GetOpted ()    const
    {
        return m_opted;
    }

    bool ApplicationSpecificOptions::IsValid ()    const
    {
        return m_opted.m_isValid;
    }

    inline bool ApplicationSpecificOptions::AskedForHelp ()    const
    {
        return m_opted.m_askedForHelp;
    }

    inline bool ApplicationSpecificOptions::CalculateAll ()  const
    {
        return m_opted.m_calculateAll;
    }

    inline bool ApplicationSpecificOptions::Random ()  const
    {
        return m_opted.m_random;
    }

    inline bool ApplicationSpecificOptions::Verbose ()  const
    {
        return m_opted.m_verbose;
    }

    const Date& ApplicationSpecificOptions::GetDate () const
    {
        return m_opted.m_date;
    }


    // modifiers
    inline void ApplicationSpecificOptions::Invalidate ()
    {
        m_opted.m_isValid = false;
    }

    inline void ApplicationSpecificOptions::SetOpted ()
    {
        for (auto am : GetArgsMap()) {
            switch (am.first) {
                case 'a':
                    // calculate all
                    m_opted.m_calculateAll = true;
                    break;
                case 'h':
                    // help
                    m_opted.m_askedForHelp = true;
                    break;
                case 'r':
                    // verbose
                    m_opted.m_random = true;
                    break;
                case 'v':
                    // verbose
                    m_opted.m_verbose = true;
                    break;
                case '@':
                    // Use special chars instead of letters [A-H] to display the shapes
                    m_opted.m_useLetters = false;
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
