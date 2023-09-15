#include <iostream>
#include "crow.h"

using namespace std;

class Prenotazione {
public:

    int IDPrenotazione;
    int IDPersona;
    string Username;
    vector<string> LezioneList; 

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
        .methods("POST"_method,"GET"_method)([](const crow::request& req) {

        std::string Utente = req.body;
        if (Utente == "")
            return crow::response(400);
        else
            return crow::response(200);
            });

    app.port(60080).run();
}