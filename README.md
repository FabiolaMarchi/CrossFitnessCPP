# CrossFitnessCPP

Server dell' applicazione Cross Fitness.
Prevede l'implementazione della comunicazione con i client C# (User) e Python (Admin) mediante REST API request sulla porta 60080, gestite attraverso al libreria "CROW".
Lo storage dei dati si gestisce mediante dei file di testo.

Di seguito si presentano le REST API implementate:
1. GET: http://localhost:60080/lezioni -> Per ottenere tutte le lezioni prenotabili 
2. GET: http://localhost:60080/reservation -> Per ottenere tutte le prenotazioni effettuate dagli utenti 
3. GET: http://localhost:60080//reservation/<string> -> Per ottenere le prenotazioni di un utente

4. POST: http://localhost:60080/login -> Il client C# invia le credenziali dell'utente e viene effettuata una verifica di correttezza/esistenza 
5. POST: http://localhost:60080/crea_account ->Il client C# invia le credenziali dell'utente per creare l'account
6. POST: http://localhost:60080/prenotazioni ->Il client C# invia i dati per effettuare una prenotazione, nel caso in cui sia inviato solo lo username allora tutte le prenotazioni ad esso correlate vengono eliminate
7. POST: http://localhost:60080/user -> Il client C# invia i dati dell'utente per visualizzare le prenotazioni effettuare
