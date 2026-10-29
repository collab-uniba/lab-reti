## ESERCIZIO 2 - UDP+DNS

Progettare ed implementare un’applicazione UDP client/server, conforme al seguente protocollo applicativo. La porta del server è a scelta, purché maggiore di 1023 (le porte 0–1023 richiedono privilegi di amministratore/root).

1. Il client legge da tastiera il *nome* e il numero di porta del server UDP da contattare (Esempio: `srv.di.uniba.it:56700`).
2. Il client invia il messaggio iniziale `Helo` al server.
3. Ricevuti i dati dal client, il server visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client; il nome si ottiene con una risoluzione inversa dell’indirizzo del mittente (Esempio: `Ricevuti dati dal client nome: localhost, indirizzo: 127.0.0.1`).
4. Il client legge una stringa di caratteri dallo std input e la invia al server (Esempio: `ciao`).
5. Il server legge la stringa inviata dal client e la visualizza sullo std output; dopodiché, elimina tutte le vocali (a, e, i, o, u, sia minuscole sia maiuscole; non è richiesto gestire le vocali accentate), la visualizza sullo std output e la invia nuovamente al client (Esempio: `Ricevuto 'ciao', inviato 'c'`).
6. Terminata la comunicazione con il client corrente, il server resta in attesa del client successivo (il server è sequenziale e serve un solo client alla volta).
7. Il client legge la risposta inviata dal server e visualizza sullo std output la risposta, il *nome* del server inserito al passo 1 e l’*indirizzo* da cui è arrivata la risposta (Esempio `Stringa 'c' ricevuta dal server nome: srv.di.uniba.it indirizzo: 127.0.0.1`); dopodiché, il client termina.

**Note:**

- Per la risoluzione dei nomi si usino `getaddrinfo()` per ricavare l’indirizzo dal nome (client) e `getnameinfo()` per ricavare il nome dall’indirizzo (server).
- Per provare in locale un nome come `srv.di.uniba.it`, lo si può associare a `127.0.0.1` nel file hosts (`/etc/hosts` su Linux e macOS, `C:\Windows\System32\drivers\etc\hosts` su Windows).
- La risoluzione inversa di `127.0.0.1` restituisce di norma `localhost`, che è quindi il nome del client atteso nelle prove in locale.
