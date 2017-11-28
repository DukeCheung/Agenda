#include "Date.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Date::Date(){
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}
Date::Date(const std::string &dateString){
	(*this) = Date::stringToDate(dateString);
}

int Date::getYear(void) const{
	return m_year;
}
void Date::setYear(const int t_year){
	m_year = t_year;
}
int Date::getMonth(void) const{
	return m_month;
}
void Date::setMonth(const int t_month){
	m_month = t_month;
}
int Date::getDay(void) const{
	return m_day;
}
void Date::setDay(const int t_day){
	m_day = t_day;
}
int Date::getHour(void) const{
	return m_hour;
}
void Date::setHour(const int t_hour){
	m_hour = t_hour;
}
int Date::getMinute(void) const{
	return m_minute;
}
void Date::setMinute(const int t_minute){
	m_minute = t_minute;
}
Date Date::stringToDate(const std::string &dateString){
	Date temp;
	Date empty;
	int num = 0,n = 0;
	if(dateString.size()!=16)
		return empty;
	if(dateString[4]!='-'||dateString[7]!='-'||dateString[10]!='/'||dateString[13]!=':'){
		return empty;
	}
	int t = 0;
	int sum = 0;
	for(int i = 0;i < 4;++i){
		if(dateString[i]>='0'&&dateString[i]<='9'){
			sum = sum*10 + dateString[i]-'0';
		}
		else
			return empty;
	}
	t += 4;
	temp.m_year = sum;
	sum = 0;
	if(dateString[t++]!='-')
		return empty;
	for(int i = t;i < t+2;++i){
		if(dateString[i]>='0'&&dateString[i]<='9'){
			sum = sum*10 + dateString[i]-'0';
		}
		else
			return empty;
	}
	temp.m_month = sum;
	t+=2;
	sum = 0;
	if(dateString[t++]!='-')
		return empty;
	for(int i = t;i < t+2;++i){
		if(dateString[i]>='0'&&dateString[i]<='9'){
			sum = sum*10 + dateString[i]-'0';
		}
		else
			return empty;
	}
	temp.m_day = sum;
	t += 2;
	sum = 0;
	if(dateString[t++]!='/')
		return empty;
	for(int i = t;i < t+2;++i){
		if(dateString[i]>='0'&&dateString[i]<='9'){
			sum = sum*10 + dateString[i]-'0';
		}
		else
			return empty;
	}
	temp.m_hour = sum;
	sum = 0;
	t += 2;
	if(dateString[t++]!=':')
		return empty;
	for(int i = t;i < t+2;++i){
		if(dateString[i]>='0'&&dateString[i]<='9'){
			sum = sum*10 + dateString[i]-'0';
		}
		else
			return empty;
	}
	temp.m_minute = sum;
	return temp;
}
bool Date::isValid(const Date &t_date){
	if(!(t_date.m_year<=9999&&t_date.m_year>=1000&&t_date.m_month<=12&&t_date.m_month>=1&&
		t_date.m_day<=31&&t_date.m_day>=1&&t_date.m_hour<=23&&t_date.m_hour>=0&&t_date.m_minute<=59&&
		t_date.m_minute>=0)){
		return false;
	}
	if(t_date.m_month == 2){
		if((t_date.m_year%100!=0&&t_date.m_year%4==0)||t_date.m_year%400==0){
			if(t_date.m_day>29){
				return false;
			}
		}
		else{
			if(t_date.m_day>28)
				return false;
		}
	}
	else if(t_date.m_month==4||t_date.m_month==6||t_date.m_month==9||t_date.m_month==11){
		if(t_date.m_day > 30){
			return false;
		}
	}
	return true;
}
std::string Date::dateToString(const Date &t_date){
	stringstream s;
	string str;
	if(!isValid(t_date)){
		s << "0000-00-00/00:00";
		s >> str;
		return str;
	}
	s << t_date.m_year << "-";
	if(t_date.m_month < 10){
		s << "0";
	}
	s << t_date.m_month << "-";
	if(t_date.m_day < 10){
		s << "0";
	}
	s << t_date.m_day << "/";
	if(t_date.m_hour < 10){
		s << "0";
	}
	s << t_date.m_hour << ":";
	if(t_date.m_minute < 10){
		s << "0";
	}
	s << t_date.m_minute;
	s >> str;
	return str;
}
Date& Date::operator=(const Date &t_date){
		m_year = t_date.m_year;
		m_month = t_date.m_month;
		m_day = t_date.m_day;
		m_hour = t_date.m_hour;
		m_minute = t_date.m_minute;
	return *this;
}
bool Date::operator==(const Date &t_date) const{
	if (m_year!=t_date.m_year){
		return false;
	}
	if(m_month!=t_date.m_month){
		return false;
	}
	if(m_day!=t_date.m_day){
		return false;
	}
	if(m_hour!=t_date.m_hour){
		return false;
	}
	if(m_minute!=t_date.m_minute){
		return false;
	}
	return true;
}
bool Date::operator>(const Date &t_date) const{
	if (m_year>t_date.m_year){
		return true;
	}
	if (m_year<t_date.m_year){
		return false;
	}
	if(m_month>t_date.m_month){
		return true;
	}
	if(m_month<t_date.m_month){
		return false;
	}
	if(m_day>t_date.m_day){
		return true;
	}
	if(m_day<t_date.m_day){
		return false;
	}
	if(m_hour>t_date.m_hour){
		return true;
	}
	if(m_hour<t_date.m_hour){
		return false;
	}
	if(m_minute>t_date.m_minute){
		return true;
	}
	return false;
}
bool Date::operator<(const Date &t_date) const{
	if (m_year<t_date.m_year){
		return true;
	}
	if (m_year>t_date.m_year){
		return false;
	}
	if(m_month<t_date.m_month){
		return true;
	}
	if(m_month>t_date.m_month){
		return false;
	}
	if(m_day<t_date.m_day){
		return true;
	}
	if(m_day>t_date.m_day){
		return false;
	}
	if(m_hour<t_date.m_hour){
		return true;
	}
	if(m_hour>t_date.m_hour){
		return false;
	}
	if(m_minute<t_date.m_minute){
		return true;
	}
	return false;
}
bool Date::operator>=(const Date &t_date) const{
	if (m_year>t_date.m_year){
		return true;
	}
	if (m_year<t_date.m_year){
		return false;
	}
	if(m_month>t_date.m_month){
		return true;
	}
	if(m_month<t_date.m_month){
		return false;
	}
	if(m_day>t_date.m_day){
		return true;
	}
	if(m_day<t_date.m_day){
		return false;
	}
	if(m_hour>t_date.m_hour){
		return true;
	}
	if(m_hour<t_date.m_hour){
		return false;
	}
	if(m_minute>=t_date.m_minute){
		return true;
	}
	return false;
}
bool Date::operator<=(const Date &t_date) const{
	if (m_year<t_date.m_year){
		return true;
	}
	if (m_year>t_date.m_year){
		return false;
	}
	if(m_month<t_date.m_month){
		return true;
	}
	if(m_month>t_date.m_month){
		return false;
	}
	if(m_day<t_date.m_day){
		return true;
	}
	if(m_day>t_date.m_day){
		return false;
	}
	if(m_hour<t_date.m_hour){
		return true;
	}
	if(m_hour>t_date.m_hour){
		return false;
	}
	if(m_minute<=t_date.m_minute){
		return true;
	}
	return false;
}