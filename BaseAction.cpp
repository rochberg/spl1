#include "Action.h"
#include "Restaurant.h"     //added header
#include <iostream>
#include <Action.h>


extern Restaurant* backup;

//*******************************
//BaseAction class
BaseAction::BaseAction():status(PENDING) {
}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status=COMPLETED;
}




void BaseAction::error(std::string errorMsg){
    this->errorMsg = errorMsg;
    this->status=ERROR;
    std::cout <<"Error: "<< getErrorMsg() << std::endl;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}



//*********************************************************

//OpenTable class
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):
        tableId(id),BaseAction(){
    std::vector <Customer*> customers(customersList);
}

void OpenTable::act(Restaurant &rest){
    Table* table=rest.getTable(tableId);
    if((table) && !table->isOpen() && table->getCapacity()>=customers.size()){
        table->open = true;
        for(auto &c:customers)
            table->customersList.push_back(c);
    }
    else
        error("Table does not exist or is already open");
}

std::string OpenTable::toString() const{ //NEED TO DO
    std::cout <<"request-status for opening table number " << tableId <<" is:   *** "<< getStatus()<<"   ***\n"<< std::endl;
}

OpenTable::~OpenTable(){
    for(Customer* c:customers) {
        c= nullptr;
    }
}





//******************************************************

//Close class
Close::Close(int id):BaseAction(),tableId(id){
}

void Close::act(Restaurant &restaurant){
    Table* TtoC=restaurant.getTable(tableId);
    if(TtoC &&TtoC->isOpen()) {
        TtoC->closeTable();
        std::cout << "Table "<< tableId << " was closed. Bill " << TtoC->getBill() << "NIS" << std::endl;
        TtoC->reOpen();
    }
    else error("Table does not exist or is not open");
}


std::string Close::toString() const{
    std::cout <<"request-status for closing table number " << tableId <<" is:   *** "<< getStatus()<<"   ***\n"<< std::endl;
}



//*****************************************************


//class Order

Order::Order(int id): BaseAction(),tableId(id){
}

void Order::act(Restaurant &restaurant){
    Table *orderT = restaurant.getTable(tableId);
    if(orderT && orderT->isOpen()){
        orderT->order(restaurant.getMenu());
        complete();
    }
    else
        error("Table does not exist or is not open");
}

std::string Order::toString() const{
    std::cout <<"order-status from table number " << tableId <<" is:   *** "<< getStatus()<<"   ***\n"<< std::endl;

}


//*******************************************************


//class CloseAll

CloseAll::CloseAll():BaseAction(){
}

void CloseAll::act(Restaurant &restaurant){
    std::vector<Table*>* TList=restaurant.getAllTables();
    for(auto &TableToClose: *TList)
        Close(TableToClose->getId()).act(restaurant);
    restaurant.close();
    //
}

std::string CloseAll::toString() const{
    std::cout << "closing-request for the whole restaurant is ***" << getStatus() << "***" << std::endl;
}


//*******************************************************

//class MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(),srcTable(src),dstTable(dst),id(customerId){
}

void MoveCustomer::act(Restaurant &restaurant){
    Table* Tsrc=restaurant.getTable(srcTable);
    Table* Tdest=restaurant.getTable(dstTable);
    if(Tdest&&Tsrc){
        Customer* c=Tsrc->getCustomer(id);
        if(c&&Tdest->availableSeat()) {
            Tdest->addCustomer(c);
            Tsrc->removeCustomer(id);
            if(Tsrc->takenSeats()==0)
                Tsrc->closeTable();
            complete();
            return;
        }
    }
    error("Cannot move customer");
}

std::string MoveCustomer::toString() const{
    std::cout << "request for moving customer "<<this->id<<" is " << getStatus() << std::endl;

}


//********************************************************

//PrintMenu class
PrintMenu::PrintMenu():BaseAction(){
}
//Prints the menu
void PrintMenu::act(Restaurant &restaurant){
    for(auto &it: restaurant.getMenu()){
        std::cout << it.getName() << " ";
        std::cout << it.getType() << " ";
        std::cout << it.getPrice() << "NIS\n" << std::endl;
    }
}
std::string PrintMenu::toString() const{
    std::cout << "request for printing menu is " << getStatus() << std::endl;

}


//*******************************************************
//PrintTableStatus class

PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id){}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    std::cout << "Table " << tableId;
    std::cout << " status: " << std::endl;
    if(!(table->open)) {
        std::cout << "closed";
        return;
    }
    else {
        std::cout << "open\n";
        std::cout << "Customer:\n";
        for(auto &it: table->getCustomers()){
            std::cout << it->getId() << " ";
            std::cout << it->getName() + "\n" <<std::endl;
        }
        std::cout << "Orders:\n";
        for(auto &it: table->getOrders()){
            std::cout << it.second.getName() << " ";
            std::cout << it.second.getPrice() << "NIS ";
            std::cout << it.first << "\n" << std::endl;
        }
        std::cout << "Current Bill: " << table->getBill() << std::endl;
    }

}

std::string PrintTableStatus::toString() const {
    std::cout << "printing status of table "<<this->tableId<<" is " << getStatus() << std::endl;

}


*******************************************************


//PrintActionsLog Class - NEED TO DO
PrintActionsLog::PrintActionsLog():BaseAction() {

}

void PrintActionsLog::act(Restaurant &restaurant) {

}

std::string PrintActionsLog::toString() const {
    std::cout << "printing action log is " << getStatus() << std::endl;

}

//*******************************************************
//class BackupRestaurant

BackupRestaurant::BackupRestaurant() :BaseAction(){
}

void BackupRestaurant::act(Restaurant &restaurant) {
    if(!(&backup))
        backup=new Restaurant(restaurant);
    else{
        backup->clear();
        backup=&restaurant;
    }
}


std::string BackupRestaurant::toString() const {
    std::cout <<"backup is:   *** "<< getStatus()<<"   ***\n"<< std::endl;
}

//*******************************************************

//class RestoreRestaurant
RestoreResturant::RestoreResturant():BaseAction(){

}
void RestoreResturant::act(Restaurant &restaurant) {
    if (backup) {
        restaurant.clear();
        restaurant=*backup;
        complete();
    } else error("No backup available");
}

std::string RestoreResturant::toString() const {
    std::cout << "restoring-backup status is " << getStatus() << std::endl;
}



//******************************************************
