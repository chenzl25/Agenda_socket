#include<iostream>
#include"Date.h"
#include<cstdlib>
using std:: string;
Date::Date() {
    year_ = 1000;
    month_ = 1;
    day_ = 1;
    hour_ = 0;
    minute_ = 0;
}
Date::Date(int y, int m, int d, int h, int mi) {
    year_ = y;
    month_ = m;
    day_ = d;
    hour_ = h;
    minute_ = mi;
}
int Date::getYear(void) const {
    return year_;
}
void Date::setYear(int year) {
    year_ = year;
}
int Date::getMonth(void) const {
    return month_;
}
void Date::setMonth(int month) {
    month_ = month;
}
int Date::getDay(void) const {
    return day_;
}
void Date::setDay(int day) {
    day_ = day;
}
int Date::getHour(void) const {
    return hour_;
}
void Date::setHour(int hour) {
    hour_ = hour;
}
int Date::getMinute(void) const {
    return minute_;
}
void Date::setMinute(int minute) {
    minute_ = minute;
}
bool Date::isValid(Date date) {
    int year_ = date.getYear();
    int month_ = date.getMonth();
    int day_ = date.getDay();
    int hour_ = date.getHour();
    int minute_ = date.getMinute();
    if (year_ > 0 && year_ <= 9999 && month_ > 0 && month_ <= 12 && day_ > 0 \
    && hour_ >= 0 && hour_ <= 23 && minute_ >= 0 && minute_ <= 59) {
        if (month_ == 1 || month_ == 3 || month_ == 5 || month_ == 7 \
        || month_ == 8 || month_ == 10 || month_ == 12) {
            if (day_ > 31) {
                return false;
            }
        } else {
            if (day_ > 30) {
                return false;
            }
        }
        if ((year_ % 4 == 0 && year_ % 100 != 0) || year_ % 400 == 0) {
            if (month_ == 2 && day_ > 29) {
                return false;
            }
        } else {
            if (month_ == 2 && day_ > 28) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}
Date Date::stringToDate(std::string dateString) {
    int y,m,d,h,mi;
    string s = dateString;
    string sy, sm, sd, sh, smi;
    sy = s.substr(0,4);
    sm = s.substr(5,2);
    sd = s.substr(8,2);
    sh = s.substr(11,2);
    smi = s.substr(14,2);
    y = atoi(sy.c_str());
    m = atoi(sm.c_str());
    d = atoi(sd.c_str());
    h = atoi(sh.c_str());
    mi = atoi(smi.c_str());
    Date res(y, m, d, h, mi);
    return res;
}
std::string Date::dateToString(Date date) {
    string res = "";
    int y,m,d,h,mi;
    string sy, sm, sd, sh, smi;
    y = date.getYear();
    m = date.getMonth();
    d = date.getDay();
    h = date.getHour();
    mi = date.getMinute();
    for (int i = 0; i < 4; i++) {
        int ry = 0;
        ry = y % 10;
        y /= 10;
        sy = static_cast<char>(ry + '0') + sy;
    }
    for (int i = 0; i < 2; i++) {
        int rm, rd, rh, rmi;
        rm = m % 10;
        rd = d % 10;
        rh = h % 10;
        rmi = mi % 10;
        m /= 10;
        d /= 10;
        h /= 10;
        mi /= 10;
        sm = static_cast<char>(rm + '0') + sm;
        sd = static_cast<char>(rd + '0') + sd;
        sh = static_cast<char>(rh + '0') + sh;
        smi = static_cast<char>(rmi + '0') + smi;
    }
    res = sy + '-' + sm + '-' + sd + '/' + sh + ':' + smi;
    return res;
}
Date &Date::operator=(const Date& date) {
    if (this == &date) {
        return *this;
    } else {
        year_ = date.year_;
        month_ = date.month_;
        day_ = date.day_;
        hour_ = date.hour_;
        minute_ = date.minute_;
        return *this;
    }
}
bool Date::operator==(const Date& date) const {
    if (year_ == date.year_ && month_ == date.month_ && day_ == date.day_ && hour_ == date.hour_ && minute_ == date.minute_) {
        return true;
    } else {
        return false;
    }
}
bool Date::operator>(const Date& date) const {
    if (year_ > date.year_) {
        return true;
    } else if (year_ < date.year_) {
        return false;
    } else {
        if (month_ > date.month_) {
            return true;
        } else if (month_ < date.month_) {
            return false;
        } else {
            if (day_ > date.day_) {
                return true;
            } else if (day_ < date.day_) {
                return false;
            } else {
                if (hour_ > date.hour_) {
                    return true;
                } else if (hour_ < date.hour_) {
                    return false;
                } else {
                    if (minute_ > date.minute_) {
                        return true;
                    } else if(minute_ < date.minute_) {
                        return false;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
}
bool Date::operator<(const Date& date) const {
    if (*this > date || *this == date) {
        return false;
    } else {
        return true;
    }
}
bool Date::operator>=(const Date& date) const {
    if (*this > date || *this == date) {
        return true;
    } else {
        return false;
    }
}
bool Date::operator<=(const Date& date) const {
    if (*this < date || *this == date) {
        return true;
    } else {
        return false;
    }
}

