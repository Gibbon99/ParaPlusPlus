//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create German text strings
void as_setItalianText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText("scrollBoxIntroText",		"  ^ ^       Benvenuto in Paradosso. ! ! Premere [LEFT CTRL KEY] per giocare o attendere ulteriori istruzioni! Usa i [TASTI FRECCIA] per navigare nei menu! Premere [ENTER] per effettuare una scelta! ! ^ Libera il cargo dei robot distruggendoli con laser gemelli o trasferendo il controllo su di essi. ! ! Il controllo avviene solo tramite tastiera, come segue:! ! In ogni momento la tastiera muove il robot e tenendo premuto il fuoco consentirà l'uso di ascensori e console. ! ! Inoltre, premendo il pulsante di accensione senza premere i tasti, verrà preparato il robot corrente per il trasferimento. Il contatto con un altro robot con il tasto di accensione giù avvierà il trasferimento. ! ! ! ^! ! ! ! Una flotta di Mercanti Robo in viaggio verso il sistema Beta Ceti ha riferito di aver inserito un campo inesplorato di asteroidi. Ogni nave trasporta un carico di droidi da battaglia per rafforzare le difese esterne. ! ! Due fari di soccorso sono stati recuperati. Messaggi simili sono stati memorizzati su ciascuno. Le navi erano state bombardate da un potente raggio radionico proveniente da uno degli asteroidi. ! ! Tutti i robot sulle navi, compresi quelli in deposito, sono diventati iper-attivi. Gli equipaggi riferiscono di un attacco da parte di droidi, isolandoli sul ponte. Non possono raggiungere lo shuttle e possono resistere solo per un paio d'ore. ! ! Dato che questi beacon sono stati localizzati due giorni fa, possiamo solo temere il peggio. ! ! Parte della flotta è stata vista per l'ultima volta in direzione dello spazio nemico. Nelle mani dei nemici i droidi possono essere usati contro le nostre forze. ! ! L'attracco sarebbe impossibile, ma possiamo trasmettere a bordo di un prototipo di dispositivo di influenza. ! ^! ! ! Il dispositivo 001 Influence è costituito da un casco che, quando posizionato su un'unità di controllo del robot, può interrompere le normali attività di quel robot per un breve periodo. Il casco ha il proprio alimentatore e alimenta il robot stesso, con capacità potenziate. Il casco utilizza anche un mantello energetico per la protezione dell'ospite. ! ! Il casco è equipaggiato con laser gemelli montati in una corrente. Questi sono a bassa potenza e hanno un tasso di riciclaggio lento. ! ! La maggior parte delle risorse del dispositivo vengono indirizzate verso il mantenimento del controllo del robot host, nel tentativo di riprendere il normale funzionamento. È quindi necessario cambiare spesso il robot host per evitare che il dispositivo si scarichi. Il trasferimento a un nuovo robot richiede che il dispositivo prosciughi la sua energia per poterlo prendere. Il mancato raggiungimento dei risultati di trasferimento nel dispositivo è di nuovo un agente libero. ! ! Ulteriori informazioni in arrivo ...! ^! ! ! Un dispositivo Influence può trasmettere solo determinati dati, vale a dire la sua posizione e la posizione di altri robot nel raggio visivo. Questi dati vengono uniti con i layout della nave noti sul terminale remoto C64. ! ! Ulteriori informazioni sulla nave e sui robot possono essere ottenute accedendo al computer della nave su una console. È disponibile una planimetria su scala ridotta dell'intero mazzo, oltre a un prospetto laterale della nave. ! ! I robot sono rappresentati sullo schermo come un simbolo che mostra un numero di tre cifre. La prima cifra mostrata è quella importante, la classe del robot. Denota anche la forza. ! ! Per saperne di più su un determinato robot, utilizzare il sistema di ispezione del robot su una console. Sono disponibili solo i dati relativi alle unità di una classe inferiore rispetto all'host corrente, in quanto è il nulla osta di sicurezza dell'host utilizzato per accedere alla console. ! ! ! ! ! Ulteriori informazioni in arrivo ...! ^! ! ! ! ! Paradroid programmato da Andrew Braybrook. ! ! ! Un buon consiglio di Steve Turner. ! ! ! Porta SDL2.0 di David Berry. ! 2019.! ! ! ! ! Alimentato dalla libreria SDL2. ! Motore di scripting di AngelScript. ! Fisica di Box2D. ! Filewatcher di James Wynn 2009.! ! ! ! Premi il fuoco per giocare. ! ! ! ! ! ! ^ # ");
	gui_addKeyAndText("gameLeft", "Muovere a sinistra");
	gui_addKeyAndText("gameRight", "Vai a destra");
	gui_addKeyAndText("gameDown", "Abbassati");
	gui_addKeyAndText("gameUp", "Andare avanti");
	gui_addKeyAndText("gameAction", "Esegui un'azione");
	gui_addKeyAndText("gamePause", "Metti in pausa il gioco");
	gui_addKeyAndText("gameEscape", "Fuga");
	gui_addKeyAndText("consoleAction", "Richiama la console");
	gui_addKeyAndText("gameScreenShot", "Fai uno screenshot");

	gui_addKeyAndText ("backButton", "Back");

	gui_addKeyAndText ("mainMenu.startGameButton", "Inizia il gioco");
	gui_addKeyAndText ("mainMenu.optionsButton", "Opzioni");
	gui_addKeyAndText ("mainMenu.tutorialButton", "lezione");
	gui_addKeyAndText ("mainMenu.briefingButton", "Riunione");
	gui_addKeyAndText ("mainMenu.quitButton", "Abbandonare il gioco");
}
