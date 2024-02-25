//
// ElapsedTime.h
//
#pragma once

#include <chrono>       // chrono::system_clock
#include <iostream>     // cout

namespace Nessie {

    using namespace std::chrono;

    using std::cout;

    class ElapsedTime {
    public:
        ElapsedTime ()
            : m_startTime  (system_clock::now())
            , m_endTime    (m_startTime)
        {
        }

        ~ElapsedTime ()
        {
            m_endTime = system_clock::now();
            duration<double> elapsed_seconds = m_endTime - m_startTime;
            cout << "\n Elapsed Time: " << elapsed_seconds.count() << "s\n";
        }

    private:
        time_point<system_clock> m_startTime;
        time_point<system_clock> m_endTime;
    };

} // namespace Nessie
