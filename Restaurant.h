#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
    Restaurant(const Restaurant& R);
    Restaurant(Restaurant&& R);
    Restaurant(const std::string &configFilePath);
	Restaurant& operator=(const Restaurant& R);
    Restaurant& operator=(Restaurant&& R);
    virtual ~Restaurant();
	void clear();
	void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    DishType findType(std::string str);
	Dish& createDish(std::string line, int id);
	int readNumOfTables(std::string line);
	std::vector<Table*>& createTables(int numOfTables, std::string line);
	void readFile(std::ifstream& input);
	std::vector<Table*>* getAllTables();
	void close();


private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif