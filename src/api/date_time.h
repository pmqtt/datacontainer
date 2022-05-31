//
// Created by cefour on 12.05.22.
//

#ifndef DATACONTAINER_DATE_TIME_H
#define DATACONTAINER_DATE_TIME_H
#include <string>
#include <chrono>
#include <functional>
#include <map>

#include "format.h"
#include "crtp.h"
#include "named_type.h"

CREATE_STRONG_FIELD_TYPE(Year,long);
CREATE_STRONG_FIELD_TYPE(Month,long);
CREATE_STRONG_FIELD_TYPE(Day,long);
CREATE_STRONG_FIELD_TYPE(Hour,long);
CREATE_STRONG_FIELD_TYPE(Minute,long);
CREATE_STRONG_FIELD_TYPE(Second,long);
CREATE_STRONG_FIELD_TYPE(Millisecond,long);
CREATE_STRONG_FIELD_TYPE(Microsecond,long);
CREATE_STRONG_FIELD_TYPE(Nanosecond,long);



class date_time : public format_type_visitor{
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
private:
    format fmt;
    mutable std::string formatted_date;
public:
    date_time() { }
    static date_time timestamp();
public:
    void set_format(const std::string & format_spec);
    std::string to_string() const;

public:
    void visit(format_type_readable_month * f);
    void visit(format_type_month * f);
    void visit(format_type_word * f);
    void visit(format_type_year * f);
    void visit(format_type_short_year * f);
    void visit(format_type_day * f);
    void visit(format_type_time * f);
    void visit(format_type_time_all * f);
    void visit(format_type_time_hour * f);
    void visit(format_type_time_second * f);
    void visit(format_type_time_minute * f);
    void visit(format_type_time_milli * f);
    void visit(format_type_time_micro * f);
    void visit(format_type_time_nano * f);



};

#endif //DATACONTAINER_DATE_TIME_H
