**Primo esonero di Laboratorio - Reti di Calcolatori (INF) A.A. 2018/2019**

Progettare ed implementare un'applicazione TCP client/server, dove il server è una calcolatrice remota che risponde alle richieste del client nel seguente modo:
1. Il client legge dallo standard input l'indirizzo e il numero di porta del server (es. `58000`), dopodiché richiede la connessione al server.
2. Ricevuta la connessione, il server visualizza sullo standard output l'indirizzo e il numero di porta del client, scrivendo "*Connection established with `xxx.xxx.xxx.xxx:yyyyy`"*.
3. Stabilita la connessione, il client legge l'operazione da compiere (usando i caratteri `A`, `M`, `S`, `D`, rispettivamente per _Addizione_, _Moltiplicazione_, _Sottrazione_ e _Divisione_) e due numeri interi dallo standard input (es. `A 23 45`) ed invia al server quanto letto da tastiera.
4. Il server legge quanto inviato dal client, esegue l'operazione richiesta e invia il risultato al client (`es. 68`).
5. Il client legge la risposta inviata dal server e la visualizza sullo standard output.
6. Il client legge dallo standard input la successiva operazione da compiere.
7. Se invece di un'operazione è inserito il carattere `=`, il client chiude la connessione con il server e termina qui il suo processo; altrimenti, torna al punto 4.
8. Il server non termina mai il suo processo e deve essere in grado di accettare una coda massima di 6 client contemporaneamente.

**NOTE:**
- Creare le funzioni matematiche utilizzando i seguenti nomi: `add()`, `mult()`, `sub()` e `div()`.

- Non saranno accettate consegne di studenti che non si sono opportunamente registrati sul form di prenotazione pubblicato sulla piattaforma ADA.

- La consegna deve avvenire di persona durante le lezioni di laboratorio di venerdì 9 novembre o, in alternative, di venerdì 16 novembre. Qualora l'esonero sia stato svolto in coppia, entrambi gli studenti dovranno essere presenti.

- La correzione sarà contestuale alla consegna.

- Gli ammessi potranno sostenere la seconda prova di esonero su UDP.
