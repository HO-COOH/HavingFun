#include <chrono>
#include <iostream>

class Timer
{
    std::chrono::steady_clock::time_point last;
    std::chrono::steady_clock::duration duration{};
    bool started=false;
    bool natural_presentation;
public:
    Timer(bool start=false, bool natural_presentation=false):natural_presentation(natural_presentation)
    {
        if(start)
        {
            last=std::chrono::steady_clock::now();
            started=true;
        }
    }
    void start()
    {
        last=std::chrono::steady_clock::now();
        started=true;
    }
    void pause()
    {
        if(started)
        {
            duration+=(std::chrono::steady_clock::now()-last);
            started=false;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, Timer const& t)
    {
        if(t.natural_presentation)
        {
            auto temp=t.duration;
            const auto hours=std::chrono::duration_cast<std::chrono::hours>(temp);
            temp-=hours;
            const auto mins=std::chrono::duration_cast<std::chrono::minutes>(temp);
            temp-=mins;
            const auto secs=std::chrono::duration_cast<std::chrono::seconds>(temp);
            temp-=secs;
            const auto mills=std::chrono::duration_cast<std::chrono::milliseconds>(temp);
            temp-=mills;
            const auto micros=std::chrono::duration_cast<std::chrono::milliseconds>(temp);

            os<<hours.count()<<':'<<mins.count()<<':'<<secs.count()<<"::"<<mills.count()<<'.'<<micros.count()<<'\n';
        }
        else
            os<<std::chrono::duration_cast<std::chrono::microseconds>(t.duration).count()<<" ms.\n";
        return os;
    }
    ~Timer()
    {
        pause();
        std::cout<<*this;
    }
};
