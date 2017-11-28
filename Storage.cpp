#include "Storage.hpp"
#include "Path.hpp"
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>

using namespace std;

std::shared_ptr<Storage> Storage::m_instance = nullptr;


Storage::Storage(){
	readFromFile();
	m_dirty = false;
}

bool Storage::readFromFile(void){
	ifstream infile;
	infile.open(Path::userPath);
	if(infile){
		while(!infile.eof()){
			User temp;
			string name;
			string password;
			string email;
			string phone;
			string str;
			getline(infile,str,'\"');
			getline(infile,name,'\"');
			getline(infile,str,'\"');
			getline(infile,password,'\"');
			getline(infile,str,'\"');
			getline(infile,email,'\"');
			getline(infile,str,'\"');
			getline(infile,phone,'\"');
			getline(infile,str,'\n');
			temp.setName(name);
			temp.setPassword(password);
			temp.setEmail(email);
			temp.setPhone(phone);
			if(name!="")
				m_userList.push_back(temp);
		}
	}
	
	else{
		return false;
	}
	infile.close();
	infile.open(Path::meetingPath);
	if(infile){
		while(!infile.eof()){
			Meeting temp;
			string sponsor;
			string part;
			string str1,str2,str;
			string title;
			getline(infile,str,'\"');
			getline(infile,sponsor,'\"');
			getline(infile,str,'\"');
			getline(infile,part,'\"');
			string part1 = "";
			for(int i = 0;i < part.size();++i){
				if(part[i]!='&'){
					part1 += part[i];
				}
				else {
					temp.addParticipator(part1);
					part1 = "";
				}
				if(i == part.size()-1){
					temp.addParticipator(part1);
				}
			}
			getline(infile,str,'\"');
			getline(infile,str1,'\"');
			Date sta(str1);
			getline(infile,str,'\"');
			getline(infile,str2,'\"');
			Date end(str2);
			getline(infile,str,'\"');
			getline(infile,title,'\"');
			getline(infile,str,'\n');
			temp.setStartDate(sta);
			temp.setEndDate(end);
			temp.setSponsor(sponsor);
			temp.setTitle(title);
			if(sponsor!="")
				m_meetingList.push_back(temp);

		}
	}

	else {
		return false;
	}
	infile.close();
	return true;
}

bool Storage::writeToFile(void){
	ofstream outfile;
	outfile.open(Path::userPath);
	for(list<User>::iterator i=m_userList.begin();i!=m_userList.end();++i){
		outfile << "\"" << i->getName() << "\",\"" << i->getPassword() << "\",\"" << i->getEmail() << "\",\"" 
		<< i->getPhone() << "\"";
		auto i1 = i;
		if(++i1!=m_userList.end()){
			outfile << endl;
		}
	}
	outfile.close();
	outfile.open(Path::meetingPath);
	for(list<Meeting>::iterator i = m_meetingList.begin();i!=m_meetingList.end();++i){
		outfile << "\"" << i->getSponsor() << "\",\"";
		vector<string> v = i->getParticipator();
		for(vector<string>::iterator it = v.begin();it!=v.end();++it){
			outfile << (*it);
			if((it+1)!=v.end()){
				outfile << "&";
			}
		}
		outfile << "\",\"" << Date::dateToString(i->getStartDate()) << "\",\"" << Date::dateToString(i->getEndDate())
		<< "\",\"" << i->getTitle() << "\"";
		auto i1 = i;
		if(++i1!=m_meetingList.end()){
			outfile << endl;
		}
	}
	outfile.close();
	return true;
}

std::shared_ptr<Storage> Storage::getInstance(void){
	if(m_instance == nullptr){
		m_instance = (shared_ptr<Storage>)new Storage();
	}
	return m_instance;
}

Storage::~Storage(){
	writeToFile();
	m_userList.clear();
	m_meetingList.clear();
}

void Storage::createUser(const User &t_user){
	m_dirty = true;
	m_userList.push_back(t_user);
}

list<User> Storage::queryUser(std::function<bool(const User &t)> filter) const{
	list<User> temp;
	for(auto &i:m_userList){
		if(filter(i)){
			temp.push_back(i);
		}
	}
	return temp;
}
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &t)> filter) const{
	list<Meeting> temp;
	for(auto &i:m_meetingList){
		if(filter(i)){
			temp.push_back(i);
		}
	}
	return temp;
}

int Storage::updateUser(std::function<bool(const User &t)> filter,
                 std::function<void(User &s)> switcher){
	m_dirty = true;
	int num = 0;
	for(auto &i:m_userList){
		if(filter(i)){
			switcher(i);
			++num;
		}
	}
	return num;
}
int Storage::deleteUser(std::function<bool(const User &t)> filter){
	m_dirty = true;
	int num = 0;
	for(auto i = m_userList.begin();i!=m_userList.end();){
		if(filter(*i)){
			i = m_userList.erase(i);
			++num;
		}
		else{
			++i;
		}
	}
	return num;
}
void Storage::createMeeting(const Meeting &t_meeting){
	m_dirty = true;
	if(t_meeting.getStartDate() < t_meeting.getEndDate())
		m_meetingList.push_back(t_meeting);
}
int Storage::updateMeeting(std::function<bool(const Meeting &t)> filter,
                    std::function<void(Meeting &s)> switcher){
	m_dirty = true;
	int num = 0;
	for(auto &i:m_meetingList){
		if(filter(i)){
			switcher(i);
			++num;
		}
	}
	return num;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &t)> filter){
	m_dirty = true;
	int num = 0;
	for(auto iter = m_meetingList.begin();iter!=m_meetingList.end();){
		if(filter(*iter)){
			iter = m_meetingList.erase(iter);
			++num;
		}
		else{
			++iter;
		}
	}
	return num;
}


bool Storage::sync(void){
	if(m_dirty==true){
		writeToFile();
		m_dirty = false;
	}
	return true;
}