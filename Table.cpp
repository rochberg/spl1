#include <iostream>
#include "Table.h"

//template <class T> class shared_ptr;



//constuctor (reg)
Table::Table(int t_capacity): capacity(t_capacity),open(false){
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList;
}



//copy constructor
Table::Table(const Table& t):customersList(t.customersList), capacity(t.getCapacity()), open(t.isOpen()), id(t.getId()){
    std::vector<OrderPair>orderList;
    int numOfOrders=(int)t.orderList.size();
    std::vector<OrderPair>* temp2=&t.getOrders();
    for(int i=0;i<numOfOrders;i++){
        OrderPair o=new OrderPair(temp2[i]);
        OrderPair[i]=o;
    }
}



//move constructor
Table::Table(Table&& T):capacity(T.getCapacity()),open(T.isOpen()),id(T.getId()){
    this->customersList= std::move (T.getCustomers());
    this->orderList=std::move (T.getOrders());

}


//assignment (reg)
Table& Table::operator=(const Table& t){
    if(this==&t)
        return *this;
    clear();
    Table temp(t);
    (*this)=std::move (temp);
    return *this;
}


//assignment move
Table& Table::operator=(Table&& t){
    customersList=t.getCustomers();
    orderList=t.getOrders();
    t.customersList= nullptr;
    t.orderList= nullptr;

}


//destructor
Table::~Table() {
  clear();
}



void Table::clear(){
    customersList.clear();
    orderList.clear();
    customersList=nullptr;
    orderList=nullptr;
}



int Table::takenSeats() const{
    return (int)customersList.size();
}


 int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    if(customer){
        if(capacity>takenSeats()){
            customersList.push_back(customer);
            if(!isOpen())
                open= true;
        }
        else
            std::cout << "no room in table" << std::endl;
    }

}

bool Table::availableSeat(){
    return getCapacity()>takenSeats();
}


void Table::removeCustomer(int id) {
    auto it=customersList.begin();
    while (it!=customersList.end()) {
        if ((*it)->getId() == id) {
            delete (*it);
            (*it)= nullptr;
            customersList.erase(it);
            if(customersList.empty())
                open= false;
            return;
        }
        else
            it++;
    }
}

Customer* Table::getCustomer(int id) {
    auto it=customersList.begin();
    while (it!=customersList.end()) {
        if ((*it)->getId() == id) {
            return *it;
        }
        else
            it++;
    }
    return nullptr;
}


void Table::reOpen(){
    customersList.clear();
    orderList.clear();
    open= false;
}




std::vector<Customer*>& Table::getCustomers(){
    return this->customersList;
}

std::vector<OrderPair>& Table::getOrders(){
    //order from each customer
    return this->orderList;
}

void Table::order(const std::vector<Dish> &menu){
    for(Customer* c: customersList){
        OrderPair p ;
        p.first = c->getId();
        p.second = c->order(menu);
        orderList.push_back(p);
    }
}


void Table::openTable(){
    open= true;
}

void Table::closeTable(){
    open=false;
}

int Table::getBill(){

}

bool Table::isOpen() const{
    return open;
}

int Table::getId() const{
    return id;
}










