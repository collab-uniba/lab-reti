## ESERCIZIO 1 - TCP

Progettare ed implementare un’applicazione TCP client/server, conforme al seguente protocollo applicativo. La porta del server è a scelta, purché maggiore di 1023 (le porte 0–1023 richiedono privilegi di amministratore/root).

1. Il client richiede la connessione al server.
2. Stabilita la connessione, il server invia al client la stringa "connessione avvenuta".
3. Il client stampa la stringa ricevuta; dopodiché, legge due stringhe di caratteri dallo standard input e le invia al server.
4. Il server legge le stringhe inviate dal client e:
   - le visualizza sullo standard output;
   - converte la prima stringa ricevuta tutta in MAIUSCOLO, la seconda tutta in minuscolo;
   - invia le nuove stringhe al client;
   - terminata la comunicazione con il client corrente, rimane in ascolto per servire il successivo (il server è sequenziale e serve un solo client alla volta).
5. Il client legge la risposta inviata dal server:
   - visualizza la risposta sullo standard output;
   - chiude la connessione corrente;
   - termina il processo.
