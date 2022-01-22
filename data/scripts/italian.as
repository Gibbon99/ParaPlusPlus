//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create Italian text strings
void as_setItalianText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText ("scrollBoxIntroText", "! Benvenuto in Paradroid. ! ! Premi [TASTO CTRL SINISTRO] per giocare o attendi ulteriori istruzioni! Utilizzare i [TASTI FRECCIA] per navigare nei menu! Premere [TASTO CTRL SINISTRO] per fare una scelta! ! ^ Elimina i robot dal mercantile distruggendoli con due laser o trasferendogli il controllo. ! ! Il controllo è solo da tastiera, come segue: ! ! In ogni momento la tastiera muove il robot e tenere premuto il fuoco consentirà l'uso di ascensori e console. ! ! Inoltre, premendo il fuoco senza tasti premuti si preparerà il robot corrente per il trasferimento. Il contatto con un altro robot con la chiave di fuoco abbassata avvierà il trasferimento. ! ! ! ^! ! ! ! Una flotta di Robo-Freighter in viaggio verso il sistema Beta Ceti ha riferito di essere entrata in un campo inesplorato di asteroidi. Ogni nave trasporta un carico di droidi da battaglia per rafforzare le difese del mondo esterno. ! ! Sono stati recuperati due segnali di soccorso. Messaggi simili sono stati memorizzati su ciascuno. Le navi erano state bombardate da un potente raggio radionico proveniente da uno degli asteroidi. ! ! Tutti i robot sulle navi, compresi quelli in deposito, sono diventati iperattivi. Gli equipaggi segnalano un attacco da parte di droidi, isolandoli sul ponte. Non possono raggiungere la navetta e possono resistere solo per un altro paio d'ore. ! ! Dal momento che questi fari sono stati localizzati due giorni fa, possiamo solo temere il peggio. ! ! Parte della flotta è stata vista l'ultima volta dirigersi verso lo spazio nemico. Nelle mani del nemico i droidi possono essere usati contro le nostre forze. ! ! L'attracco sarebbe impossibile, ma possiamo teletrasportarci a bordo di un prototipo di dispositivo di influenza. ! ^! ! ! Il dispositivo 001 Influence è costituito da un casco che, se posizionato sopra l'unità di controllo di un robot, può interrompere le normali attività di quel robot per un breve periodo. Il casco ha il proprio alimentatore e alimenta il robot stesso, con una capacità potenziata. Il casco utilizza anche un mantello energetico per la protezione dell'ospite. ! ! Il casco è dotato di due laser montati in una torretta. Questi sono a bassa potenza e hanno una velocità di riciclo lenta. ! ! La maggior parte delle risorse del dispositivo vengono convogliate per mantenere il controllo del robot host, mentre tenta di riprendere il funzionamento \"normale\". È quindi necessario cambiare spesso il robot host per evitare che il dispositivo si bruci. Il trasferimento a un nuovo robot richiede che il dispositivo prosciughi la sua energia per prenderne il controllo. Il mancato raggiungimento del trasferimento fa sì che il dispositivo sia nuovamente un agente libero. ! ! Ulteriori informazioni in arrivo... ! ^! ! ! Un dispositivo Influence può trasmettere solo determinati dati, vale a dire la propria posizione e la posizione di altri robot nel raggio visivo. Questi dati vengono uniti con i layout della nave noti sul terminale remoto C64. ! ! Ulteriori informazioni sulla nave e sui robot possono essere ottenute accedendo al computer della nave da una console. È disponibile una pianta in scala ridotta dell'intero ponte, nonché un prospetto laterale della nave. ! ! I robot sono rappresentati sullo schermo come un simbolo che mostra un numero a tre cifre. La prima cifra mostrata è quella importante, la classe del robot. Denota anche forza. ! ! Per saperne di più su un determinato robot, utilizza il sistema di richiesta robot su una console. Sono disponibili solo i dati sulle unità di una classe inferiore rispetto all'host attuale, poiché è il nulla osta di sicurezza dell'host che viene utilizzato per accedere alla console. ! ! ! ! ! Ulteriori informazioni in arrivo... ! ^! ! ! ! ! Paradroid programmato da Andrew Braybrook. ! ! ! Buoni consigli di Steve Turner. ! ! ! Porta SDL2 di David Berry. ! 2019. ! ! ! ! ! Basato sulla libreria SDL2.0. ! Motore di scripting di AngelScript. ! Fisica di Chipmunk2D. ! Filewatcher di James Wynn 2009. ! ! ! ! Premi il fuoco per giocare. ! ! ! Considera una donazione per aiutare a sviluppare più giochi e più navi. ! ! PayPal.Me/DBerry999 ! ! ! Dedicato a Jane. ! ! ^ #");
	gui_addKeyAndText ("scrollBoxWonText", " COMPLIMENTI! ! Hai sconfitto tutti i droidi su Ship Paradroid. ! ! Grazie per aver giocato al mio remake del classico gioco per Commodore 64 Paradroid di Andrew Braybrook. ! ! Sulla versione C64 ci sono altre sette navi da sconfiggere. Tuttavia, questo gioco ha già richiesto molte, molte, molte, molte ore di lavoro. Se desideri vedere più navi, prendi in considerazione una donazione per aiutare a finanziare lo sviluppo a ! ! PayPal.Me/DBerry999 ! ! oppure puoi raggiungermi a ! ! paradroidplusplus@gmail.com. ! ! ! ^ # ");
	gui_addKeyAndText ("db_001", "! Descrizione: 001 - Dispositivo di influenza! ClassName: Influenza! Guida: Nessuno! Cervello: Nessuno! Arma: Laser! Sensore 1: -! Sensore 2: -! Sensore 3: -! Note : Dispositivo di influenza dell'attività del robot. Questo casco è autoalimentato e controllerà qualsiasi robot per un breve periodo. I laser sono montati su torretta. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_123", "! Descrizione: 123 - Smaltimento Droid! ClassName: Smaltimento! Guida: tracce! Cervello: Nessuno! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: Infrarossi! Sensore 3: -! Note : Robot semplice per lo smaltimento dei rifiuti. Dispositivo comune nella maggior parte dei veicoli spaziali per mantenere una nave pulita. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_139", "! Descrizione: 139 - Smaltimento Droide! ClassName: Smaltimento! Guida: Antigravitazionale! Cervello: Nessuno! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Creato dal Dr. Masternak per ripulire grandi cumuli di spazzatura. La sua paletta serve per raccogliere i rifiuti. Viene poi schiacciato internamente. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_247", "! Descrizione: 247 - Droide servitore! ClassName: Servo! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Robot servitore leggero. Uno dei primi ad utilizzare il sistema antigravitazionale. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_249", "! Descrizione: 249 - Droide servitore! ClassName: Servo! Guida: Tripedale! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note: versione più economica del robot servo antigravitazionale. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_296", "! Descrizione: 296 - Droide servitore! ClassName: Servo! Guida: tracce! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Questo robot viene utilizzato principalmente per servire bevande. Un vassoio è montato sulla testa. Costruito da Orchard e Marsden Enterprises. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_302", "! Descrizione: 302 - Messenger Droid! ClassName: Messenger! Guida: Antigravitazionale! Cervello: Nessuno! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Dispositivo comune per lo spostamento di piccoli pacchi. Il morsetto è montato sul corpo inferiore. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_329", "! Descrizione: 329 - Messenger Droid! ClassName: Messenger! Guida: ruote! Cervello: Nessuno! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note: Robot messaggero di primo tipo. Le grandi ruote impediscono il movimento su piccole imbarcazioni. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_420", "! Descrizione: 420 - Droide di manutenzione! ClassName: Manutenzione! Guida: tracce! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Robot a manutenzione lenta. Confinato per guidare la manutenzione durante il volo. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_476", "! Descrizione: 476 - Droide di manutenzione! ClassName: Manutenzione! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: Infrarossi! Sensore 3: -! Note : Robot per la manutenzione della nave. Dotato di più bracci per eseguire riparazioni alla nave in modo efficiente. Tutte le imbarcazioni costruite dopo l'incidente di Giove vengono fornite con una squadra di queste. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_493", "! Descrizione: 493 - Droide della manutenzione! ClassName: Manutenzione! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Droide per la manutenzione degli schiavi. La versione standard avrà la propria cassetta degli attrezzi. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_516", "! Descrizione: 516 - Droide dell'equipaggio! ClassName: Equipaggio! Guida: Bipede! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : I primi droidi dell'equipaggio. In grado di effettuare solo semplici controlli di volo. Non più fornito. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_571", "! Descrizione: 571 - Droide dell'equipaggio! ClassName: Equipaggio! Guida: Bipede! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Droide dell'equipaggio standard. Fornito con la nave. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_598", "! Descrizione: 598 - Crew Droid! ClassName: Equipaggio! Guida: Bipede! Cervello: Neutronico! Arma: Nessuno! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Un dispositivo altamente sofisticato. In grado di controllare da solo il Robo-Freighter. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_614", "! Descrizione: 614 - Droide sentinella! ClassName: Sentinella! Guida: Bipede! Cervello: Neutronico! Arma: fucile laser! Sensore 1: Spettrale! Sensore 2: Subsonico! Sensore 3: -! Note : Droide sentinella a bassa sicurezza. Utilizzato per proteggere le aree della nave dagli intrusi. Un dispositivo lento ma sicuro. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_615", "! Descrizione: 615 - Droide sentinella! ClassName: Sentinella! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: Infrarossi! Sensore 3: -! Note : Sofisticato droide sentinella. Solo 2000 costruito dalla società Nicholson. Questi ora sono molto rari. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_629", "! Descrizione: 629 - Droide sentinella! ClassName: Sentinella! Guida: tracce! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: Subsonico! Sensore 3: -! Note : Droide sentinella lento. I laser sono integrati nella torretta. Questi sono montati su un piccolo corpo del serbatoio. Può essere dotato di un cannone automatico sulla versione Gillen. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_711", "! Descrizione: 711 - Battle Droid! ClassName: Battaglia! Guida: Bipdeal! Cervello: Neutronico! Arma: Disgregatore! Sensore 1: Ultrasuoni! Sensore 2: radar! Sensore 3: -! Note : Droide da battaglia pesante. Il disgregatore è integrato nella testa. Uno dei primi in servizio con i militari. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_742", "! Descrizione: 742 - Battle Droid! ClassName: Battaglia! Guida: Bipede! Cervello: Neutronico! Arma: Disgregatore! Sensore 1: Spettrale! Sensore 2: radar! Sensore 3: -! Note : Questa versione è quella usata principalmente dai Militari. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_751", "! Descrizione: 751 - Battle Droid! ClassName: Battaglia! Guida: Bipede! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: -! Sensore 3: -! Note : Droide da battaglia molto pesante. Finora solo pochi sono entrati in servizio. Queste sono le unità da battaglia più potenti mai costruite. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_821", "! Descrizione: 821 - Droide di sicurezza! ClassName: Sicurezza! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: radar! Sensore 3: Infrarossi! Note : Su questo droide è montata un'unità antigravitazionale molto affidabile. Pattuglierà la nave ed eliminerà gli intrusi non appena rilevati da potenti sensori. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_834", "! Descrizione: 834 - Droide di sicurezza! ClassName: Sicurezza! Guida: Antigravitazionale! Cervello: Neutronico! Arma: Laser! Sensore 1: Spettrale! Sensore 2: radar! Sensore 3: -! Note : Droide di sicurezza antigravitazionale di primo tipo. Dotato di un'unità antigravitazionale overdrive. Questo droide è molto veloce ma non è affidabile. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_883", "! Descrizione: 883 - Droide di sicurezza! ClassName: Sicurezza! Guida: ruote! Cervello: Neutronico! Arma: Sterminatore! Sensore 1: Spettrale! Sensore 2: radar! Sensore 3: -! Note : Questo droide è stato progettato da dati di archivio. Per qualche ragione sconosciuta instilla grande paura negli avversari Umani. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");
	gui_addKeyAndText ("db_999", "! Descrizione: 999 - Comando cyborg! NomeClasse: Comando! Guida: Antigravitazionale! Cervello: Primode! Arma: Laser! Sensore 1: Infrarossi! Sensore 2: radar! Sensore 3: Subsonico! Note : Cyborg di comando sperimentale. Dotato di un nuovo tipo di cervello. Montato su un'unità antigravitazionale Security Droid per comodità. Avvertenza: il dispositivo di influenza potrebbe non controllare a lungo un cervello primordiale. ! ! ! ! ! ! Le informazioni si ripetono! ! ! ! !");

	gui_addKeyAndText ("scrTutorial.tutMoveLabel", "Esercitazione - Sposta");
	gui_addKeyAndText ("scrTutTransfer.tutTransferLabel", "Esercitazione - Modalità di trasferimento");
	gui_addKeyAndText ("scrTutTransGame.tutTransGameLabel", "Tutorial - Trasferimento di gioco");
	gui_addKeyAndText ("scrTutLifts.tutLiftLabel", "Tutorial - Ascensori");
	gui_addKeyAndText ("scrTutTerminals.tutTerminalLabel", "Tutorial - Terminali");
	gui_addKeyAndText ("scrTutHealing.tutHealingLabel", "Tutorial - Guarigione");
	gui_addKeyAndText ("scrTutTips.tutTipsLabel", "Tutorial - Suggerimenti di gioco");

	gui_addKeyAndText ("gameLeft", "Muovere a sinistra");
	gui_addKeyAndText ("gameRight", "Vai a destra");
	gui_addKeyAndText ("gameDown", "Abbassati");
	gui_addKeyAndText ("gameUp", "Andare avanti");
	gui_addKeyAndText ("gameAction", "Eseguire un'azione");
	gui_addKeyAndText ("gamePause", "Metti in pausa il gioco");
	gui_addKeyAndText ("gameEscape", "Fuga");
	gui_addKeyAndText ("consoleAction", "Richiama la console");
	gui_addKeyAndText ("gameScreenShot", "Fai uno screenshot");

	gui_addKeyAndText ("hudTransfer", "Trasferimento");
	gui_addKeyAndText ("hudMoving", "Mossa");
	gui_addKeyAndText ("hudGameOn", "Inizio partita");
	gui_addKeyAndText ("hudRecharging", "Ricaricare");
	gui_addKeyAndText ("hudLift", "Sollevare");
	gui_addKeyAndText ("hudBriefing", "Riunione");
	gui_addKeyAndText ("hudTutorial", "Esercitazione");
	gui_addKeyAndText ("hudPauseMode", "In pausa");
	gui_addKeyAndText ("hudOptions", "Opzioni");
	gui_addKeyAndText ("hudPressKey", "Premere il tasto");

	gui_addKeyAndText ("backButton", "Indietro");
	gui_addKeyAndText ("nextButton", "Prossima");
	gui_addKeyAndText ("cancelButton", "Annulla");

	gui_addKeyAndText ("optionsMenu.videoButton", "Video");
	gui_addKeyAndText ("optionsMenu.audioButton", "Audio");
	gui_addKeyAndText ("optionsMenu.controlsButton", "Controlli");
	gui_addKeyAndText ("optionsMenu.gameButton", "Gioco");
	gui_addKeyAndText ("audioOptions.volume", "Volume");
	gui_addKeyAndText ("audioOptions.enableSound", "Abilita l'audio");
	gui_addKeyAndText ("audioOptions.speakers", "Altoparlanti");
	gui_addKeyAndText ("audioOptions.heading", "OPZIONI AUDIO");

	gui_addKeyAndText ("videoOptions.heading", "OPZIONI VIDEO");
	gui_addKeyAndText ("videoOptions.renderers", "Rendering");
	gui_addKeyAndText ("videoOptions.display", "Schermo");
	gui_addKeyAndText ("videoOptions.windowed", "Finestra");
	gui_addKeyAndText ("videoOptions.fullscreen", "A schermo intero");
	gui_addKeyAndText ("videoOptions.desktop", "Desktop completo");

	gui_addKeyAndText ("videoOptions.nearest", "Più vicino");
	gui_addKeyAndText ("videoOptions.linear", "Lineare");
	gui_addKeyAndText ("videoOptions.best", "Migliore");
	gui_addKeyAndText ("videoOptions.scalequality", "Qualità in scala");

	gui_addKeyAndText ("videoOptions.usevsync", "Usa vsync");
	gui_addKeyAndText ("videoOptions.borderlesswindow", "Nessun telaio della finestra");
	gui_addKeyAndText ("videoOptions.highdpi", "Consenti DPI elevati");
	gui_addKeyAndText ("videoOptions.screeneffect", "Effetto CRT");
	gui_addKeyAndText ("videoOptions.applyButton", "Applicare");

	gui_addKeyAndText ("gameOptions.heading", "OPZIONI DI GIOCO");

	gui_addKeyAndText ("controlsOptions.heading", "CONTROLLI");

	gui_addKeyAndText ("mainMenu.startGameButton", "Inizia il gioco");
	gui_addKeyAndText ("mainMenu.optionsButton", "Opzioni");
	gui_addKeyAndText ("mainMenu.tutorialButton", "Esercitazione");
	gui_addKeyAndText ("mainMenu.briefingButton", "Riunione");
	gui_addKeyAndText ("mainMenu.quitButton", "Abbandonare il gioco");

	gui_addKeyAndText ("terminalMenu.terminalText", "Terminale accessibile");
	gui_addKeyAndText ("terminalMenu.logoffButton", "Disconnettersi");
	gui_addKeyAndText ("terminalMenu.databaseButton", "Banca dati");
	gui_addKeyAndText ("terminalMenu.deckviewButton", "Vista sul ponte");
	gui_addKeyAndText ("terminalMenu.shipviewButton", "Vista della nave");

	gui_addKeyAndText ("gameMenu.tileStyle", "Stile piastrella");
	gui_addKeyAndText ("gameMenu.tileColor", "Colore delle piastrelle");
	gui_addKeyAndText ("gameMenu.language", "Lingua");

	gui_addKeyAndText ("databaseScreen.previousButton", "Precedente");
	gui_addKeyAndText ("databaseScreen.nextButton", "Prossima");

	gui_addKeyAndText ("guiTransferOne.heading", "Trasferimento - Unità corrente");
	gui_addKeyAndText ("guiTransferOne.textbox", "Questa è l'unità che controlli attualmente.");

	gui_addKeyAndText ("guiTransferTwo.heading", "Trasferimento - Unità target");
	gui_addKeyAndText ("guiTransferTwo.textbox", "Questa è l'unità che desideri controllare. Preparati a tentare l'acquisizione del trasferimento.");

	gui_addKeyAndText ("selectSide", "Scegli il lato:");
	gui_addKeyAndText ("transferFinish", "Fine :");
	gui_addKeyAndText ("burntout", "Bruciato");
	gui_addKeyAndText ("transferFailed", "Trasferimento fallito");
	gui_addKeyAndText ("transferred", "Trasferito");
	gui_addKeyAndText ("deadlock", "Bloccato");

	gui_addKeyAndText ("lostTransmission", "TRASMISSIONE");
	gui_addKeyAndText ("lostTerminated", "TERMINATA");
	gui_addKeyAndText ("lostGameOver", "Gioco finito");
	gui_addKeyAndText ("wonGameOver", "Partita vinta");
	gui_addKeyAndText ("highScore", "Miglior punteggio");

	gui_addKeyAndText ("quitGameDialogbox", "Annulla trasmissione.");
	gui_addKeyAndText ("quitGameDialogbox.text", "Vuoi annullare questa trasmissione e tornare al menu?");
	gui_addKeyAndText ("quitGameDialogbox.confirmButton", "Confermare");
	gui_addKeyAndText ("quitGameDialogbox.denyButton", "Negare");

	gui_addKeyAndText ("hudCaptured", "Catturata");
	gui_addKeyAndText ("hudMainMenu", "Menu principale");

	gui_addKeyAndText ("highScoreDisplay.heading", "Miglior punteggio");
	gui_addKeyAndText ("hudHighscore", "Punteggio alto");
	gui_addKeyAndText ("highScoreEntry.achieved", "Hai ottenuto un punteggio alto.");
	gui_addKeyAndText ("highScoreEntry.initials", "Inserisci le tue iniziali.");
	gui_addKeyAndText ("emptyString", "...");
}
