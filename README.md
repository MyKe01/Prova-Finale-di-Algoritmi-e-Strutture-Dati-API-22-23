# Prova Finale di Algoritmi e Strutture Dati - AA 22/23

## Made with &#x2764;&#xfe0f; by :
- ###  [Michelangelo Stasi](https://github.com/MyKe01) <br> michelangelo.stasi@mail.polimi.it

## Valutazione - 30
## Obiettivo 
Progetto volto all'applicazione pratica degli algoritmi e delle strutture dati affrontate nel corso di Algoritmi e Principi dell'Informatica, prestando attenzione ad aspetti concreti di efficienza del codice (tempo di esecuzione e memeoria usata).

## Realizzazione
- Linguaggio C
- Nessuna libreria esterna
- No multithreading
- dati in input ricevuti da stdin, risultati forniti via stdout 

## Struttura della repository
- Assignment : cartella contenente la specifica del progetto 
- my_tests: test forniti dal docente o condivisi da altri studenti per evidenziare gli errori di algoritmo e per testare di conseguenza il corretto funzionamento del programma
- utils : cartella contente pseudocodice di algoritmi affrontati durante il corso (non è garantito che il codice sia correttamente scritto)
- test_gen.py : generatore di test 
- WordChecker.c : codice principale del programma

## Progetto - WordChecker
L'obiettivo del progetto di quest'anno è di realizzare un sistema che, al 
suo cuore, controlla la corrispondenza tra le lettere di 2 parole di 
ugual lunghezza.<br>
Le parole sono intese come sequenze di simboli che possono essere 
caratteri alfabetici minuscoli (a-z) o maiuscoli (A-Z), cifre numeriche 
(0-9), oppure i simboli - (trattino) e _ ("underscore"):
 - esempio di parola di 20 simboli: djHD1af9fj7g__l-ssOP

<br>
Il sistema legge:

 - un valore k, che indica la lunghezza delle parole
 - una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, che 
costituisce l'insieme delle parole ammissibili
 - si dia pure per scontato che la sequenza di parole non contenga duplicati

<br>
A quel punto, viene letta da standard input una sequenza di "partite", 
in cui l'inizio di ogni nuova partita è marcato dal comando (letto 
sempre da input) +nuova_partita.
<br> Maggiori dettagli nella cartella "Assignment".

## Implementazione 
Nel mio caso, ho utilizzato alberi rosso-neri e strutture per contenere tutti i vincoli che le parole devono soddisfare. <br>
L'uso di un Flexible Array Member nella struttura dati ( `char key[];`), mi ha permesso di risparmiare molta memoria nella creazione dei nodi degli alberi.  