# Secondo esonero di Laboratorio - Reti di Calcolatori (INF) aa 20-21 🎅 🎄 🎁

Progettare ed implementare un'applicazione UDP client/server, dove il server è una calcolatrice remota che risponde alle richieste del client nel seguente modo:

1. Il client legge da tastiera il *nome* e il numero di porta del server UDP da contattare, rispettando il seguente formato : `srv.di.uniba.it:56700`.
2. Il client legge la scelta dell'operazione da compiere (usando i caratteri `+`, `x`, `-`, `/`, rispettivamente per _Addizione_, _Moltiplicazione_, _Sottrazione_ e _Divisione_) e due numeri interi dallo standard input, rispettando il seguente formato: `+ 23 45`
3. Il client invia al server quanto letto da tastiera.
4. Ricevuti i dati dal client, il server visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client e l'operazione da eseguire (Esempio: `Richiesta operazione '+ 23 45' dal client pippo.di.uniba.it, ip 193.204.187.154`).
5. Il server segue l'operazione richiesta e invia il risultato al client ***nell'esatto formato*** indicato negli esempi riportati di seguito:
    * `23 + 45 = 68` per l'addizione;
    * `23 / 45 = 0.51` per la divisione;
    * `23 * 45 = 1,035` per il prodotto;
    * `23 - 45 = -22` per la sottrazione.
6. Il client legge la risposta inviata dal server e visualizza la risposta sullo std output (Esempio: `Ricevuto risultato dal server srv.di.uniba.it, ip 193.204.187.166: 23 + 45 = 68`).
7. Il client legge dallo standard input la successiva operazione da compiere.
8. Se invece di un'operazione è inserito il carattere `=`, il client termina qui il suo processo; altrimenti, torna al punto 2.
9. Il server non termina mai il suo processo.

## REQUISITI

* Il protocollo applicativo condiviso fra client e server deve essere specificato tramite file header `.h`.
* Creare le funzioni matematiche utilizzando i seguenti nomi: `add()`, `mult()`, `sub()` e `division()`.
* La lettura da riga di comando è effettuata in una singola lettura (ossia `+ 23 45[invio]`, non `+[invio]23[invio]45[invio]`).
* Il client e il server devono potersi avviare senza parametri passati da riga di comando, ossia useranno un indirizzo ip e numero di porta predefiniti.
* Le operazioni sono da intendersi indipendenti l'una dall'altra (ossia, i risultati delle operazioni non sono cumulativi).
* Codice e commenti dovranno essere scritti in inglese.

## NOTE

* La consegna dovrà avvenire entro il **19 dicembre** attraverso questo [form](https://forms.gle/iZiTXeUMfh93i5GK6).
* L'elenco degli ammessi sarà pubblicato online a questo [indirizzo](TBD).
* Gli studenti che avranno superato anche la seconda prova pratica di laboratorio potranno sostenere direttamente la prova di esame scritta, previa prenotazione su ESSE3.
