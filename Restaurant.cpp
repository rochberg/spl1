#include "Restaurant.h"
#include <vector>
#include <fstream>


Restaurant::Restaurant(){
}

Restaurant::Restaurant(const std::string &configFilePath):open(true){
    std::ifstream input;
    input.open(configFilePath);
     if(input.is_open())
         readFile(input);
     input.close();
}

void Restaurant::readFile(std::ifstream& input){
    int counter = 0;
    int idDish = 0;
    int numOfTables = 0;
    std::string line;
    std::vector<Dish> menuTemp;
    while(getline(input, line)){
        if(line[0] == '#' || line.empty())
            continue;
        else if(counter == 0){
            numOfTables = readNumOfTables(line);
            counter++;
        }
        else if(counter == 1){
            this->tables = std::move (createTables(numOfTables, line));
            counter++;
        }
        else if(counter == 2){
            menuTemp.push_back(createDish(line, idDish));
        }
    }
    this->menu = menuTemp;
}


//copy const
Restaurant::Restaurant(const Restaurant& R):open(R.open) {
    const std::vector<Table *> *RTables = &(R.tables);
    for (auto &tmp: *RTables) {
        Table *t=new Table(*tmp);
        this->tables.push_back(t);
    }
    for (auto &d : R.menu)
        this->menu.push_back(Dish(d));
    const std::vector<BaseAction*>* RAction = &(R.getActionsLog());
    for (auto &act : *RAction) {
        BaseAction *t(act);
        this->actionsLog.push_back(t);
    }
}

//move const
Restaurant::Restaurant(Restaurant&& R) {
    this->tables=std::move (R.tables);
    this->menu=std::move (R.menu);
    this->actionsLog=std::move (R.actionsLog);
    this->open=R.open;
}

//move assignment
Restaurant& Restaurant::operator=(Restaurant&& R){
    clear();
    this->tables=std::move (R.tables);
    this->actionsLog=std::move (R.actionsLog);
    this->menu=std::move(R.menu);
    this->open=R.open;
}

//destructor
Restaurant::~Restaurant() {
    clear();
}

void Restaurant::clear(){
    tables.clear();
    menu.clear();
    actionsLog.clear();
}

//assignment op
Restaurant& Restaurant::operator=(const Restaurant &R) {
    if(this == &R)
        return *this;
    clear();
    Restaurant TMP(R);
    *this= std::move (TMP);
    return* this;
}



int Restaurant::readNumOfTables(std::string line){
    return std::stoi(line);
}

std::vector<Table*>& Restaurant::createTables(int numOfTables, std::string line){
    std::vector<Table*> tables;
    std::vector<std::string> seats;
    for(int i=0; i<numOfTables; i++){
        auto comma = line.find(',');
        Table* table = new Table(std::stoi(line.substr(0,comma)));
        table->id=(int)tables.size();
        this->tables.push_back(table);
        line.erase(0,comma);
    }
    return this->tables;
}

Dish& Restaurant::createDish(std::string line, int id){
    std::vector<std::string> dishTemp;
    for(int i = 0; i < 3; i++){
        auto comma = line.find(',');
        dishTemp.push_back(line.substr(0,comma));
        line.erase(0,comma);
    }
    DishType type = findType(dishTemp[1]);
    Dish d(id, dishTemp[0], std::stoi(dishTemp[2]), type);
    return d;
}

DishType Restaurant::findType(std::string str){
    DishType type;
    if(str.compare("VEG"))
        type = VEG;
    else if(str.compare("SPC"))
        type = SPC;
    else if(str.compare("BVG"))
        type = BVG;
    else if(str.compare("ALC"))
        type = ALC;
    return type;
}

int Restaurant::getNumOfTables() const{
    return (int)tables.size();
}

std::vector<BaseAction*>& Restaurant::getActionsLog() const{
    return actionsLog;                  //
}

std::vector<Dish>& Restaurant::getMenu(){
    return menu;
}


Table* Restaurant::getTable(int ind) {
    for (auto &it: tables) {
        if (ind == it->id)
            return it;
    }
    return nullptr;
}

std::vector<Table*>* Restaurant::getAllTables(){
    return &tables;
}

void Restaurant::start() {

}

void Restaurant::close(){
    open= false;
}




