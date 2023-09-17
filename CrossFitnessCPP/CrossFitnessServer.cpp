#include <iostream>
#include<map>
#include "crow.h"

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
       
        std::string x = req.body;
        if (x == "User=Fabiola&Password%3A=1234" || x == "User=Matteo&Password%3A=9876")
           return crow::response(200);
        else 
            return crow::response(400);       
            });

    CROW_ROUTE(app, "/crea_account")
        .methods("POST"_method, "GET"_method)([](const crow::request& req) {

        std::string Utente = req.body;
        if (Utente == "")
            return crow::response(400);
        else
            return crow::response(200);
            });

    CROW_ROUTE(app, "/prenotazioni")
        .methods("POST"_method)([](const crow::request& req) {
        Prenotazione p; 

        std::string Utente = req.body;

        auto json_data = crow::json::load(req.body);
        p.setUsername(json_data["Username:"].s());
        p.setIDPrenotazione(json_data["IDPrenotazione:"].i());
        p.setIDPersona(json_data["IDPersona:"].i()); 
        p.setLezione(json_data["LezioneStr:"].s());
        p.LezioneList.push_back(json_data["LezioneStr:"].s());

        cout << "PRENOTAZIONE: \n" << p.showPrenotazione() << endl;
     
        if (Utente == "")
            return crow::response(400);
        else
            return crow::response(200);
            });

    app.port(60080).run();
}