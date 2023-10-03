#include <iostream>
#include<fstream>
#include<map>
#include "crow.h"
#include <stdio.h>
#include <stdexcept>

using namespace std;

class myExceptionFile : public std::exception {
public:
    const char * what() {
       return "Impossibile aprire il file";
    }
};

map<string, string> caricaFile() {
    try {
        ifstream file("User.txt");
        if (!file.is_open()) {
            throw myExceptionFile();            
        }
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
    catch(myExceptionFile ex){        
        cerr << "Error: " << ex.what() << endl;
        return {};
    }
    
}
int cercaInFile(string filename, string lezione) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            throw myExceptionFile();
        }
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
    catch (myExceptionFile ex) {
        cerr << "Error: " << ex.what() << endl;       
        return {};           
        
    }
    
}

int main()
{     
    cout << "Avvio del server ...\n" << endl;
    crow::SimpleApp app;    
        
    CROW_ROUTE(app, "/lezioni")
        .methods("GET"_method)([](const crow::request& req) {
        try {
            ifstream file("lezioni.txt");
            if (!file.is_open()) {
                throw myExceptionFile();
            }
            string line;
            string postlezione;

            while (std::getline(file, line))
            {
                postlezione += line + '\n';
            }
            file.close();

            if (postlezione != "")
                return crow::response(postlezione);
            else if (postlezione == "") {                
                return crow::response(501);
            }
        }
        catch (myExceptionFile ex) {
            cerr << "Error: " << ex.what() << endl;
            return crow::response(500);    
        }                      

    });

    CROW_ROUTE(app, "/reservation")
        .methods("GET"_method)([](const crow::request& req) {
        string reservation;
        try {
            ifstream file("User.txt");
            if (!file.is_open()) {
                throw myExceptionFile();
            }
            map<string, string> users;
            string line, usr;

            while (std::getline(file, line)) {
                istringstream iss(line);
                getline(iss, usr, ';');
                ifstream file(usr + ".txt");
                string line;
                string getlezione;
                while (std::getline(file, line))
                {
                    getlezione += line + '\n';
                }
                file.close();
                reservation += getlezione;
            }
            file.close();

            if (reservation != "")
                return crow::response(reservation);
            else
                return crow::response(501);
        }
        catch (myExceptionFile ex) {
            cerr << "Error: " << ex.what() << endl;
            return crow::response(500);
        }       

            });

    CROW_ROUTE(app, "/reservation/<string>")([](string usr){
        try {
            ifstream file(usr + ".txt");
            if (!file.is_open()) {
                throw myExceptionFile();
            }
            string line;
            string getlezione;

            while (std::getline(file, line))
            {
                getlezione += line + '\n';
            }
            file.close();

            if (getlezione != "")
                return crow::response(getlezione);
            else
                return crow::response(500);
        }
        catch (myExceptionFile ex) {
            cerr << "Error: " << ex.what() << endl;
            return crow::response(501);
        }
                
            });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method, "GET"_method)([](const crow::request& req) {

        string json_psw, json_nome, res, line, nome;
        auto json_data = crow::json::load(req.body);
        json_nome = json_data["User:"].s();        
        json_psw = json_data["Password:"].s();        

        map<string, string> users = caricaFile();
        if (users.find(json_nome) != users.end() && users[json_nome] == json_psw)

            return crow::response(200);
        else
            return crow::response(501);       
              
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
            try {
                ofstream file("User.txt", ios::app);
                if (!file.is_open()) {
                    throw myExceptionFile();
                }
                file.eof();
                file << tmp << "\n";
                file.close();
                return crow::response(200);
            }
            catch (myExceptionFile ex) {
                cerr << "Error: " << ex.what() << endl;
                return crow::response(501);
            }            
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
                try {
                    ofstream file(usr + ".txt", ios::app);
                    file.eof();
                    file << lez + "\n";
                    file.close();
                    return crow::response(200);
                }
                catch (myExceptionFile ex) {
                    cerr << "Error: " << ex.what() << endl;
                    return crow::response(500);
                }
               
            }
                          
            else
                return crow::response(501);
        }
        else if (lez == "") {            
            if (remove(filename.c_str()) == 0) {
                return crow::response(200);
            }
            else return crow::response(400);
        }
            });   

    CROW_ROUTE(app, "/user")
        .methods("POST"_method)([](const crow::request& req) {
        auto json_data = crow::json::load(req.body);
    
        string usr = json_data.s();
        string filename = usr + ".txt";
        try {
            ifstream file(filename);
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
                return crow::response(501);
        }
        catch (myExceptionFile ex) {
            cerr << "Error: " << ex.what() << endl;
            return crow::response(500);
        }

            });

    app.port(60080).run();
}