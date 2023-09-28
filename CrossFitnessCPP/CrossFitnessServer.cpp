#include <iostream>
#include<fstream>
#include<map>
#include "crow.h"
#include <stdio.h>

using namespace std;

map<string, string> caricaFile() {
    
    ifstream file("User.txt");
    map<string, string> users;
    string line, nome;
    string psw;
    while (std::getline(file, line)) {
        istringstream iss(line);
        getline(iss, nome, ';');
        getline(iss, psw, ';');
        users.insert({ nome, psw });
    }
    file.close();
    return users;
}
int cercaInFile(string filename, string lezione) {

    ifstream file(filename);    
    string line;
    int flag = 0;
    while (std::getline(file, line)) {
        if (line == lezione) {
            flag++;
        }       
        
    }
    file.close();
    return flag;
}

int main()
{     
    cout << "Avvio del server ...\n" << endl;
    crow::SimpleApp app;    
        
    CROW_ROUTE(app, "/lezioni")
        .methods("GET"_method)([](const crow::request& req) {

        ifstream file("lezioni.txt");      
        string line;
        string postlezione;       

        while (std::getline(file, line))
        {
            postlezione += line + '\n';
        }
        file.close();

        if (postlezione != "")
            return crow::response(postlezione);      
        else
            return crow::response(400);

    });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method, "GET"_method)([](const crow::request& req) {

        string json_psw, json_nome, res, line, nome;
        auto json_data = crow::json::load(req.body);
        json_nome = json_data["User:"].s();        
        json_psw = json_data["Password:"].s();        

        map<string, string> users = caricaFile();
        
        ifstream file1(json_nome + ".txt");       
               
        while (std::getline(file1, line)) {                       
            res += line +  '\n';            
        }
        file1.close();
        if (users.find(json_nome) != users.end() && users[json_nome] == json_psw)             
            return crow::response(res);                     
        else
            return crow::response(400);       
       
    });                

    CROW_ROUTE(app, "/crea_account")
        .methods("POST"_method, "GET"_method)([](const crow::request& req) {

        string json_psw, json_nome, tmp;        
        auto json_data = crow::json::load(req.body);
        json_nome = json_data["User:"].s();
        json_psw = json_data["Password:"].s();
        tmp = json_nome + ";" + json_psw;
        if (json_nome == "" && json_psw == "")
            return crow::response(400);
        else
        {
            ofstream file("User.txt", ios::app);
            file.eof();
            file << tmp;
            file.close();
            return crow::response(200);
        }                       
    });
    CROW_ROUTE(app, "/prenotazioni")
        .methods("POST"_method)([](const crow::request& req) {

        auto json_data = crow::json::load(req.body);
        string usr = json_data["Username:"].s();        
        string lez = json_data["Lezione:"].s();
        string filename = usr + ".txt";
        if (lez != "") {
            if (cercaInFile(filename, lez) == 0) {
                ofstream file(usr + ".txt", ios::app);
                file.eof();
                file << lez;
                file.close();
                return crow::response(200);
            }
                          
            else
                return crow::response(400);
        }
        else if (lez == "") {            
            if (remove(filename.c_str()) == 0) {
                return crow::response(200);
            }
            else return crow::response(400);
        }
        
        

            });   

    app.port(60080).run();
}