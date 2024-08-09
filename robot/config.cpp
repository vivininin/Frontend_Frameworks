#include"config.h"
#include<fstream>

Config* Config::instance = NULL;
const char CONFIG_FILE_NAME[] = "_config_data";

// get the singleton
Config &Config::getInstance(){
    if(instance == NULL){
        instance = new Config();
    }
    return *instance;
}

Config::Config(){
    load();
}

// load configs from file
void Config::load(){
    std::ifstream fin_data(CONFIG_FILE_NAME);
    int i;
    std::string name;
    double value;
    names.clear();
    values.clear();
    for(i = 0; ; i++){
        fin_data >> name >> value;
        if(not fin_data) break;
        names.push_back(name);
        values.push_back(value);
    }
    fin_data.close();
}

// save configs to file
void Config::save(){
    int n, i;
    std::ofstream fout_data(CONFIG_FILE_NAME);
    n = names.size();
    for(i = 0; i < n; i++){
        fout_data << names[i] << ' ' << values[i] << '\n';
    }
    fout_data.close();
}

// set a config variable (name, value) if not exist
void Config::initConfig(char *name, double value){
    int i, n;
    n = names.size();
    for(i = 0; i < n; i++){
        if(names[i] == name)
            return;
    }
    names.push_back(name);
    values.push_back(value);
    save();
}

double Config::getConfig(char *name) throw(ConfigNotFoundException){
    int i, n;
    load();
    n = names.size();
    for(i = 0; i < n; i++)
        if(names[i] == name)
            break;
    if(i == n) 
        throw ConfigNotFoundException(name);
    return values[i];
}
