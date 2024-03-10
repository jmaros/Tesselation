//
// Options.h
//
#pragma once

#include "Date.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

namespace Nessie {

    using std::pair;
    using std::set;
    using std::string;
    using std::make_pair;
    using std::vector;

    using ArgsMap = map<char, pair<int, string>>;

    class Options {
    public:
    // constructors
        Options (int            argc,
                 const char* const argv[],
                 const char* const arge[]);

        virtual ~Options () = default;

        // accessors
        virtual const map<string, char>     & GetLongNamesMap       () const = 0;
        virtual const set<char>             & GetSingleCharOpts     () const = 0;
        virtual const map<char, string>     & GetDefinedOptChars    () const = 0;
        virtual bool                        IsValid                 () const = 0;
        virtual const Date                  & GetDate               () const = 0;

        const ArgsMap                       & GetArgsMap            () const
        {
            return m_argMap;
        }

    // modifiers
        virtual void                        Invalidate              () = 0;
        void                                Evaluate                ();
    private:
            vector<string>      m_argv;
            vector<string>      m_arge;
            ArgsMap             m_argMap;
    };

    Options::Options (int               argc,
                      const char* const argv[],
                      const char* const arge[])
        : m_argv           ()
        , m_arge           ()
        , m_argMap         ()
    {
        for (int idx = 0; idx < argc; ++idx) {
            m_argv.push_back(argv[idx]);
        }
        if (arge) {
            for (int ide = 0; arge[ide] != nullptr; ++ide) {
                m_arge.push_back(arge[ide]);
            }
        }
    }

    void Options::Evaluate ()
    {
        auto    & doch  = GetDefinedOptChars();
        auto    & sco   = GetSingleCharOpts();
        auto    & lm    = GetLongNamesMap();
        int idax{};
        for (size_t idx = 0; idx < m_argv.size(); ++idx) {
            const string& latestArg = m_argv[idx];
            if (!latestArg.empty() &&
                latestArg[0] == '-') {
                // expecting named command line argument
                // latestArg[1] can be 0, but must
                // exist at this point of execution!
                char    optionChar{ latestArg[1] };
                string  argStr{};
                if (optionChar == '-') {
                    // long name
                    string      longName    = latestArg.substr(2);
                    const auto  cit         = lm.find(longName);
                    if (cit != lm.end()) {
                        // valid long option name found
                        optionChar = cit->second;

                        auto scit = sco.find(optionChar);
                        if (scit == sco.end()) {
                            // has argument
                            argStr = m_argv[++idx];
                        }

                        m_argMap[optionChar] = make_pair(++idax, argStr);
                    } else {
                        Invalidate();
                        break;
                    }
                } else {
                    // short name
                    size_t  idc     = 1;

                    // we allow any number of single-char options to be specified
                    // after a single dash, and the last option can even require a parameter
                    while (idc < latestArg.size()) {
                        optionChar = latestArg[idc++];
                        if (doch.find(optionChar) == doch.end()) {
                            Invalidate();
                            break;
                        }
                        argStr.clear();
                        auto scit = sco.find(optionChar);
                        if (scit == sco.end()) {
                            // has argument
                            argStr = latestArg.substr(idc);
                            if (argStr.empty()) {
                                // the parameter is in a separate argument
                                argStr = m_argv[++idx];
                            }
                            m_argMap[optionChar] = make_pair(++idax, argStr);
                            break;
                        } else {
                            m_argMap[optionChar] = make_pair(++idax, argStr);
                        }
                    }
                }
                if (!IsValid()) {
                    // invalid argument
                    cout << "Error: " << idx << ". argument('" << latestArg << "') is  invalid!" << endl;
                    break;
                }
            }
        }
    }
} // namespace Nessie
