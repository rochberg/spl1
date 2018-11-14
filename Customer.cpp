#include "Restaurant.h"//Making sure it doesn't put empty id in the order list
#include <limits>
#include <iostream>
#include <bits/shared_ptr.h>
#include "Customer.h"


typedef std::shared_ptr<class Customer> dog;

Customer* Customer::


//customer class
Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id){
}


std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}


//*********************************************

//VegetarianCustomer class
//Constructor for VEG customers
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id){
}

//Making an order for VEG costumer
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> order;
    for(auto &it: menu){ //NEED TO MAKE SURE THE ITERATOR STARTS FROM THE BEGINING , order lowest id veg dish
        if(it.getType() == 0)
            order.push_back(it.getId());
    }
    int price = 0;
    int idDish;
    for(auto &it: menu){ //NEED TO MAKE SURE THE ITERATOR STARTS FROM THE BEGINING, order priciest bvg dish
        if(it.getType() == 2 && it.getPrice() > price){
            price = it.getPrice();
            idDish = it.getId();
        }        
    }
    if(idDish != NULL) //Making sure it doesn't put empty id in the order list
            order.push_back(idDish);    
    return order;
}

std::string VegetarianCustomer::toString() const{ //Returns a string of customer's name and id
    std::cout << "Customer's Name: " + this->getName();
    std::cout << "Customer's ID: " + this->getId();
}




//*******************************************


//CheapCustomer class
//Constractor for CHP customers
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id), ordered(false){
}

//Making an order for CHP costumer
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> order;
    int price = std::numeric_limits<int>::max();
    int idDish;
    if(ordered) //Checks if the customer already ordered
        return order;
    for(auto &it: menu){ //order cheapest dish
        if(it.getPrice() < price){
            price = it.getPrice();
            idDish = it.getId();
        }    
    }
    if(idDish != NULL) //Making sure it doesn't put empty id in the order list
            order.push_back(idDish);        
    ordered = true;
    return order;
}

std::string CheapCustomer::toString() const{ //Returns a string of customer's name and id
    std::cout << "Customer's Name: " + this->getName();
    std::cout << "Customer's ID: " + this->getId();
}



//***************************************************





//SpicyCustomer class
//Constractor for SPC customers
SpicyCustomer::SpicyCustomer(std::string name, int id): Customer(name,id), ordered(false){
}

//Making an order for SPC costumer
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu){
    std::vector<int> order;
    int idDish;
    if(!ordered){
        int price = 0;
        for(auto &it: menu){ //Order priciest spc dish
            if(it.getType() == 1 && it.getPrice() > price){
                price = it.getPrice();
                idDish = it.getId();
            }   
        }   
        if(idDish != NULL) //Making sure it doesn't put empty id in the order list
                order.push_back(idDish);   
        ordered = true;      
    }
    else if(ordered){
        int price = std::numeric_limits<int>::max();
        for(auto &it: menu){ //Order cheapest bvg dish
            if(it.getType() == 2 && it.getPrice() < price){
                price = it.getPrice();
                idDish = it.getId();
            }          
        }
        if(idDish != NULL) //Making sure it doesn't put empty id in the order list
                order.push_back(idDish);  
    }
    return order;
}

std::string SpicyCustomer::toString() const{ //Returns a string of customer's name and id
    std::cout << "Customer's Name: " + this->getName();
    std::cout << "Customer's ID: " + this->getId();
}



//********************************************


//AlcoholicCustomer class
//Constructor for ALC customers
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(name,id), 
    finished(false), lastPrice(0){}

//Making an order for Alcoholic costumer
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> order;
    int idDish;
    int prevPrice = std::numeric_limits<int>::max();
    if (!finished) {
        for (auto &it: menu) { //Order cheapest alc dish
            int currPrice = it.getPrice();
            /* lastPrice is the last drink the customer ordered in the previous order, 
            pervPrice is the previous Drink in the iterator, 
            currPrice is the current drink in the iterator */
            if (it.getType() == 3 && currPrice > lastPrice && currPrice < prevPrice) {
                prevPrice = it.getPrice();
                idDish = it.getId();
            }
        }
        if (prevPrice == lastPrice) { /*if the final drink aka prevPrice equals to the previous
        order then the customer ordered all the alcholic drinks from the menu */
            finished = true;
            return order;
        } else if (idDish != NULL) { //Making sure it doesn't put empty id in the order list
            order.push_back(idDish);
            lastPrice = prevPrice;
        }
    }
    return order;
}

std::string AlchoholicCustomer::toString() const { //Returns a string of customer's name and id
    std::cout << "Customer's Name: " + this->getName();
    std::cout << "Customer's ID: " + this->getId();
}