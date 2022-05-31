//
// Created by cefour on 12.05.22.
//

#ifndef DATACONTAINER_DATETIME_H
#define DATACONTAINER_DATETIME_H
#include <string>
#include <chrono>
#include <functional>
#include <map>

#include "Format.h"
#include "crtp.h"
#include "NamedType.h"

CREATE_STRONG_FIELD_TYPE(Year,long);
CREATE_STRONG_FIELD_TYPE(Month,long);
CREATE_STRONG_FIELD_TYPE(Day,long);
CREATE_STRONG_FIELD_TYPE(Hour,long);
CREATE_STRONG_FIELD_TYPE(Minute,long);
CREATE_STRONG_FIELD_TYPE(Second,long);
CREATE_STRONG_FIELD_TYPE(Millisecond,long);
CREATE_STRONG_FIELD_TYPE(Microsecond,long);
CREATE_STRONG_FIELD_TYPE(Nanosecond,long);



class DateTime : public FormatTypeVisitor{
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
    Format format;
    mutable std::string formattedDate;
public:
    DateTime() { }
    static DateTime timestamp();
public:
    void setFormat(const std::string & format);
    std::string toString() const;

public:
    void visit(FormatTypeReadableMonth * f);
    void visit(FormatTypeMonth * f);
    void visit(FormatTypeWord * f);
    void visit(FormatTypeYear * f);
    void visit(FormatTypeShortYear * f);
    void visit(FormatTypeDay * f);
    void visit(FormatTypeTime * f);
    void visit(FormatTypeTimeAll * f);
    void visit(FormatTypeTimeHour * f);
    void visit(FormatTypeTimeSecond * f);
    void visit(FormatTypeTimeMinute * f);
    void visit(FormatTypeTimeMilli * f);
    void visit(FormatTypeTimeMicro * f);
    void visit(FormatTypeTimeNano * f);



};

#endif //DATACONTAINER_DATETIME_H
