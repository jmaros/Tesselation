//
// Options.h
//
#pragma once

#include "ApplicationSpecificOptios.h"

#include <vector>

namespace Nessie {

    using std::make_pair;
    using std::vector;

    using AppsOpt = ApplicationSpecificOptios;

    class Options {
    public:
    // constructors
        Options (int            argc,
                 const char     * const argv[],
                 const char     * const arge[],
                 const AppsOpt  & appsOpt = AppsOpt());
    // accessors
        const Opted& GetOpted ()    const
        {
            return m_appSpecOpt.GetOpted();
        }
        const Date& GetDate ()    const
        {
            return m_appSpecOpt.GetOpted().m_date;
        }

        bool IsValid () const
        {
            return m_appSpecOpt.IsValid();
        }
        bool AskedForHelp ()  const
        {
            return m_appSpecOpt.AskedForHelp();
        }

        // modifiers
        void Invalidate ()
        {
            m_appSpecOpt.Invalidate ();
        }

        void SetOpted ()
        {
            m_appSpecOpt.SetOpted(m_argMap);
        }

    private:
            int                 m_argc;
            vector<string>      m_argv;
            vector<string>      m_arge;
            AppsOpt             m_appSpecOpt;
            ArgsMap             m_argMap;
            bool                m_hasOptions;
    };

    Options::Options (int               argc,
                      const char        * const argv[],
                      const char        * const arge[],
                      const AppsOpt&    appsOpt)
     : m_argc           (argc)
     , m_argv           ()
     , m_arge           ()
     , m_appSpecOpt     (appsOpt)
     , m_argMap         ()
     , m_hasOptions     ()
    {
        auto    & doch  = m_appSpecOpt.GetDefinedOptChars();
        auto    & sco   = m_appSpecOpt.GetSingleCharOpts();
        auto    & lm    = m_appSpecOpt.GetLongNamesMap();
        int idax{};
        for (int idx = 0; idx < argc; ++idx) {
            m_argv.push_back(argv[idx]);
            const string & latestArg = m_argv.back();
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
                            m_argv.push_back(argv[++idx]);
                            argStr = m_argv.back();
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
                                m_argv.push_back(argv[++idx]);
                                argStr = m_argv.back();
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
                    cout << "Error: " << idx + 1 << ". argument('" << latestArg << "') is  invalid!" << endl;
                    break;
                }
            }
        }
        if (arge) {
            for (int ide = 0; arge[ide] != nullptr; ++ide) {
                m_arge.push_back(arge[ide]);
            }
        }
    }
} // namespace Nessie
