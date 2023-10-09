# CrossFitnessCPP

Server dell' applicazione Cross Fitness
La comunicatrione con i client ADMIN e USER avviene mediante REST-API, lo stagage delle infomazioen e' gestito tramite file di testo. 

Tramite la libreria CROW ROUTE sono state implementate le rest API (port:60080) che permettono di gestire: 

1. GET: /lezioni -> Ottenere tutte le lezioni prenotabili salvarte nel file lezioni.txt
2. GET: /reservation -> Ottenere tutte le prenotazioni da tutti gli utenti registrati (Analissi dell' affluenza)
3. GET: /reservation/<string> -> Ottenere tutte le prenotazioni del singolo utente

4. POST: /login -> IN: username e pws -> Viene eseguita una ricerca nel file User.txt -> OUT: 200=good 501=bad  
5. POST: /crea_account -> IN: username e pws -> Viene aggiuta una linea nel file User.txt (NO controllo duplicati) -> OUT: 200=good 501=bad  
6. POST: /prenotazioni -> IN: list<string> -> Viene aggiuta una o più linee nel file <username>.txt (SI controllo duplicati) -> OUT: 200=good 501=bad OTHERS=error
7. POST: /prenotazioni -> IN: list VUOTA -> Viene cancellato il file <username>.txt -> OUT: 200=good 400=bad 
8. POST: /user -> IN: username -> Viene restituite le prenotazzioni toprovate nel file <username>.txt -> OUT: 200=good 501=bad 500=ServerERROR
