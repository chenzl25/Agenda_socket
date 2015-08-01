#include"AgendaService.h"
#include<cctype>
using namespace std;
AgendaService::AgendaService() {
    storage_ = NULL;
}
AgendaService::~AgendaService() {

}
bool AgendaService::userLogIn(std::string userName, std::string password) {
    function<bool(const User&)> f = [](const User&) {return true;};
    list<User> l = storage_->queryUser(f);
    for (list<User>::iterator it = l.begin(); it != l.end(); it++) {
        if (it->getName() == userName && it->getPassword() == password) {
            return true;
        }
    }
    return false;
}
bool AgendaService::userRegister(std::string userName, std::string password, std::string email, std::string phone) {
    function<bool(const User&)> f = [](const User&) {return true;};
    list<User> l = storage_->queryUser(f);
    for (list<User>::iterator it = l.begin(); it != l.end(); it++) {
        if (it->getName() == userName) {
            return false;
        }
    }
    storage_->createUser(User(userName, password, email, phone));
    storage_->sync();
    return true;
}
bool AgendaService::deleteUser(std::string userName, std::string password) {
    function<bool(const User&)> f = [userName, password](const User &x) {if(x.getName() == userName && x.getPassword() == password) return true; else return false;};
    if (storage_->deleteUser(f)) {
        deleteAllMeetings(userName);
        storage_->sync();
        return true;
    } else {
        return false;
    }
}
// a user can only delete itself
std::list<User> AgendaService::listAllUsers(void) {
    function<bool(const User&)> f = [](const User &x) {return true;};
    return storage_->queryUser(f);
}
bool stringIsOk(std::string s) {
    if (s.length() != 16) {
        return false;
    }
    int pos[12] ={0,1,2,3,5,6,8,9,11,12,14,15};
    for (int i = 0; i < 12; i++) {
        if (!isdigit(s[pos[i]])) {
            return false;
        }
    }
    if (s[4] != '-' || s[7] != '-' || s[10] != '/' || s[13] != ':') {
        return false;
    }
    Date d = Date::stringToDate(s);
    if (!Date::isValid(d)) {
        return false;
    }
    return true;
}
bool AgendaService::createMeeting(std::string userName, std::string title,
std::string participator,
std::string startDate, std::string endDate) {
    if (!stringIsOk(startDate) || !stringIsOk(endDate)) {
        return false;
    }
    Date d1 = Date::stringToDate(startDate);
    Date d2 = Date::stringToDate(endDate);
    if (d1 >= d2) {
        return false;
    }
    list<User> l = listAllUsers();
    for (list<User>::iterator it = l.begin(); it != l.end(); it++) {
        if (it->getName() == participator) {
            list<Meeting> lp = listAllMeetings(participator);
            list<Meeting> ls = listAllMeetings(userName);
            for (list<Meeting>::iterator i = lp.begin(); i != lp.end(); i++) {
                Date sd, ed;
                sd = i->getStartDate();
                ed = i->getEndDate();
                if (sd <= d1 && ed >= d2) {
                    return false;
                } else if (d1 <= sd && d2 >= ed) {
                    return false;
                } else if (d1 <= sd && d2 <= ed && sd < d2) {
                    return false;
                } else if (sd <= d1 && ed <= d2 && d1 < ed) {
                    return false;
                }
            }
            for (list<Meeting>::iterator i = ls.begin(); i != ls.end(); i++) {
                Date sd, ed;
                sd = i->getStartDate();
                ed = i->getEndDate();
                if (sd <= d1 && ed >= d2) {
                    return false;
                } else if (d1 <= sd && d2 >= ed) {
                    return false;
                } else if (d1 <= sd && d2 <= ed && sd < d2) {
                    return false;
                } else if (sd <= d1 && ed <= d2 && d1 < ed) {
                    return false;
                }
            }
            storage_->createMeeting(Meeting(userName, participator, d1, d2, title));
            storage_->sync();
            return true;
        }
    }
    return false;
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string title) {
    function<bool(const Meeting&)> f = [userName, title](const Meeting &x) {if ((x.getSponsor() == userName|| x.getParticipator() == userName) && x.getTitle() == title) return true; else return false;};
    return storage_->queryMeeting(f);
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string startDate,
std::string endDate) {
    if (!stringIsOk(startDate) || !stringIsOk(endDate)) {
        return list<Meeting>(); // when the Date is illegal wo should do what ?
    }
    Date d1 = Date::stringToDate(startDate);
    Date d2 = Date::stringToDate(endDate);
    
    function<bool(const Meeting&)> f = [userName, d1, d2](const Meeting &x) {if ((x.getSponsor() == userName || x.getParticipator() == userName) && ((x.getStartDate() >= d1 && x.getStartDate() <= d2) || (x.getEndDate() >= d1 && x.getEndDate() <= d2))) return true; else return false;};
    return storage_->queryMeeting(f);
}
std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
    function<bool(const Meeting&)> f = [userName](const Meeting &x) {if (x.getSponsor() == userName || x.getParticipator() == userName) return true; else return false;};
    return storage_->queryMeeting(f);
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
    function<bool(const Meeting&)> f = [userName](const Meeting &x) {if (x.getSponsor() == userName) return true; else return false;};
    return storage_->queryMeeting(f);
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(std::string userName) {
    function<bool(const Meeting&)> f = [userName](const Meeting &x) {if (x.getParticipator() == userName) return true; else return false;};
    return storage_->queryMeeting(f);
}
bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    function<bool(const Meeting&)> f = [userName, title](const Meeting &x) {if ((x.getSponsor() == userName || x.getParticipator() == userName) && x.getTitle() == title) return true; else return false;};
    if (storage_->deleteMeeting(f) > 0) {
        storage_->sync();
        return true;
    } else {
        return false;
    }
}
bool AgendaService::deleteAllMeetings(std::string userName) {
    function<bool(const Meeting&)> f = [userName](const Meeting &x) {if (x.getSponsor() == userName || x.getParticipator() == userName) return true; else return false;};
    if (storage_->deleteMeeting(f) > 0) {
        storage_->sync();
        return true;
    } else {
        return false;
    }
}

void AgendaService::startAgenda(void) {
    storage_ = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
    if(storage_ != NULL) {
        storage_->sync();
        delete storage_;
    }
}

