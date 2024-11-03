# Primo esonero di Laboratorio - Reti di Calcolatori (ITPS A-L) aa 24-25

Progettare ed implementare un'applicazione TCP client/server, dove il server è un generatore di password che risponde alle richieste del client nel seguente modo:

1. Una volta avviato, il client stabilisce automaticamente la connessione al server (sulla porta e indirizzo da voi stabiliti).
2. Ricevuta la connessione, il server visualizza sullo standard output l'indirizzo e il numero di porta del client, scrivendo "*New connection from `xxx.xxx.xxx.xxx:yyyyy`*".
3. Stabilita la connessione, il client legge il tipo di password da generare dallo standard input usando i caratteri:
   - `n`: password numerica (solo cifre)
   - `a`: password alfabetica (solo lettere minuscole)
   - `m`: password mista (lettere minuscole e numeri)
   - `s`: password sicura (lettere maiuscole, minuscole, numeri e simboli)
   
   seguito dalla lunghezza desiderata (es. `n 8` per una password numerica di 8 caratteri).
4. Il server legge quanto inviato dal client, genera la password secondo i criteri richiesti e la invia al client (es. `45237891`).
5. Il client legge la password generata dal server e la visualizza sullo standard output.
6. Il client legge dallo standard input la successiva richiesta di generazione.
7. Se invece di una richiesta valida è inserito il carattere `q`, il client chiude la connessione con il server e termina qui il suo processo; altrimenti, torna al punto 3.
8. Il server non termina mai il suo processo e deve essere in grado di accettare una coda massima di 5 client (parametro `qlen`).

## REQUISITI

* Il protocollo applicativo condiviso fra client e server deve essere specificato tramite file header `.h`.
* Creare le funzioni di generazione utilizzando i seguenti nomi: `generate_numeric()`, `generate_alpha()`, `generate_mixed()` e `generate_secure()`.
* La lettura da riga di comando è effettuata in una singola lettura (ossia `n 8[invio]`, non `n[invio]8[invio]`).
* Il client e il server devono potersi avviare senza parametri passati da riga di comando, ossia usando un indirizzo ip e numero di porta di default predefiniti.
* La lunghezza minima della password deve essere 6 caratteri e la massima 32.
* Codice e commenti dovranno essere scritti in inglese.

## NOTE

* La consegna deve avvenire entro la mezzanotte del giorno **25 novembre** attraverso questo [form](https://forms.gle/gQtVQeGKTvLu1Fkv6).
* È sufficiente una prenotazione sola per coppia.
* L'elenco degli ammessi sarà pubblicato *a tempo debito* su questo [indirizzo](https://docs.google.com/spreadsheets/d/1is06LNSfanO_7d03QZl_ftW9fMuMMCluliyfrepj2NM/edit?usp=sharing).
* Gli ammessi potranno automaticamente sostenere la seconda prova di esonero su UDP.
