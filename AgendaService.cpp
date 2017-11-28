#include <list>
#include <string>
#include <vector>

#include "AgendaService.hpp"

using namespace std;

 AgendaService::AgendaService(){
  startAgenda();
 }

 AgendaService::~AgendaService(){
  quitAgenda();
 }
  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
  bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
   list<User> i = m_storage->queryUser([=](const User &t)->bool{
    if(t.getName() == userName&&t.getPassword()==password){
     return true;
    }
      else{
        return false;
      }
   });
   if(i.size() == 1){
    return true;
   }
   return false;
  }

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
  bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone){
   
   list<User> i = m_storage->queryUser([=](const User& t)->bool{
    if(t.getName()==userName){
     return true;
    }
      else{
        return false;
      }
   });
   if(i.size()==0){
     User user(userName,password,email,phone);
     m_storage->createUser(user);
     return true;
   }
   return false;
  }

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
  bool AgendaService::deleteUser(const std::string &userName, const std::string &password){

    list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.isParticipator(userName)){
        return true;
      }
      return false;
    });
    if(meeting.size()!=0){
      for(auto i:meeting){
         quitMeeting(userName,i.getTitle());
      }
    }
    list<Meeting> meeting1 = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getSponsor() == userName){
        return true;
      }
      return false;
    });
    if(meeting1.size()!=0){
      deleteAllMeetings(userName);
    }
    int te = m_storage->deleteUser([=](const User &t)->bool{
      if(t.getName() == userName){
        return true;
      }
      else
        return false;
    });
    if(te == 1)
      return true;
    else 
      return false;
  }

  /**
   * list all users from storage
   * @return a user list result
   */
  std::list<User> AgendaService::listAllUsers(void) const{
   
   list<User> user = m_storage->queryUser([=](const User &t)->bool{return true;});
   return user;
  }

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
   Date sta(startDate);
   Date end(endDate);
  
    if((!Date::isValid(sta))||(!Date::isValid(end)))
      return false;
    if(sta >= end)
      return false;

    list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.getTitle() == title){
     return true;
    }
    else
        return false;
   });
   if(meeting.size()!=0)
    return false;

     list<User> numberOfSponsor = m_storage->queryUser([=](const User &t)->bool{
      if(t.getName() == userName){
        return true;
      }
      else
        return false;
    });
    if(numberOfSponsor.size() == 0)
      return false;

    for(auto i:participator){
      if(i == userName){
        return false;
      }
    }


    for(auto i:participator){
      list<User> numberOfParticipator = m_storage->queryUser([=](const User &t)->bool{
      if(t.getName() == i){
        return true;
      }
      else
        return false;
      });
      if(numberOfParticipator.size() == 0){
        return false;
      }
    }

   

    for(int i = 0;i < participator.size();++i){
      for(int j = 0;j < participator.size();++j){
        if((i!=j)&&(participator[i] == participator[j])){
          return false;
        }
      }
    }


   list<Meeting> meetingSponsor = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.getSponsor() == userName||t.isParticipator(userName)){
       return true;
    }
    else
        return false;
   });

    for(auto i:meetingSponsor){
      Date d1 = i.getStartDate();
      Date d2 = i.getEndDate();
      if(!(end <= d1 || sta >= d2)){
        return false;
      }
    }

    for(auto i:participator){
      list<Meeting> meetingPar = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getSponsor() == i||t.isParticipator(i)){
        return true;
      }
      else
        return false;
      });
      for(auto j:meetingPar){
        Date d3 = j.getStartDate();
        Date d4 = j.getEndDate();
        if(!(end <= d3 || sta >= d4)){
            return false;
        }
      }
    }

    Meeting meet(userName,participator,startDate,endDate,title);
    m_storage->createMeeting(meet);
    return true;
  }

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator){
    if(userName == participator)
      return false;

    list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.getSponsor() == userName&&t.getTitle() == title){
     return true;
    }
    else
        return false;
   });
   if(meeting.size()==0)
    return false;

    Date sta, end;
    for(auto i:meeting){
      sta = i.getStartDate();
      end = i.getEndDate();
      break;
    }

   list<User> user = m_storage->queryUser([=](const User &t)->bool{
    if(t.getName() == participator){
     return true;
    }
      else
        return false;
   });
   if(user.size() == 0)
    return false;


    list<Meeting> meeting1 = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getTitle() == title&&!t.isParticipator(participator)){
        return true;
      }
      else
        return false;
    });
    if(meeting1.size()==0){
      return false;
    }

    list<Meeting> meetingPar = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getSponsor() == participator||t.isParticipator(participator)){
        return true;
      }
      else
        return false;
    });
    if(meetingPar.size()!=0){
      for(auto j:meetingPar){
      Date d3 = j.getStartDate();
      Date d4 = j.getEndDate();

      if(!(end <= d3||sta >= d4)){
        return false;
      }
      }
    }
    

    int i = m_storage->updateMeeting([=](const Meeting &t)->bool{
      if(t.getSponsor() == userName&&t.getTitle() == title&&!t.isParticipator(participator)){
        return true;
      }
      else
        return false;
    },[=](Meeting &s){s.addParticipator(participator);});
    if(i!=0){
      return true;
    }
    return false;
  }
   

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator){

     if(userName == participator)
      return false;

     bool i = quitMeeting(participator,title);
     if(i == true){
       return true;
     }
     return false;
  }

  /**
   * quit from a meeting
   * @param userName the current userName. no need to be the sponsor
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
     list<User> numberOfSponsor = m_storage->queryUser([=](const User &t)->bool{
      if(t.getName() == userName){
        return true;
      }
      else
        return false;
    });
    if(numberOfSponsor.size() == 0)
      return false;


    list<Meeting> meet = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getTitle() == title){
        return true;
      }
      else
        return false;
    });
    if(meet.size() == 0)
      return false;

   int i = m_storage->updateMeeting([=](const Meeting &t)->bool{
    if(t.getTitle()== title&&t.isParticipator(userName)){
     return true;
    }
    return false;
   },[=](Meeting &s){s.removeParticipator(userName);});

   int meeting = m_storage->deleteMeeting([=](const Meeting &t)->bool{
    if(t.getParticipator().size() == 0){
     return true;
    }
    return false;
   });

   if(i==0){
    return false;
   }
   return true;
  }

  /**
   * search a meeting by username and title
   * @param uesrName the sponsor's userName
   * @param title the meeting's title
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const{

   list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.getTitle()==title&&(t.getSponsor() == userName||t.isParticipator(userName))){
     return true;
    }
    else
        return false;
   });
   return meeting;
  }
  /**
   * search a meeting by username, time interval
   * @param uesrName the sponsor's userName
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
    Date sta(startDate);
    Date end(endDate);
    list<Meeting> temp;

    if((!Date::isValid(sta))||(!Date::isValid(end)))
      return temp;
    if(sta > end)
      return temp;
    
   list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if((t.getSponsor() == userName||t.isParticipator(userName))&&(!(end < t.getStartDate()||sta > t.getEndDate()))){
         return true;
    }
    else
        return false;
   });
   return meeting;
  }

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
   
   list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
     if(t.isParticipator(userName)||t.getSponsor() == userName){
     return true;
    }
    else
        return false;
   });
   return meeting;
  }

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
   list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.getSponsor() == userName){
     return true;
    }
    else
        return false;
   });
   return meeting;
  }

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const{
   list<Meeting> meeting = m_storage->queryMeeting([=](const Meeting &t)->bool{
    if(t.isParticipator(userName)){
     return true;
    }
    else
        return false;
   });
   return meeting;
  }

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
     list<User> numberOfSponsor = m_storage->queryUser([=](const User &t)->bool{
      if(t.getName() == userName){
        return true;
      }
      else
        return false;
    });
    if(numberOfSponsor.size() == 0)
      return false;

    list<Meeting> meet = m_storage->queryMeeting([=](const Meeting &t)->bool{
      if(t.getTitle() == title){
        return true;
      }
      return false;
    });
    if(meet.size() == 0){
      return false;
    }

   int i = m_storage->deleteMeeting([=](const Meeting &t)->bool{
    if((t.getSponsor() == userName&&t.getTitle() == title)){
     return true;
    }
    else
        return false;
   });
   return true;
  }

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const std::string &userName){
    list<User> numberOfSponsor = m_storage->queryUser([=](const User &t)->bool{
      if(t.getName() == userName){
        return true;
      }
      else
        return false;
    });
    if(numberOfSponsor.size() == 0)
      return false;
   
   int i = m_storage->deleteMeeting([=](const Meeting &t)->bool{
    if(t.getSponsor() == userName){
     return true;
    }
    else
        return false;
   });
   return true;
  }

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda(void){
   m_storage = Storage::getInstance();

  }

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda(void){
   m_storage->sync();
  }