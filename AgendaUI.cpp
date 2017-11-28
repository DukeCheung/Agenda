#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "AgendaUI.hpp"

using namespace std;
void printMenu(){
	cout << "-------------------------------------Agenda-------------------------------------"<< endl
	     << "Action :" << endl
	     << "l    - log in Agenda by user name and password" << endl
	     << "r    - register an Agenda account" << endl
	     << "q    - quit Agenda" << endl 
	     << "--------------------------------------------------------------------------------" << endl << endl
	     << "Agenda :~$ ";
}
void printMenuLog(){
	cout << "[log in] [username] [password]" << endl
	     << "[log in] ";
}
void printMenuRegister(){
	cout << "[register] [username] [password] [email] [phone]" << endl
	     << "[register] ";
}
void printMenuOperation(string username){
	cout << "-------------------------------------Agenda-------------------------------------" << endl
	     << "Action :" << endl
	     << "o    - log out Agenda" << endl
	     << "dc   - delete Agenda account" << endl
	     << "lu   - list all Agenda user" << endl
	     << "cm   - create a meeting" << endl
	     << "amp  - add meeting participator" << endl
	     << "rqm  - request to quit meeting" << endl
	     << "la   - list all meetings" << endl
	     << "las  - list all sponsor meetings" << endl
	     << "lap  - list all participator meetings" << endl
	     << "qm   - query meeting by title" << endl
	     << "qt   - query meeting by time interval" << endl
	     << "dm   - delete meeting by title" << endl
	     << "da   - delete all meetings" << endl
	     << "--------------------------------------------------------------------------------" << endl << endl
	     << "Agenda@" << username << " :~# ";
}
AgendaUI::AgendaUI(){
	m_userName = "";
	m_userPassword = "";
	startAgenda();
}
void AgendaUI::startAgenda(){
	m_agendaService.startAgenda();
}
string AgendaUI::getOperation(){
	printMenu();
	string option;
	cin >> option;
	return option;
}

bool AgendaUI::executeOperation(const string &t_operation){
	if(t_operation!="q"){
		return true;
	}
	return false;
}
void AgendaUI::userLogIn(){
	printMenuLog();
	cin.sync();
	cin.clear();
	string name;
	string password;
	cin >> name >> password;
	int t = m_agendaService.userLogIn(name,password);
	if(t){
		cout << "[log in] succeed!" << endl;
		m_userName = name;
		m_userPassword = password;
	}
	else{
		cout << "[log in] Password error or user doesn't exist." << endl;
	}
	getchar();
}
void AgendaUI::userRegister(){
	printMenuRegister();
	cin.sync();
	cin.clear();
	string name;
	string password;
	string email;
	string phone;
	cin >> name >> password >> email >> phone;
	int t = m_agendaService.userRegister(name,password,email,phone);
	if(t){
		cout << "[register] succeed!" << endl;
	}
	else{
		cout << "[register] This username has been registered!" << endl;
	}
	getchar();
}
void AgendaUI::userLogOut(){
	m_userName = "";
	m_userPassword = "";
}
void AgendaUI::quitAgenda(){
	m_agendaService.quitAgenda();
}
void AgendaUI::deleteUser(){
	cout << "[delete agenda account] [please enter your password]" << endl
	     << "[delete agenda account] ";
	string pass;
	cin.sync();
	cin.clear();
	getline(cin,pass,'\n');
	if(pass == m_userPassword){
		m_agendaService.deleteUser(m_userName,m_userPassword);
		cout << endl << "[delete agenda account] succeed!" << endl;
	  
	}
	else{
		cout << endl << "[delete agenda account] fail!" << endl;
	}
}
void AgendaUI::listAllUsers(){
	list<User> user = m_agendaService.listAllUsers();
	cout << "[list all users]" << endl;
	cout << left << setw(10) << "name" << setw(15) << "email" << setw(13) << "phone" << endl << endl;
	for(auto i:user){
		cout << left << setw(10) << i.getName() << setw(15) << i.getEmail() << setw(13) << i.getPhone() << endl;
	}
}
void AgendaUI::createMeeting(){
	int number;
	vector<string> v;
	cin.sync();
	cin.clear();
	string str;
	cout << "[create meeting] [the number of participators]" << endl
	     << "[create meeting] ";
	stringstream s;
	string strnum;
	getline(cin,strnum,'\n');
	s << strnum;
	s >> number;

	for(int i = 0;i < number;++i){
		cout << "[create meeting] [please enter the participator " << i+1 << " ]" << endl
		     << "[create meeting] ";
		getline(cin,str,'\n');
		v.push_back(str);
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
	     << "[create meeting] ";
	string title,startTime,endTime;
	cin >> title >> startTime >> endTime;
	int is = m_agendaService.createMeeting(m_userName,title,startTime,endTime,v);
	if(is){
		cout << "[create meeting] succeed!" << endl;
	}
	else{
		cout << "[create meeting] error!" << endl;
	}
	getchar();

}
void AgendaUI::addMeetingParticipator(){
	cout << "[add participator] [meeting title] [participator username]" << endl
	     << "[add participator] ";

	string title,username;
	cin.sync();
	cin.clear();
	cin >> title >> username;
	int is = m_agendaService.addMeetingParticipator(m_userName,title,username);
	if(is){
		cout << "[add participator] succeed!" << endl;
	}
	else{
		cout << "[add participator] error!" << endl;
	}
	getchar();
}
void AgendaUI::removeMeetingParticipator(){
	cout << "[remove participator] [meeting title] [participator username]" << endl
	     << "[remove participator] ";
	string title,username;
	cin.sync();
	cin.clear();
	cin >> title >> username;
	int is = m_agendaService.removeMeetingParticipator(m_userName,title,username);
	if(is){
		cout << "[remove participator] succeed!" << endl;
	}
	else{
		cout << "[remove participator] error!" << endl;
	}
	getchar();
}
void AgendaUI::quitMeeting(){
	cout << "[quit meeting] [meeting title]" << endl
	     << "[quit meeting] ";
	string title;
	cin.sync();
	cin.clear();
	getline(cin,title,'\n');
	int is = m_agendaService.quitMeeting(m_userName,title);
	if(is){
		cout << "[quit meeting] succeed!" << endl;
	}
	else{
		cout << "[quit meeting] error!" << endl;
	}
}
void AgendaUI::listAllMeetings(){
	cout << "[list all meetings]" << endl << endl << left << setw(15)
	<< "title" << setw(15) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	list<Meeting> meeting = m_agendaService.listAllMeetings(m_userName);
	for(auto i:meeting){
		cout << left << setw(15) << i.getTitle() << setw(15) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
}
void AgendaUI::listAllSponsorMeetings(){
	cout << "[list all meetings]" << endl << endl << left << setw(15)
	<< "title" << setw(15) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	list<Meeting> meeting = m_agendaService.listAllSponsorMeetings(m_userName);
	for(auto i:meeting){
		cout << left << setw(15) << i.getTitle() << setw(15) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
}
void AgendaUI::listAllParticipateMeetings(){
	cout << "[list all participate meetings]" << endl << endl << left << setw(10)
	<< "title" << setw(10) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	list<Meeting> meeting = m_agendaService.listAllParticipateMeetings(m_userName);
	for(auto i:meeting){
		cout << left << setw(10) << i.getTitle() << setw(10) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
}
void AgendaUI::queryMeetingByTitle(){
	cout << "[query meetings] [title]" << endl
	<< "[query meetings] ";
	string title;
	cin.sync();
	cin.clear();
	getline(cin,title,'\n');
	cout << "[query meetings]" << endl;
	cout << left << setw(10) << "title" << setw(10) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	list<Meeting> meeting = m_agendaService.meetingQuery(m_userName,title);
	for(auto i:meeting){
		cout << left << setw(10) << i.getTitle() << setw(10) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
}
void AgendaUI::queryMeetingByTimeInterval(){
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
	<< "[query meetings] ";
	string sta,end;
	cin.sync();
	cin.clear();
	cin >> sta >> end;
	cout << "[query meetings]" << endl;
	cout << left << setw(10) << "title" <<setw(10) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	list<Meeting> meeting = m_agendaService.meetingQuery(m_userName,sta,end);
	for(auto i:meeting){
		cout << left << setw(10) << i.getTitle() << setw(10) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
	getchar();

}
void AgendaUI::deleteMeetingByTitle(){
	cout << "[delete meeting] [title]" << endl
	<< "[delete meeting] ";
	cin.sync();
	cin.clear();
	string title;
	getline(cin,title,'\n');
	int is = m_agendaService.deleteMeeting(m_userName,title);
	if(is){
		cout << "[delete meeting] succeed!" << endl;
	}
	else{
		cout << "[delete meeting] error!" << endl;
	}
}
void AgendaUI::deleteAllMeetings(){
	int is = m_agendaService.deleteAllMeetings(m_userName);
	if(is){
		cout << "[delete all meeting] succeed!" << endl;
	}
	else{
		cout << "[delete all meeting] error!" << endl;
	}
}
void AgendaUI::printMeetings(const list<Meeting> &t){
	cout << left << setw(10)<<"title" << setw(10) << "sponsor" << setw(20) << "start time" << setw(20) << "end time"
	<< setw(30) << "participator" << endl << endl;
	for(auto i:t){
		cout << left << setw(10) << i.getTitle() << setw(10) <<i.getSponsor() << setw(20) << Date::dateToString(i.getStartDate())
		<< setw(20) << Date::dateToString(i.getEndDate());
		vector<string> v = i.getParticipator();
		for (auto j = v.begin();j!=v.end();++j)
		{
			cout << (*j);
			auto k = j;
			if(++k!=v.end()){
				cout << ",";
			}
		}
		cout << endl;
	}
}

void AgendaUI::OperationLoop(){
	while(1){
		printMenu();
		cin.sync();
		cin.clear();
	    string option;
	    getline(cin,option,'\n');
	    cout << endl;
	    if(option == "l"){
	    	userLogIn();
	    	while(m_userName != ""){

		        printMenuOperation(m_userName);
		        string option2;
		        cin.sync();
		        cin.clear();
		        getline(cin,option2,'\n');
		    
			    if(option2 == "o"){
			    	userLogOut();
			    	break;
			    }
			    else if(option2 == "dc"){
			    	deleteUser();
			    	userLogOut();
			    	break;
			    }
			    else if(option2 == "lu"){
			    	listAllUsers();
			    }
			    else if(option2 == "cm"){
			    	createMeeting();
			    }
			    else if(option2 == "amp"){
			    	addMeetingParticipator();
			    }
			    else if(option2 == "rmp"){
			    	removeMeetingParticipator();
			    }
			    else if(option2 == "rqm"){
			    	quitMeeting();
			    }
			    else if(option2 == "la"){
			    	listAllMeetings();
			    }
			    else if(option2 == "las"){
			    	listAllSponsorMeetings();
			    }
			    else if(option2 == "lap"){
			    	listAllParticipateMeetings();
			    }
			    else if(option2 == "qm"){
			    	queryMeetingByTitle();
			    }
			    else if(option2 == "qt"){
			    	queryMeetingByTimeInterval();
			    }
			    else if(option2 == "dm"){
			    	deleteMeetingByTitle();
			    }
			    else if(option2 == "da"){
			    	deleteAllMeetings();
			    }
		    }
	    }
	    else if(option == "r"){
	    	userRegister();
	    }
	    else if(option == "q"){
	    	quitAgenda();
	    	break;
	    }
	}
}