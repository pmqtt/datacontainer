//
// Created by cefour on 16.05.22.
//

#include "date_time.h"
#include <locale>
#include <chrono>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>


date_time date_time::timestamp() {
    date_time dateTime;
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
void date_time::set_format(const std::string& format_spec){
    format_specifier spec = {
            {'Y',std::make_shared<format_type_year>()},
            {'y',std::make_shared<format_type_short_year>()},
            {'M',std::make_shared<format_type_readable_month>()},
            {'m',std::make_shared<format_type_month>()},
            {'d',std::make_shared<format_type_day>()},
            {'T',std::make_shared<format_type_time>()},
            {'A',std::make_shared<format_type_time_all>()},
            {'h',std::make_shared<format_type_time_hour>()},
            {'s',std::make_shared<format_type_time_second>()},
            {'i',std::make_shared<format_type_time_minute>()},
            {'l',std::make_shared<format_type_time_milli>()},
            {'c',std::make_shared<format_type_time_micro>()},
            {'n',std::make_shared<format_type_time_nano>()},

    };
    format f(format_spec, spec);
    this->fmt = f;
}

std::string date_time::to_string() const{

    auto format_list = this->fmt.get_formate_list();
    for(auto & x : format_list){
        x->handle(const_cast<date_time *>(this));
    }
    std::string buffer = formatted_date;
    formatted_date = "";
    return buffer;
}

void date_time::visit(format_type_month * f) {
    UNUSED(f);
    std::string v = month.to_string();
    if(v.length() == 1){
        v = "0"+v;
    }

    formatted_date += v;
}

void date_time::visit(format_type_readable_month * f){
    UNUSED(f);
    static std::map<std::string,std::string> monthConverter = {
            {"01","Jan"},{"02","Feb"},{"03","Mar"},
            {"04","Apr"},{"05","Mai"},{"06","Jun"},
            {"07","Jul"},{"08","Aug"},{"09","Sep"},
            {"10","Oct"},{"11","Nov"},{"12","Dez"}
    };
    std::string v = month.to_string();
    if(v.length() == 1){
        v = "0"+v;
    }

    formatted_date += monthConverter[v];
}

void date_time::visit(format_type_year * f){
    UNUSED(f);
    formatted_date += year.to_string();
}

void date_time::visit(format_type_day * f){
    UNUSED(f);
    formatted_date += day.to_string();
}
void date_time::visit(format_type_word * f){
    UNUSED(f);
    formatted_date += f->toAcceptedWord;
}
void date_time::visit(format_type_short_year * f){
    UNUSED(f);
    std::string buffer ="";
    buffer += year.to_string()[2];
    buffer += year.to_string()[3];
    formatted_date += buffer;
}

void date_time::visit(format_type_time * f){
    UNUSED(f);
    std::string hh = hour.to_string();
    std::string min = minutes.to_string();
    std::string sec = seconds.to_string();;
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    if(min.length() == 1){ min = "0" + min ; }
    if(sec.length() == 1){ sec = "0" + sec ; }
    formatted_date += hh + ":" + min + ":" + sec;
}

void date_time::visit(format_type_time_all * f){
    UNUSED(f);
    std::string hh = hour.to_string();
    std::string min = minutes.to_string();
    std::string sec = seconds.to_string();;
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    if(min.length() == 1){ min = "0" + min ; }
    if(sec.length() == 1){ sec = "0" + sec ; }
    formatted_date += hh + ":" + min + ":" + sec + ".";
    formatted_date += milliseconds.to_string() + "/" + microseconds.to_string() + "/" + nanoseconds.to_string();
}

void date_time::visit(format_type_time_hour * f){
    UNUSED(f);
    std::string hh = hour.to_string();
    if(hh.length() ==  1){ hh  = "0" + hh  ; }
    formatted_date += hh;
}
void date_time::visit(format_type_time_second * f){
    UNUSED(f);
    std::string sec = seconds.to_string();;
    if(sec.length() == 1){ sec = "0" + sec ; }
    formatted_date += sec;
}
void date_time::visit(format_type_time_minute * f){
    UNUSED(f);
    std::string min = minutes.to_string();
    if(min.length() == 1){ min = "0" + min ; }
    formatted_date += min;
}
void date_time::visit(format_type_time_milli * f){
    UNUSED(f);
    formatted_date += milliseconds.to_string();
}
void date_time::visit(format_type_time_micro * f){
    UNUSED(f);
    formatted_date += microseconds.to_string();
}
void date_time::visit(format_type_time_nano * f){
    UNUSED(f);
    formatted_date += nanoseconds.to_string();
}