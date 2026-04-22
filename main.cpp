#include <iostream>
#include <string>
#include <cctype>	//per tolower
#include <fstream>  //per i files
#include <time.h>	//per numeri casuali
#include <stdlib.h>	//per numeri casuali
#include "setColor.h"	//per colorare il testo
using namespace std;
/*****PROTOTIPI******/
void landing();
void pulisci();
void orizzontale();
int menu();
string toLower(string s);
size_t codifica(string s);
bool verifica(size_t ID);
void schedaElettorale();
bool smistaVoti(string voto, int &contSi,int &contNo, int &contNulla, int &contBianca, bool &continua);
void lettura(int &Si, int &No, int &Nulla, int &Bianca,int &Affluenza, string nomeFile="urna voti.txt");
void scriviID(size_t ID);
void stampaVoti(int contSi,int contNo,int contNulla, int contBianca,int Affluenza);
void calcolaMassimo(int Si,int No);
void calcolaMassimo(int Si,int No,fstream &file);
void salvaSuFile(int si,int no,int bianca, int nulla);
void shuffle(int n,string nomi[],string cognomi[]);
/******************/
const int ELETTORI=1000;
int main(int argc, char** argv) {
	srand((unsigned) time(NULL));
	system("");
	setColor(Reset);
	//lista di nomi utilizzati nella simulazione
	string nomi[100]={
		"Alessandro", "Giulia", "Francesco", "Sofia", "Lorenzo", "Aurora", "Mattia", "Alice", "Leonardo", "Emma",
		"Riccardo", "Giorgia", "Tommaso", "Beatrice", "Gabriele", "Martina", "Edoardo", "Gaia", "Matteo", "Greta",
		"Federico", "Vittoria", "Andrea", "Ginevra", "Christian", "Chiara", "Davide", "Sara", "Giuseppe", "Anna",
		"Antonio", "Elena", "Marco", "Ludovica", "Samuele", "Noemi", "Pietro", "Nicole", "Filippo", "Matilde",
		"Simone", "Arianna", "Luca", "Margherita", "Manuel", "Viola", "Alessio", "Bianca", "Gioele", "Elisa",
		"Diego", "Camilla", "Daniele", "Marta", "Giovanni", "Isabel", "Giacomo", "Rachele", "Emanuele", "Miriam"
	};
	//lista di cognomi utilizzati nella simulazione
	string cognomi[100]={
		"Rossi", "Ferrari", "Russo", "Bianchi", "Esposito", "Colombo", "Romano", "Ricci", "Marini", "Greco",
		"Bruno", "Gallo", "Conti", "De Luca", "Mancini", "Costa", "Giordano", "Rizzo", "Lombardi", "Moretti",
		"Barbieri", "Fontana", "Santoro", "Mariani", "Rinaldi", "Caruso", "Ferrara", "Galli", "Martini", "Leone",
		"Longo", "Gentile", "Martinelli", "Vitale", "Lombardo", "Serra", "Coppola", "De Santis", "D'Angelo", "Marchetti",
		"Parisi", "Villa", "Conte", "Ferraro", "Ferri", "Fabbri", "Monti", "Palumbo", "Grasso", "Anna",
		"Pellegrini", "Carbone", "Giuliani", "Benedetti", "Barone", "Rossetti", "Caputo", "Montanari", "Guerra", "Silvestri"
	};
	int scelta;
	int n;
	string nomeCompleto;
	string uscita;
	size_t ID;		//size_t= numero massimo possibile in memoria
	string voto;
	bool continua;
	bool continuaPrincipale=true;
	int contSi=0,contNo=0,contBianca=0,contNulla=0,Affluenza=0;
	int totaleVoti;
	bool votoValido;
	bool confermaUscita=false;
	//landing page con funzionalita che puo fare l'utente
	//lettura iniziale del file delle votazioni
	do {
		
	landing();
	//leggere la scelta
	setColor(Cyan);
	cout<<"COSA VUOI FARE? ";
	cin>>scelta;
	//stampaVoti(contSi,contNo,contNulla,contBianca,Affluenza);
	if(scelta==1){
		lettura(contSi,contNo,contNulla,contBianca,Affluenza);	//inizialmente legge dal file preinizializzato i voti
		int sceltaSottoMenu;
		system("cls");
		do {
			//menu
			sceltaSottoMenu=menu();
			//inizialmente l'utente inserisce il nome del votante, poi viene convertito in lowercase e codificato tramite std::hash
			switch(sceltaSottoMenu){
				case 0:
				//torna al menu principale
				break;
				
				case 1:
				// aggiunta di nuovi voti
				continua=true;
				do {
					system("cls"); // pulisce lo schermo
					setColor(BiancoSplendente);
					cout<<"\n\n[ MENU DI VOTO - INSERIMENTO FILE ]\n\n";
					setColor(Cyan);
					cout<<"INSERISCI IL NOME COMPLETO DELL'ELETTORE\n";
					cout<<"(OPPURE 0 PER TORNARE AL MENU PRINCIPALE): ";
					setColor(Reset);
					fflush(stdin);
					getline(cin,nomeCompleto);
					
					if (nomeCompleto == "0" || nomeCompleto == ""){
						//tornare alla pagina precedente
						continua=false;
					}
					else {
						nomeCompleto=toLower(nomeCompleto);		//conversione in minuscolo, in questo modo "Mario Rossi" e "mario rossi" non sono due persone diverse
						ID=codifica(nomeCompleto);				//generazione di un codice univoco per ogni persona
						if (verifica(ID)){						//se il codice univoco esiste gia allora non puo piu votare
							setColor(Rosso);
							cout<<"\n\t[!] ATTENZIONE: QUESTA PERSONA HA GIA' VOTATO!\n\n";
							system("pause");
							setColor(Reset);
						}
						else {
							votoValido=false;
							continua=true; // reinizializza per la seconda parte
							do {
								schedaElettorale();	//pagina per votare
								cin>>voto;
								
								if(smistaVoti(voto,contSi,contNo,contNulla,contBianca,continua)){	//se il voto é diverso da q oppure si inserisce un voto valido
									votoValido=true;
									scriviID(ID);		//si scrive l'id della persona nel file
									system("cls");
									setColor(Verde);
									cout<<"\n\n\n\t\t[+] VOTO REGISTRATO CON SUCCESSO! GRAZIE.\n\n\n";
									setColor(Reset);
									system("pause");
								}
								else {	//voto invalido oppure q
									setColor(Rosso);
									if (voto!="q" && voto !="Q" && continua){
										cout<<"\n\t\t[!] ERRORE: VOTO NON VALIDO. RIPROVA.\n\n";
										system("pause");
									}
									setColor(Reset);
								}
							} 
							while(!votoValido && continua);
						}
					}
				} 
				while(continua);
				system("cls");
				break;
				
				case 2:
				{
					system("cls");
					lettura(contSi,contNo,contNulla,contBianca,Affluenza);		//leggere le votazioni dal file 
					if (Affluenza == 0) {
						setColor(Cyan);
						cout<<"\n==================================================\n";
						cout<<"|           STATISTICHE IN TEMPO REALE           |\n";
						cout<<"==================================================\n\n";
						setColor(Reset);
						cout<<"NESSUN VOTO REGISTRATO FINORA.\n\n";
					} else {
						stampaVoti(contSi,contNo,contNulla,contBianca,Affluenza);	//stampa ordinata dei voti con risultato
					}
					cout<<"\n";
					pulisci();
				}
				break;
				
				case 3:
				//salva su un file il risultato
				salvaSuFile(contSi,contNo,contBianca,contNulla);
				setColor(Verde);
				cout<<"\t\t[+] FILE CREATO CON SUCCESSO! (VEDI verbale.txt)\n";
				system("pause");
				system("cls");
				break;
				
				default:
				setColor(Rosso);
				cout<<"ERRORE, OPZIONE NON CONSENTITA, RIPROVA"<<endl;
				setColor(Reset);
				break;
			}//fine switch
		}
		while(sceltaSottoMenu!=0);	
	}
	else if (scelta==2){
		//mostrare l'andamento fino adesso
		system("cls");
		lettura(contSi,contNo,contNulla,contBianca,Affluenza);		//ipotizzando che non si sia fatto il case 1 leggere i voti dal file
		if (Affluenza == 0) {
			setColor(Cyan);
			cout<<"\n==================================================\n";
			cout<<"|           STATISTICHE IN TEMPO REALE           |\n";
			cout<<"==================================================\n\n";
			setColor(Reset);
			cout<<"NESSUN VOTO REGISTRATO FINORA.\n\n";
		} else {
			stampaVoti(contSi,contNo,contNulla,contBianca,Affluenza);	//stampa ordinata e risultato
		}
		cout<<"\n";
		system("pause");
	}	
	else if (scelta==3){
		//simulazione veloce di n votazioni
		system("cls");
		do {
			setColor(Cyan);
			cout<<"QUANTE PERSONE VUOI SIMULARE? ";
			setColor(Reset);
			cin>>n;	
			if (n<=0 || n>50){
				setColor(Rosso);
				cout<<"[!] PER IL MOMENTO IL RANGE DELLE SIMULAZIONI E' 1-50\n";
				setColor(Reset);
			}
		}
		while(n<=0 || n>50);
		
		fstream tempFile;
		tempFile.open("simulazione voti.txt", ios::out);
		shuffle(n,nomi,cognomi);
		for (int i=0; i<n; i++){	
			// 35% s, 35% n, 15% b, 15% x per un maggior realismo
			int votoCasuale = rand() % 100;	//numeri 0-99
			char votoScelto;
			if(votoCasuale < 35){
				votoScelto = 's';
			} 
			else if(votoCasuale < 70) {
				votoScelto = 'n';
			}
			else if(votoCasuale < 85){
				votoScelto = 'b';
			} 
			else {
				votoScelto = 'x';
			}		
			tempFile<<votoScelto<<endl;	//scrive sul file il voto
			system("cls");
		}
		tempFile.close();
		lettura(contSi,contNo,contNulla,contBianca,Affluenza,"simulazione voti.txt");	//legge il file della simulazione
		if (Affluenza == 0) {
			setColor(Cyan);
			cout<<"\n==================================================\n";
			cout<<"|           STATISTICHE IN TEMPO REALE           |\n";
			cout<<"==================================================\n\n";
			setColor(Reset);
			cout<<"NESSUN VOTO REGISTRATO FINORA.\n\n";
		} else {
			stampaVoti(contSi,contNo,contNulla,contBianca,Affluenza);
		}
		cout<<"\n";
		pulisci();
		
	}
	else if (scelta==0){
		//esci (chiedi conferma)
		continua=true;
		setColor(Rosso);
		cout<<"SEI SICURO/A DI VOLER USCIRE [s][N]? ";
		setColor(Reset);
		fflush(stdin);
		getline(cin,uscita);
		if (uscita=="" || uscita=="N" || uscita=="n"){
//			system("cls");
		}
		else if(uscita=="s" || uscita=="S"){
			setColor(Verde);
			cout<<"GRAZIE PER AVERE USATO GESTIONE VOTAZIONI"<<endl;
			setColor(Reset);
			continuaPrincipale=false;
//			system("pause");
			return 0;
		}
		else {
			setColor(Rosso);
			cout<<"[!] SCELTA NON CONSENTITA\n";
			setColor(Reset);
		}
	}
	else {
		//scelta non consentita
		setColor(Rosso);
		cout<<"\t [!] ERRORE,SCELTA NON CONSENTITA\n";
		setColor(Reset);
	}
	system("cls");
	}
	while(continuaPrincipale);
	return 0;
}
void landing(){
	setColor(Cyan);
	//ho trovato come mettere le ascii art su https://stackoverflow.com/questions/37765925/ascii-art-in-c
	cout<<R"(
	   ____           _   _                   __     __    _            _             _ 
	  / ___| ___  ___| |_(_) ___  _ __   ___  \ \   / /__ | |_ __ _ ___(_) ___  _ __ (_)
	 | |  _ / _ \/ __| __| |/ _ \| '_ \ / _ \  \ \ / / _ \| __/ _` |_  / |/ _ \| '_ \| |
	 | |_| |  __/\__ \ |_| | (_) | | | |  __/   \ V / (_) | || (_| |/ /| | (_) | | | | |
	  \____|\___||___/\__|_|\___/|_| |_|\___|    \_/ \___/ \__\__,_/___|_|\___/|_| |_|_|
	                                                                                    
	)";
	cout<<endl;
	cout<<"BENVENUTO IN GESTIONE VOTAZIONI, CON QUESTO SOFTWARE PUOI SIMULARE LA RACCOLTA VOTI DI UN REFERENDUM IN MODO SEMPLICE.\n";
	cout<<"ADESSO SCEGLI COSA FARE: \n";
	setColor(Verde);
	cout<<"[1] AVVIARE IL SOFTWARE PER INIZIARE A REGISTRARE I VOTI\n";
	cout<<"[2] VEDERE L'ANDAMENTO DEL REFERENDUM FINO AD ADESSO\n";
	cout<<"[3] SIMULAZIONE VELOCE DI N VOTAZIONI\n";
	setColor(Rosso);
	cout<<"[0] ESCI\n";
}

int menu(){
	char verticale=186;
	char a=201;
	char b=187;
	char c=200;
	char d=188;
	char e=204;
	char f=185;
	int scelta;
	setColor(Cyan);
	cout<<"\t\t"<<a;
	orizzontale();
	cout<<b<<endl;
	cout<<"\t\t"<<verticale<<"\t\t    GESTIONE VOTAZIONI:\t\t\t   "<<verticale<<endl;
	//riga sotto
	cout<<"\t\t"<<e;
	orizzontale();
	cout<<f<<endl;
	cout<<"\t\t"<<verticale<<"[1]. Inserisci nuovi voti al totale                       "<<verticale<<endl;
	cout<<"\t\t"<<verticale<<"[2]. Visualizza andamento attuale                         "<<verticale<<endl;
	cout<<"\t\t"<<verticale<<"[3]. Genera file di report                                "<<verticale<<endl;
	cout<<"\t\t"<<verticale;
	setColor(Rosso);
	cout<<"[0]. TORNA ALLA HOME                                      ";
	setColor(Cyan);
	cout<<verticale<<endl;
	cout<<"\t\t"<<c;
	orizzontale();
	cout<<d<<endl;
	cout<<"Che scelta vuoi fare? ";
	cin>>scelta;
	setColor(Reset);
	return scelta;
	
}

void orizzontale(){
	char oriz=205;
	for (int i=0; i<58; i++){
		cout<<oriz;
	}
}

void pulisci(){
	system("pause");
	system("cls");
}
//funzione che dato il nome e cognome della persona lo converte in caratteri minuscoli
string toLower(string s){
	for (int i=0; i<s.length(); i++){
		s[i]=tolower(s[i]);
	}
	return s;
}
//funzione che dato una stringa la codifica usando std::hash in modo univoco (documentazione trovata: https://tigercosmos.xyz/en/post/2024/12/c++/hash/)
size_t codifica(string s){
	size_t ris;
	hash<string> hashStringa;
	ris=hashStringa(s);
	return ris;
}
//apre il file dei codici univoci, se trova un riscontro la persona ha gia votato
bool verifica(size_t ID){
	bool corrispondenza=false;
	fstream file;
	file.open("elettori.txt",ios::in);
	string riga;
	while(getline(file,riga) && !corrispondenza){
		if (riga==to_string(ID)){
			corrispondenza=true;
		}
	}
	return corrispondenza;
}
//stampa di una breve schermata per scegliere il voto
void schedaElettorale(){
	system("cls");
	cout<<"\n\n";
	setColor(Cyan);
	cout<<"==================================================\n";
	cout<<"|                 SCHEDA ELETTORALE              |\n";
	cout<<"==================================================\n\n";
	setColor(Reset);
	
	setColor(Verde);
	cout<<"\t[S] SI\n\n";
	setColor(Rosso);
	cout<<"\t[N] NO\n\n";
	setColor(BiancoSplendente);
	cout<<"\t[B] BIANCA\n\n";
	setColor(Reset);
	cout<<"\t[X] NULLA\n\n";
	cout<<"\t-----------------------\n";
	setColor(RossoChiaro);
	cout<<"\t[Q] ANNULLA / ESCI\n\n";
	
	cout<<"";
	setColor(Cyan);
	cout<<"ESPRIMI LA TUA PREFERENZA: ";
	setColor(Reset);
}
//funzione che aggiorna i valori dei voti da un voto iniziale dato e stampa sul file
bool smistaVoti(string voto, int &contSi,int &contNo, int &contNulla, int &contBianca, bool &continua){
	if(voto=="s"|| voto=="s"){
		//voto si
		contSi++;
	}
	else if(voto=="n"|| voto=="N"){
		//voto no
		contNo++;
	}
	else if(voto=="b" || voto=="B"){
		//voto bianca
		contBianca++;
	}
	else if(voto=="x"|| voto=="X"){
		//voto nulla
		contNulla++;
	}
	else if(voto=="q"|| voto=="Q"){
		//ESCI
		continua=false;
		return false;
	}
	else if(voto.length()>1){
		return false;
	}
	else {
		//non valido
		setColor(Rosso);
		cout<<"ERRORE, VOTO NON CONSENTITO\n";
		setColor(Reset);
		return false;
	}
	
	fstream file;
	file.open("urna voti.txt",ios::app);
	file<<voto<<endl;
	file.close();
	return true;
}
//funzione che apre il file "urna voti.txt"  e aggiorna i risultati
void lettura(int &Si, int &No, int &Nulla, int &Bianca,int &Affluenza, string nomeFile){
	Si=0;
	No=0;
	Nulla=0;
	Bianca=0;
	Affluenza=0;
	fstream file;
	file.open(nomeFile,ios::in);
	char voto;
	int contSchede=0;
	while(file>>voto){
		contSchede++;
		if(voto=='s'|| voto=='S'){
			//voto si
			Si++;
		}
		else if(voto=='n'|| voto=='N'){
			//voto no
			No++;
		}
		else if(voto=='b'|| voto=='B'){
			//voto bianca
			Bianca++;
		}
		else if(voto=='x'|| voto=='X'){
			//voto nulla
			Nulla++;
		}
	}
	Affluenza=Si+No+Bianca+Nulla;
}
//scrive l'id sul file
void scriviID(size_t ID){
	fstream file;
	file.open("elettori.txt",ios::app);
	file<<ID<<endl;
}
//stampa a schermo una pagina di riepilogo con tutte le informazioni principali, compreso anche chi ha vinto
void stampaVoti(int contSi,int contNo,int contNulla, int contBianca,int Affluenza){
	setColor(Cyan);
	cout<<"\n==================================================\n";
	cout<<"|           STATISTICHE IN TEMPO REALE           |\n";
	cout<<"==================================================\n\n";
	cout<<"[AFFLUENZA] "<<Affluenza<<" / "<<ELETTORI<<" ("<<(float)Affluenza/ELETTORI*100<<"%)\n\n";
	setColor(Reset);
	
	setColor(Verde);
	cout<<"[SI]\t\tVoti: "<<contSi<<"--->("<<(float)contSi/Affluenza*100<<"%)\n\n";
	
	setColor(Rosso);
	cout<<"[NO]\t\tVoti: "<<contNo<<"--->("<<(float)contNo/Affluenza*100<<"%)\n\n";
	
	setColor(BiancoSplendente);
	cout<<"[BIANCA]\tVoti: "<<contBianca<<"--->("<<(float)contBianca/Affluenza*100<<"%)\n\n";
	
	setColor(Reset);
	cout<<"[NULLA]\t\tVoti: "<<contNulla<<"--->("<<(float)contNulla/Affluenza*100<<"%)\n\n";
	setColor(Cyan);
	calcolaMassimo(contSi,contNo);
	setColor(Reset);
}
//calcola il vincitore solo tra si e no
void calcolaMassimo(int Si,int No){
	//vedere chi  é il piu grande tra si e no
	int tot=Si+No;
	cout<<"RISULTATO FINALE: ";
	if (No>Si){
		cout<<"HA VINTO IL NO ";
		cout<<"CON "<<No<<" VOTI/O: "<<(float)No/tot*100<<"%\n";
		cout<<"IL SI HA AVUTO "<<Si<<" VOTI/O: "<<(float)Si/tot*100<<"%\n";
	}
	else if (Si>No){
		cout<<"HA VINTO IL SI ";
		cout<<"CON "<<Si<<" VOTI/O: "<<(float)Si/tot*100<<"%\n";
		cout<<"IL NO HA AVUTO "<<No<<" VOTI/O: "<<(float)No/tot*100<<"%\n";
	}
	else {
		cout<<"PAREGGIO!\n";
		cout<<"IL SI E IL NO HANNO AVUTO LO STESSO NUMERO DI VOTI: 50%\n";
	}
}
//stessa funzione di quella sopra solo che differisce perche stampa sul file finale
void calcolaMassimo(int Si,int No,fstream &file){
	//vedere chi é  il piu grande tra si e no
	int tot=Si+No;
	file<<"RISULTATO FINALE: ";
	if (No>Si){
		file<<"HA VINTO IL NO\n";
		file<<"CON "<<No<<" VOTI: "<<(float)No/tot*100<<"%\n";
		file<<"IL SI HA AVUTO "<<Si<<" VOTI: "<<(float)Si/tot*100<<"%\n";
	}
	else if (Si>No){
		file<<"HA VINTO IL SI\n";
		file<<"CON "<<Si<<" VOTI: "<<(float)Si/tot*100<<"%\n";
		file<<"IL NO HA AVUTO "<<No<<" VOTI: "<<(float)No/tot*100<<"%\n";
	}
	else {
		file<<"PAREGGIO!\n";
		file<<"IL SI E IL NO HANNO AVUTO LO STESSO NUMERO DI VOTI: 50%\n";
	}
}
//stampa sul file "verbale.txt" il risultato del referendum
void salvaSuFile(int si,int no,int bianca, int nulla){
	int affluenza=si+no+bianca+nulla;
	fstream file;
	file.open("verbale.txt",ios::out);
	int totale=si+no+bianca+nulla;
	char oriz='=';
	for (int i=0; i<58; i++){
		file<<oriz;
	}
	file<<endl;
	file<<"|\t\t\tVERBALE SCRUTINIO REFERENDUM\t\t   |\n";
	for (int i=0; i<58; i++){
		file<<oriz;
	}
	file<<endl;
	file<<"Totale schede scrutinate: "<<totale<<endl;
	file<<"Affluenza: "<<affluenza<<"/"<<ELETTORI<<" ovvero "<<(float)affluenza/ELETTORI*100<<"%\n";
	for (int i=0; i<58; i++){
		file<<'-';
	}
	file<<endl;
	//voti singoli
	file<<"Numero totale di voti SI: "<<si<<"| "<<(float)si/totale*100<<"%"<<endl;
	file<<"Numero totale di voti NO: "<<no<<"| "<<(float)no/totale*100<<"%"<<endl;
	file<<"Numero totale di voti BIANCA: "<<bianca<<"| "<<(float)bianca/totale*100<<"%"<<endl;
	file<<"Numero totale di voti NULLI: "<<nulla<<"| "<<(float)nulla/totale*100<<"%"<<endl;
	for (int i=0; i<58; i++){
		file<<'-';
	}
	file<<endl;
	calcolaMassimo(si,no,file);
	for (int i=0; i<58; i++){
		file<<oriz;
	}
	file.close();
}

//funzione che mischia l'ordine dei nomi
void shuffle(int n,string nomi[],string cognomi[]){
	//due array, uno con i nomi e uno con gli indici, mischiare l'ordine degli indici
	string s;
	fstream fileSimulazione;
	fstream fileSimulazioneID;
	size_t ID;
	int indiceScambio=0;
	string temp;
	for (int i=n-1; i>0; i--){		//in questo modo partendo dal fondo e generando numeri da 0 a i-1 si riescono a mescolare nomi e cognomi senza ripetizioni
		indiceScambio=rand()%i;
		//nomi
		temp=nomi[i];
		nomi[i]=nomi[indiceScambio];
		nomi[indiceScambio]=temp;
		//-----
		indiceScambio=rand()%i;
		//cognomi
		temp=cognomi[i];
		cognomi[i]=cognomi[indiceScambio];
		cognomi[indiceScambio]=temp;
	}
	//stampa su file
	fileSimulazione.open("simulazione elettori.txt",ios::out);
	fileSimulazioneID.open("simulazione ID elettori.txt",ios::out);
	//codifica del nome appena generato casualmente e stampa sui due file per le simulazioni
	for (int i=0; i<n; i++){
		s=nomi[i]+" "+cognomi[i];
		fileSimulazione<<s<<endl;
		s=toLower(s);
		ID=codifica(s);
		fileSimulazioneID<<ID<<endl;
	}
	fileSimulazioneID.close();
	fileSimulazione.close();
	setColor(Verde);
	cout<<"[+] SIMULAZIONE VOTI AVVENUTA CON SUCCESSO\n";
	setColor(Reset);
	//return s;
}
