**Secondo esonero di Laboratorio - Reti di Calcolatori (INF)**

Progettare ed implementare un'applicazione UDP client/server, dove il server è una calcolatrice remota che risponde alle richieste del client nel seguente modo:
1. Il client legge da tastiera il *nome* e il numero di porta del server UDP da contattare, rispettando il seguente formato : `srv.di.uniba.it:56700`.
2. Il client stampa un help con i comand a dispozione
3. Il client legge la scelta dell'operazione da compiere (usando i caratteri `A`, `M`, `S`, `D`, rispettivamente per _Addizione_, _Moltiplicazione_, _Sottrazione_ e _Divisione_) e due numeri interi dallo standard input , rispettando il seguente formato: `A 23 45`
4. Il client invia al server quanto letto da tastiera.
5. Ricevuti i dati dal client, il server visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client e l'operazione da eseguire (Esempio: `Richiesta operazione 'A 23 45' dal client pippo.di.uniba.it, ip 193.204.187.154`).
6. Il server segue l'operazione richiesta e invia il risultato al client ***nell'esatto formato*** indicato negli esempi riportati di seguito:
    * `23 + 45 = 68` per l'addizione;
    * `23 / 45 = 0.51` per la divisione;
    * `23 * 45 = 1,035` per il prodotto;
    * `23 - 45 = -22` per la sottrazione.
7. Il client legge la risposta inviata dal server e visualizza la risposta sullo std output (Esempio: `Ricevuto risultato dal server srv.di.uniba.it, ip 193.204.187.166: 23 + 45 = 68`).
8. Il client legge dallo standard input la successiva operazione da compiere.
9. Se invece di un'operazione è inserito il carattere `=`, il client termina qui il suo processo; altrimenti, torna al punto 2.
10. Il server non termina mai il suo processo.

**NOTE:**
- Creare le funzioni matematiche utilizzando i seguenti nomi: `add()`, `mult()`, `sub()` e `divis()`.

- Usare un file `.h` per racchiudere le specifiche di protocollo applicativo condivise fra client e server.

- Non saranno accettate consegne di studenti che non hanno sostenuto con successo il primo esonero di laboratorio.

- La consegna dovr avvenire entro **TBD** attraverso questo form.

- L'elenco degli ammessi sarà pubblicato online a questo indirizzo (TBD).

- Gli ammessi al secondo esonero di laboratorio potranno sostenere direttamente la prova scritta, previa prenotazione su ESSE3.
