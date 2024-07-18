//
// TileOptions.h
//
#pragma once

#include "Options.h"

namespace Nessie
{

    struct Opted
    {
        bool    m_isValid{true};
        bool    m_askedForHelp{};
        bool    m_hexagonal{};
        bool    m_calculateAll{};
        bool    m_random{};
        size_t  m_steps{};
        bool    m_verbose{};
        bool    m_useLetters{true};
        Date    m_date{Date::GetCurrentDate()};
    };

    class TileOptions : public Options
    {
    public:
        // constructors
        TileOptions(int             argc,
                    const char      * const argv[],
                    const char      * const arge[])
         : Options(argc,
                   argv,
                   arge)
         , m_longNamesMap{{"year",                   'y'},
                          {"month",                  'm'},
                          {"day",                    'd'},
                          {"all",                    'a'},
                          {"help",                   'h'},
                          {"random",                 'r'},
                          {"steps",                  's'},
                          {"verbose",                'v'},
                          {"hexagonal",              '6'}}
         , m_singleCharOpts{'a', 'h', 'r', 'v', '6', '@'}
         , m_definedOptChars{}
         , m_opted()
        {
            for (const auto &mi : m_longNamesMap) {     // create the {short --> long} inverse mapping
                m_definedOptChars[mi.second] = mi.first;
            }
            for (auto ch : m_singleCharOpts) {          // add the short name options that have
                auto it = m_definedOptChars.find(ch);   // no long names to the inverse mapping
                if (it == m_definedOptChars.end()) {    // there could be other categories defined
                    m_definedOptChars[ch] = string{};   // for one or more additional parameters
                }
            }
        }
        // accessors
        const map<string, char>  & GetLongNamesMap      () const override;
        const set<char>          & GetSingleCharOpts    () const override;
        const map<char, string>  & GetDefinedOptChars   () const override;
        inline const Opted       & GetOpted             () const;
        bool                     IsValid                () const override;
        inline bool              AskedForHelp           () const;
        inline bool              IsHexagonal            () const;
        inline bool              CalculateAll           () const;
        inline bool              Random                 () const;
        inline size_t            Steps                  () const;
        inline bool              Verbose                () const;
        const Date               & GetDate              () const override;

        // modifiers
        void                    SetOpted                ();
        void                    Invalidate              () override;

    private:
        // data members
        map<string, char>       m_longNamesMap;
        set<char>               m_singleCharOpts;
        map<char, string>       m_definedOptChars;
        Opted                   m_opted;
    };

    // accessors
    const map<string, char> &TileOptions::GetLongNamesMap () const
    {
        return m_longNamesMap;
    }

    const set<char> &TileOptions::GetSingleCharOpts () const
    {
        return m_singleCharOpts;
    }

    const map<char, string> &TileOptions::GetDefinedOptChars () const
    {
        return m_definedOptChars;
    }

    inline const Opted &TileOptions::GetOpted () const
    {
        return m_opted;
    }

    bool TileOptions::IsValid () const
    {
        return m_opted.m_isValid;
    }

    inline bool TileOptions::AskedForHelp () const
    {
        return m_opted.m_askedForHelp;
    }

    inline bool TileOptions::IsHexagonal () const
    {
        return m_opted.m_hexagonal;
    }

    inline bool TileOptions::CalculateAll () const
    {
        return m_opted.m_calculateAll;
    }

    inline bool TileOptions::Random () const
    {
        return m_opted.m_random;
    }

    inline size_t TileOptions::Steps () const
    {
        return m_opted.m_steps;
    }

    inline bool TileOptions::Verbose () const
    {
        return m_opted.m_verbose;
    }

    const Date &TileOptions::GetDate () const
    {
        return m_opted.m_date;
    }

    // modifiers
    inline void TileOptions::Invalidate ()
    {
        m_opted.m_isValid = false;
    }

    inline void TileOptions::SetOpted ()
    {
        for (auto am : GetArgsMap()) {
            switch (am.first) {
                case '6':
                    // Use the hexagonal playground
                    m_opted.m_hexagonal = true;
                    break;
                case 'a':
                    // calculate all
                    m_opted.m_calculateAll = true;
                    break;
                case 'h':
                    // help
                    m_opted.m_askedForHelp = true;
                    break;
                case 'r':
                    // random
                    m_opted.m_random = true;
                    break;
                case 's':
                    {
                        // steps
                        stringstream ios(am.second.second);
                        ios >> m_opted.m_steps;
                        string rest;
                        ios >> rest;
                        if (!rest.empty() || m_opted.m_steps <= 0) {
                            m_opted.m_isValid = false;
                        }
                    }
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
