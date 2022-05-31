//
// Created by cefour on 16.05.22.
//

#include "DateTime.h"
#include <locale>
#include <chrono>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>


DateTime DateTime::timestamp() {
    DateTime dateTime;
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&in_time_t);

    auto ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch())    % 1000;
    auto mikro = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000000;
    auto ns = duration_cast<std::chrono::milliseconds>(now.time_since_epoch())    % 1000000000;

    dateTime.year  = Year(local_tm.tm_year+1900);
    dateTime.month =  Month(local_tm.tm_mon+1);
    dateTime.day   =  Day(local_tm.tm_mday);
    dateTime.hour = Hour(local_tm.tm_hour);
    dateTime.minutes = Minute(local_tm.tm_min);
    dateTime.seconds = Second(local_tm.tm_sec);
    dateTime.milliseconds = Millisecond(ms.count());
    dateTime.microseconds = Microsecond(mikro.count());
    dateTime.nanoseconds = Nanosecond(ns.count());

    return dateTime;

}


/*
  %y %M %d - %X
 */
void DateTime::setFormat(const std::string& formatSpec){
    FormatSpecifier spec = {
            {'Y',std::make_shared<FormatTypeYear>()},
            {'y',std::make_shared<FormatTypeShortYear>()},
            {'M',std::make_shared<FormatTypeReadableMonth>()},
            {'m',std::make_shared<FormatTypeMonth>()},
            {'d',std::make_shared<FormatTypeDay>()},
            {'T',std::make_shared<FormatTypeTime>()},
            {'A',std::make_shared<FormatTypeTimeAll>()},
            {'h',std::make_shared<FormatTypeTimeHour>()},
            {'s',std::make_shared<FormatTypeTimeSecond>()},
            {'i',std::make_shared<FormatTypeTimeMinute>()},
            {'l',std::make_shared<FormatTypeTimeMilli>()},
            {'c',std::make_shared<FormatTypeTimeMicro>()},
            {'n',std::make_shared<FormatTypeTimeNano>()},

    };
    Format f(formatSpec,spec);
    this->format = f;
}

std::string DateTime::toString() const{

    auto formatList = this->format.getFormateList();
    for(auto & x : formatList){
        x->hanlde(const_cast<DateTime*>(this));
    }
    std::string buffer = formattedDate;
    formattedDate = "";
    return buffer;
}

void DateTime::visit(FormatTypeMonth * f) {
    UNUSED(f);
    std::string v = month.toString();
    if(v.length() == 1){
        v = "0"+v;
    }

    formattedDate += v;
}

void DateTime::visit(FormatTypeReadableMonth * f){
    UNUSED(f);
    static std::map<std::string,std::string> monthConverter = {
            {"01","Jan"},{"02","Feb"},{"03","Mar"},
            {"04","Apr"},{"05","Mai"},{"06","Jun"},
            {"07","Jul"},{"08","Aug"},{"09","Sep"},
            {"10","Oct"},{"11","Nov"},{"12","Dez"}
    };
    std::string v = month.toString();
    if(v.length() == 1){
        v = "0"+v;
    }

    formattedDate += monthConverter[v];
}

void DateTime::visit(FormatTypeYear * f){
    UNUSED(f);
    formattedDate += year.toString();
}

void DateTime::visit(FormatTypeDay * f){
    UNUSED(f);
    formattedDate += day.toString();
}
void DateTime::visit(FormatTypeWord * f){
    UNUSED(f);
    formattedDate += f->toAcceptedWord;
}
void DateTime::visit(FormatTypeShortYear * f){
    UNUSED(f);
    std::string buffer ="";
    buffer += year.toString()[2];
    buffer += year.toString()[3];
    formattedDate += buffer;
}

void DateTime::visit(FormatTypeTime * f){
    UNUSED(f);
    std::string hh = hour.toString();
    std::string min = minutes.toString();
    std::string sec = seconds.toString();;
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    if(min.length() == 1){ min = "0" + min ; }
    if(sec.length() == 1){ sec = "0" + sec ; }
    formattedDate += hh + ":" + min + ":" + sec;
}

void DateTime::visit(FormatTypeTimeAll * f){
    UNUSED(f);
    std::string hh = hour.toString();
    std::string min = minutes.toString();
    std::string sec = seconds.toString();;
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    if(min.length() == 1){ min = "0" + min ; }
    if(sec.length() == 1){ sec = "0" + sec ; }
    formattedDate += hh + ":" + min + ":" + sec + ".";
    formattedDate += milliseconds.toString() + "/" + microseconds.toString() + "/"+ nanoseconds.toString();
}

void DateTime::visit(FormatTypeTimeHour * f){
    UNUSED(f);
    std::string hh = hour.toString();
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    formattedDate += hh;
}
void DateTime::visit(FormatTypeTimeSecond * f){
    UNUSED(f);
    std::string sec = seconds.toString();;
    if(sec.length() == 1){ sec = "0" + sec ; }
    formattedDate += sec;
}
void DateTime::visit(FormatTypeTimeMinute * f){
    UNUSED(f);
    std::string min = minutes.toString();
    if(min.length() == 1){ min = "0" + min ; }
    formattedDate += min;
}
void DateTime::visit(FormatTypeTimeMilli * f){
    UNUSED(f);
    formattedDate += milliseconds.toString();
}
void DateTime::visit(FormatTypeTimeMicro * f){
    UNUSED(f);
    formattedDate += microseconds.toString();
}
void DateTime::visit(FormatTypeTimeNano * f){
    UNUSED(f);
    formattedDate += nanoseconds.toString();
}