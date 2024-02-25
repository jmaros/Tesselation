//
// Options.h
//
#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Nessie {

    using std::map;
    using std::pair;
    using std::string;
    using std::vector;

    class Options {
    public:
    // constructors
        Options (int    argc,
                 char* argv[],
                 char* arge[]);
    private:
            int                             m_argc;
            vector<string>                  m_argv;
            vector<string>                  m_arge;
            map<char, pair<int, string>>    m_argMap;
            bool                            m_hasOptions{};
    };

    Options::Options (int   argc,
                      char  * argv[],
                      char  * arge[])
     : m_argc   (argc)
     , m_argv   ()
     , m_arge   ()
     , m_argMap ()
    {
        for (int idx = 0; idx < argc; ++idx) {
            m_argv.push_back(argv[idx]);
            const string & latestArg = m_argv.back();
            if (latestArg[0] == '-') {
                m_argMap[latestArg[1]] = std::make_pair(idx, latestArg.substr(2));
            }
        }
        if (arge) {
            for (int ide = 0; arge[ide] != nullptr; ++ide) {
                m_arge.push_back(arge[ide]);
            }
        }
    }

} // namespace Nessie
