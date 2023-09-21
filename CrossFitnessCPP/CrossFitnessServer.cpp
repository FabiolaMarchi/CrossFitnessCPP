#include <iostream>
#include<fstream>
#include<map>
#include "crow.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;
class Prenotazione {
public:

    int IDPrenotazione;
    int IDPersona;
    string Username;
    string lezione;
    vector<string> LezioneList; 
    map<int, string> LezioniDict;

    int getIDPrenotazione(){ return IDPrenotazione; }
    int getIDPersona(){ return IDPersona; }
    string getUsername(){ return Username; }
    string getLezioneStr() { return lezione; }

    string getLezione()
    {
        string s = "";
        for(int i =0; i < LezioneList.size(); i++)
        {
            s += LezioneList[i];
            s += " ";
        }
        return s;
    }
    void setIDPrenotazione(int IDPren){ IDPrenotazione = IDPren; }

    void setIDPersona(int IDPers){ IDPersona = IDPers; }

    void setUsername(string name){ Username = name; }

    void setLezioneStr(string lez) { lezione = lez; }

    void setLezione(string lezione){LezioneList.push_back(lezione); }

    void setLezioneDict(map<int, string> dict) { LezioniDict = dict; };

    string showPrenotazione()
    {
        string s;
        s = "ID Prenotazione : " + to_string(getIDPrenotazione()) + "\n" +
            "ID Persona : " + to_string(getIDPersona()) + "\n" + "Username :" + getUsername() +
            "\n" + "Prenotazione: " + getLezione() + "\n";
        return s;
    }       

    Prenotazione()
    {

    }
       
    
};

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


int main()
{
    std::cout << "Hello World!\n";     
    crow::SimpleApp app;
    

    CROW_ROUTE(app, "/json")
        ([] {
        crow::json::wvalue x({ {"zmessage", "Hello, World!"},
                              {"amessage", "Hello, World2!"} });
    return x;
    });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method, "GET"_method)([](const crow::request& req) {
        string json_psw, json_nome, res;     
       
        auto json_data = crow::json::load(req.body);
        json_nome = json_data["User:"].s();        
        json_psw = json_data["Password:"].s();        

        map<string, string> users = caricaFile();
        cout << "ciao: \n" <<  endl;
        ifstream file1(json_nome + ".txt");
        
        string line, nome;
        
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
        .methods("POST"_method, "DELETE"_method)([](const crow::request& req) {
        
        Prenotazione p;
        string tmp;
        std::string Utente = req.body;
        if (req.method == "POST"_method) {
            
            auto json_data = crow::json::load(req.body);
            p.setUsername(json_data["Username:"].s());
            p.setIDPrenotazione(json_data["IDPrenotazione:"].i());
            p.setIDPersona(json_data["IDPersona:"].i());
            p.setLezione(json_data["LezioneStr:"].s());
            string usr = json_data["Username:"].s();
            int IDpr = json_data["IDPrenotazione:"].i();
            int IDp = json_data["IDPersona:"].i();
            string lez = json_data["LezioneStr:"].s();
            
            ofstream file(usr + ".txt", ios::app);
            file.eof();
            file << lez;
            file.close();          
                              
            cout << "PRENOTAZIONE: \n" << p.showPrenotazione() << endl;

            if (Utente == "")
                return crow::response(400);
            else
                return crow::response(200);
        }
        else if (req.method == "DELETE"_method)
        {
            ofstream fout("Prenotazione.txt");
            fout << " ";
            fout.close();            
            return crow::response(200);
           
        }
    });
        

    app.port(60080).run();
}