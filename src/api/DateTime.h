//
// Created by cefour on 12.05.22.
//

#ifndef DATACONTAINER_DATETIME_H
#define DATACONTAINER_DATETIME_H
#include <string>
#include <chrono>

#include "Crtp.h"
#include "NamedType.h"

struct YearParameter { };
struct MonthParameter{ };
struct DayParameter  { };
struct HourParameter { };
struct MinuteParameter { };
struct SecondParameter { };
struct MillisecondParameter { };
struct MicrosecondParameter { };
struct NanosecondParameter { };

using Year = NamedType<int,YearParameter>;
using Month = NamedType<int,MonthParameter>;
using Day = NamedType<int,DayParameter>;
using Hour =NamedType<int,HourParameter>;
using Minute = NamedType<int,MinuteParameter>;
using Second = NamedType<int,SecondParameter>;
using Millisecond = NamedType<int,MillisecondParameter>;
using Microsecond = NamedType<int,MicrosecondParameter>;
using Nanosecond = NamedType<int,NanosecondParameter>;

template<>
struct isCovertable<YearParameter,MonthParameter> {
    static constexpr bool value = true;
};

class DateTime{
private:
    Year year;
    Month month;
    Day day;
    Hour hour;
    Minute minutes;
    Second seconds;
    Millisecond milliseconds;
    Microsecond microseconds;
    Nanosecond nanoseconds;

public:
    DateTime() = default;

};

#endif //DATACONTAINER_DATETIME_H
