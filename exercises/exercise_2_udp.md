## ESERCIZIO 2 - UDP+DNS

Progettare ed implementare un’applicazione UDP client/server, conforme al seguente protocollo applicativo:

1.	Il client legge da tastiera il *nome* e il numero di porta del server UDP da contattare (Esempio: `srv.di.uniba.it:56700`).
2.	Il client, invia il messaggio iniziale `Helo` al server.
3.	Ricevuti i dati client, il server visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client (Esempio: `Ricevuti dati dal client nome: pippo.uniba.it, indirizzo: 193.204.187.154`).
4.	Il client legge una stringa di caratteri dallo std input e la invia al server (Esempio: `ciao`).
5.	Il server legge la stringa inviata dal client e la visualizza sullo std output; dopodiché, elimina tutte le vocali, la vistualliza sullo std output e la invia nuovamente al client (Esempio: `Ricevuto 'ciao', inviato 'c'`).
6.	Il server resta in attesa di altri dati.
7.	Il client legge la risposta inviata dal server e visualizza la risposta sullo std output (Esempio `Stringa 'c' ricevuta dal server nome: srv.di.uniba.it inidirizzo: 193.204.187.166`); dopodiché, il client termina.
