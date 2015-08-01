#include"Storage.h"
#include<fstream>
#include<iostream>
#include<cstdlib>
using namespace std;
// static Storage *instance_;
//std::list<User> userList_;
//std::list<Meeting> meetingList_;
Storage *Storage::instance_ = NULL;
Storage::Storage(const Storage &x) {
    userList_.assign(x.userList_.begin(), x.userList_.end());
    meetingList_.assign(x.meetingList_.begin(), x.meetingList_.end());
}
void Storage::operator=(const Storage &x) {
    if (this == &x) {
        return;
    } else {
        userList_.clear();
        meetingList_.clear();
        userList_.assign(x.userList_.begin(), x.userList_.end());
        meetingList_.assign(x.meetingList_.begin(), x.meetingList_.end());
        return;
    }
}
Storage::Storage() {
    userList_.clear();
    meetingList_.clear();
}
bool Storage::readFromFile(const char *fpath) {
    string f1 = fpath+string("User.csv");
    string f2 = fpath+string("Meeting.csv");
    ifstream user_in(f1.c_str());
    ifstream meeting_in(f2.c_str());
    if (!user_in || !meeting_in) {
        return false;
    }
    string s = "";
    getline(user_in, s);
    while(getline(user_in, s)) {
        if(s.empty()) {
            break;
        }
        int pos[3];
        int cnt = 0;
        for (int j = 0, k = 0; j < s.length(); j++) {
            if (s[j] == '\"') {
                cnt++;
            }
            if (s[j] == ',' && cnt % 2 == 0) {
                pos[k++] = j;
            }
        }
        string name, password, email, phone;
        name = s.substr(1, pos[0]-2);
        password = s.substr(pos[0]+2, pos[1]-pos[0]-3);
        email = s.substr(pos[1]+2, pos[2]-pos[1]-3);
        phone = s.substr(pos[2]+2, s.length() - pos[2]-3);
        User u(name, password, email, phone);
        userList_.push_back(u);
    }
    getline(meeting_in,s);
    while(getline(meeting_in, s)) {
        if(s.empty()) {
            break;
        }
        int pos[4];
        int cnt = 0;
        for (int j = 0, k = 0; j < s.length(); j++) {
            if (s[j] == '\"') {
                cnt++;
            }
            if (s[j] == ',' && cnt % 2 == 0) {
                pos[k++] = j;
            }
        }
        string sponsor, participator, sdate, edate, title;
        sponsor = s.substr(1, pos[0]-2);
        participator = s.substr(pos[0]+2, pos[1]-pos[0]-3);
        sdate = s.substr(pos[1]+2, pos[2]-pos[1]-3);
        edate = s.substr(pos[2]+2, pos[3]-pos[2]-3);
        title = s.substr(pos[3]+2, s.length() - pos[3]-3);
        Meeting m(sponsor, participator, Date::stringToDate(sdate), Date::stringToDate(edate), title);
        meetingList_.push_back(m);
    }
    user_in.close();
    meeting_in.close();
    return true;
}
bool Storage::writeToFile(const char *fpath) {
    string f1 = fpath+string("User.csv");
    string f2 = fpath+string("Meeting.csv");
    ofstream user_out(f1.c_str());
    ofstream meeting_out(f2.c_str());
    if (!user_out || !meeting_out) {
        return false;
    }
    user_out << "\"name\",\"password\",\"email\",\"phone\"" << std::endl;
    for (list<User>::iterator it = userList_.begin(); it != userList_.end(); it++) {
        user_out << '\"' << it->getName() << '\"' << "," << '\"' << it->getPassword()  << '\"' << ","  << '\"' << it->getEmail()  << '\"' << ","  << '\"' << it->getPhone()  << '\"' << std::endl;
    }
    meeting_out << "\"sponsor\",\"participator\",\"sdate\",\"edate\",\"title\"" << std::endl;
    for (list<Meeting>::iterator it = meetingList_.begin(); it != meetingList_.end(); it++) {
        meeting_out << '\"' << it->getSponsor()  << '\"' << "," << '\"' << it->getParticipator()  << '\"' << "," << '\"' << 
        Date::dateToString(it->getStartDate()) << '\"' << ","  << '\"' \
        << Date::dateToString(it->getEndDate())  << '\"' << ","  << '\"' << it->getTitle() << '\"'<< std::endl; 
    }
    user_out.close();
    meeting_out.close();
    return true;
}
// above is private memberfunction
/*static */
Storage * Storage::getInstance(void) {
    instance_ = new Storage();
    instance_->readFromFile("");
    return instance_;
}
Storage::~Storage() {
    userList_.clear();
    meetingList_.clear();
}
void Storage::createUser(const User& x) {
    instance_->userList_.push_back(x);
}
std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    std::list<User> res;
    for (list<User>::iterator it = instance_->userList_.begin(); it != instance_->userList_.end(); it++) {
        if (filter(*it) == true) {
            res.push_back(*it);
        }
    }
    return res;
}
int Storage::updateUser(std::function<bool(const User&)> filter, std::function<void(User&)> switcher) {
    int ans = 0;
    for (list<User>::iterator it = instance_->userList_.begin(); it != instance_->userList_.end(); it++) {
        if (filter(*it) == true) {
            switcher(*it);
            ans++;
        }
    }
    return ans;
}
int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int ans = 0;
    for (list<User>::iterator it = instance_->userList_.begin(); it != instance_->userList_.end();) {
        if (filter(*it) == true) {
            it = instance_->userList_.erase(it);
            ans++;
        } else {
            it++;
        }
    }
    return ans;
}
void Storage::createMeeting(const Meeting& x) {
    instance_->meetingList_.push_back(x);
}
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting&)> filter) {
    std::list<Meeting> res;
    for (list<Meeting>::iterator it = instance_->meetingList_.begin(); it != instance_->meetingList_.end(); it++) {
        if (filter(*it) == true) {
            res.push_back(*it);
        }
    }
    return res;
}
int Storage::updateMeeting(std::function<bool(const Meeting&)> filter, std::function<void(Meeting&)> switcher) {
    int ans = 0;
    for (list<Meeting>::iterator it = instance_->meetingList_.begin(); it != instance_->meetingList_.end(); it++) {
        if (filter(*it) == true) {
            switcher(*it);
            ans++;
        }
    }
    return ans;
}
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int ans = 0;
    for (list<Meeting>::iterator it = instance_->meetingList_.begin(); it != instance_->meetingList_.end();) {
        if (filter(*it) == true) {
            it = instance_->meetingList_.erase(it);
            ans++;
        } else {
            it++;
        }
    }
    return ans;
}
bool Storage::sync(void) {
    instance_->writeToFile("");
}

