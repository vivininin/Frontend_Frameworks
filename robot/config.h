#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<exception>
using std::string;

class ConfigNotFoundException{
    private:
        string _name;
    public:
        ConfigNotFoundException(char* name){
            _name = name;
        }
        virtual ~ConfigNotFoundException(){}

    virtual const char* what() const throw(){
        return ("Config" + _name + "not found.").c_str();
    }
};

class Config{
    private:
        std::vector<std::string> names;
        std::vector<double> values;
        Config();
        static Config* instance;
        void save();
        void load();
    public:
        static Config& getInstance();

        void initConfig(char *name, double value);
        double getConfig(char *name) throw(ConfigNotFoundException);
};
