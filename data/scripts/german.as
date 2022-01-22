//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create German text strings
void as_setGermanText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText("scrollBoxIntroText", " ! Willkommen bei Paradroid. ! ! Drücken Sie die [LINKE STRG-TASTE], um zu spielen, oder warten Sie auf weitere Anweisungen! Verwenden Sie die [PFEILTASTEN], um durch die Menüs zu navigieren! Drücken Sie die [LINKE STRG-TASTE], um eine Auswahl zu treffen! ! ^ Löschen Sie den Frachter Roboter, indem Sie sie mit Zwillingslasern zerstören oder ihnen die Kontrolle übertragen. ! ! Die Steuerung erfolgt nur über die Tastatur, wie folgt: ! ! Die Tastatur bewegt den Roboter jederzeit, und wenn Sie das Feuer gedrückt halten, können Sie Aufzüge und Konsolen verwenden. ! ! In Darüber hinaus bereitet das Drücken von Feuer ohne gedrückte Tasten den aktuellen Roboter für den Transfer vor. Der Kontakt mit einem anderen Roboter mit gedrückter Feuertaste leitet den Transfer ein. ! ! ! ^ ! ! ! ! Eine Flotte von Robo-Frachtern auf dem Weg zur Beta Ceti Das System meldete das Eindringen in ein unbekanntes Asteroidenfeld. Jedes Schiff trägt eine Ladung Kampfdroiden, um die Verteidigung der Außenwelt zu verstärken. ! ! Zwei Notsignale wurden geborgen. Auf jedem waren ähnliche Nachrichten gespeichert. Die Schiffe wurden von einem starken Radionikstrahl bombardiert einer der Asteroiden. ! ! Alle o f Die Roboter auf den Schiffen, einschließlich der gelagerten, wurden hyperaktiv. Die Besatzungen melden einen Angriff von Droiden und isolieren sie auf der Brücke. Sie können das Shuttle nicht erreichen und können nur noch ein paar Stunden durchhalten. ! ! Da diese Baken vor zwei Tagen geortet wurden, können wir nur das Schlimmste befürchten. ! ! Ein Teil der Flotte wurde zuletzt auf dem Weg in den feindlichen Raum gesehen. In feindlicher Hand können die Droiden gegen unsere Streitkräfte eingesetzt werden. ! ! Andocken wäre unmöglich, aber wir können an Bord eines Prototyps eines Einflussgeräts beamen. ! ^ ! ! ! Das 001 Influence-Gerät besteht aus einem Helm, der, wenn er über der Steuereinheit eines Roboters platziert wird, die normalen Aktivitäten dieses Roboters für kurze Zeit unterbrechen kann. Der Helm verfügt über eine eigene Stromversorgung und versorgt den Roboter selbst mit verbesserter Leistung. Der Helm verwendet auch einen Energiemantel zum Schutz des Hosts. ! ! Der Helm ist mit zwei Lasern ausgestattet, die in einem Turm montiert sind. Diese sind stromsparend und haben eine langsame Recyclingrate. ! ! Die meisten Ressourcen des Geräts werden darauf verwendet, die Kontrolle über den Host-Roboter zu übernehmen, während dieser versucht, den „normalen“ Betrieb wieder aufzunehmen. Es ist daher notwendig, den Host-Roboter häufig zu wechseln, um ein Durchbrennen des Geräts zu verhindern. Die Übertragung auf einen neuen Roboter erfordert, dass das Gerät seine Energie entzieht, um es zu übernehmen. Wenn die Übertragung nicht erreicht wird, ist das Gerät wieder ein Free Agent. ! ! Weitere Informationen in Kürze... ! ^ ! ! ! Ein Influence-Gerät kann nur bestimmte Daten übermitteln, nämlich den eigenen Standort und den Standort anderer Roboter in Sichtweite. Diese Daten werden mit bekannten Schiffslayouts auf Ihrem C64 Remote Terminal zusammengeführt. ! ! Zusätzliche Informationen über das Schiff und die Roboter können durch Zugriff auf den Schiffscomputer an einer Konsole erhalten werden. Ein kleiner Plan des gesamten Decks sowie eine Seitenansicht des Schiffes sind verfügbar. ! ! Roboter werden auf dem Bildschirm als Symbol mit einer dreistelligen Zahl dargestellt. Die erste angezeigte Ziffer ist die wichtige, die Klasse des Roboters. Es bedeutet auch Stärke. ! ! Um mehr über einen bestimmten Roboter zu erfahren, verwenden Sie das Roboterabfragesystem an einer Konsole. Es sind nur Daten über Einheiten einer niedrigeren Klasse als Ihr aktueller Host verfügbar, da es die Sicherheitsüberprüfung des Hosts ist, die für den Zugriff auf die Konsole verwendet wird. ! ! ! ! ! Weitere Informationen in Kürze... ! ^ ! ! ! ! ! Paradroid programmiert von Andrew Braybrook. ! ! ! Fundierte Ratschläge von Steve Turner. ! ! ! SDL2-Portierung von David Berry. ! 2019. ! ! ! ! ! Angetrieben von der SDL2.0-Bibliothek. ! Skript-Engine von AngelScript. ! Physik von Chipmunk2D. ! Filewatcher von James Wynn 2009. ! ! ! ! Drücken Sie Feuer, um zu spielen. ! ! ! Bitte erwägen Sie eine Spende, um bei der Entwicklung von mehr Spielen und mehr Schiffen zu helfen. ! ! PayPal.Me/DBerry999 ! ! ! Jane gewidmet. ! ! ^ # \"");
    gui_addKeyAndText ("scrollBoxWonText", " GLÜCKWUNSCH ! ! Du hast alle Droiden auf Ship Paradroid besiegt. ! ! Danke, dass Sie mein Remake des klassischen Commodore 64-Spiels Paradroid von Andrew Braybrook spielen. ! ! In der C64-Version gibt es weitere sieben Schiffe zu besiegen. Allerdings hat dieses Spiel schon viele, viele, viele, viele Stunden Arbeit gekostet. Wenn Sie mehr Schiffe sehen möchten, erwägen Sie bitte eine Spende zur Finanzierung der Entwicklung unter ! ! PayPal.Me/DBerry999 ! ! oder Sie erreichen mich unter ! ! paradroidplusplus@gmail.com. ! ! ! ^ #");
    gui_addKeyAndText ("db_001", "! Beschreibung : 001 - Gerät beeinflussen ! Klassenname: Einfluss! Antrieb: Keiner! Gehirn: Keine! Waffe: Laser! Fühler 1 : - ! Fühler 2 : - ! Fühler 3 : - ! Hinweise: Gerät zur Beeinflussung der Roboteraktivität. Dieser Helm ist autark und steuert für kurze Zeit jeden Roboter. Laser sind am Turm montiert. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_123", "! Beschreibung : 123 - Entsorgungsdroide ! Klassenname : Entsorgung ! Antrieb: Gleise! Gehirn: Keine! Waffe: Keine! Sensor 1: Spektral! Sensor 2 : Infrarot ! Fühler 3 : - ! Anmerkungen: Einfacher Müllentsorgungsroboter. Übliches Gerät in den meisten Raumfahrzeugen, um ein sauberes Schiff zu erhalten. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! ! ");
    gui_addKeyAndText ("db_139", "! Beschreibung : 139 - Entsorgungsdroide ! Klassenname : Entsorgung ! Antrieb: Anti-Grav! Gehirn: Keine! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Erstellt von Dr. Masternak, um große Müllberge zu beseitigen. Seine Schaufel wird zum Sammeln von Müll verwendet. Es wird dann innerlich zerkleinert. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! ! ");
    gui_addKeyAndText ("db_247", "! Beschreibung : 247 - Servant Droid ! Klassenname: Diener! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Leichter Dienerroboter. Einer der ersten, der das Anti-Grav-System einsetzte. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_249", "! Beschreibung : 249 - Servant Droid ! Klassenname: Diener! Antrieb: Dreibein! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Billigere Version des Anti-Grav-Dienerroboters. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_296", "! Beschreibung : 296 - Servant Droid ! Klassenname: Diener! Antrieb: Gleise! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Hinweise: Dieser Roboter wird hauptsächlich zum Servieren von Getränken verwendet. Auf dem Kopf ist ein Tablett montiert. Gebaut von Orchard und Marsden Enterprises. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_302", "! Beschreibung : 302 - Messenger Droid ! Klassenname: Bote! Antrieb: Anti-Grav! Gehirn: Keine! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Übliches Gerät zum Bewegen kleiner Pakete. Die Klemme ist am Unterkörper montiert. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
	gui_addKeyAndText ("db_329", "! Beschreibung : 329 - Messenger Droid ! Klassenname: Bote! Antriebsräder ! Gehirn: Keine! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Botenroboter des frühen Typs. Große Räder behindern die Bewegung auf kleinen Fahrzeugen. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_420", "! Beschreibung : 420 - Wartungsdroide ! Klassenname : Wartung ! Antrieb: Gleise! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Langsamer Wartungsroboter. Beschränkt auf die Wartung des Laufwerks während des Flugs. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
	gui_addKeyAndText ("db_476", "! Beschreibung : 476 - Wartungsdroide ! Klassenname : Wartung ! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Sensor 2 : Infrarot ! Fühler 3 : - ! Anmerkungen: Schiffswartungsroboter. Ausgestattet mit mehreren Armen, um Reparaturen am Schiff effizient durchzuführen. Alle nach dem Jupiter-Zwischenfall gebauten Schiffe werden mit einem solchen Team ausgestattet. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_493", "! Beschreibung : 493 - Wartungsdroide ! Klassenname : Wartung ! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Sklavenwartungsdroide. Die Standardversion wird eine eigene Toolbox mit sich führen. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_516", "! Beschreibung : 516 - Crew Droid ! Klassenname: Crew! Antrieb: Zweibeiner! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Früher Crew-Droide. Kann nur einfache Flugkontrollen durchführen. Nicht mehr geliefert. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_571", "! Beschreibung : 571 - Crew Droid ! Klassenname: Crew! Antrieb: Zweibeiner! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Standard-Crew-Droide. Wird mit dem Schiff geliefert. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_598", "! Beschreibung : 598 - Crew Droid ! Klassenname: Crew! Antrieb: Zweibeiner! Gehirn: Neutronisch! Waffe: Keine! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Ein hoch entwickeltes Gerät. Kann den Robo-Frachter alleine steuern. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_614", "! Beschreibung : 614 - Sentinel Droid ! Klassenname: Sentinel! Antrieb: Zweibeiner! Gehirn: Neutronisch! Waffe: Lasergewehr! Sensor 1: Spektral! Sensor 2: Unterschall! Fühler 3 : - ! Anmerkungen: Wächterdroide mit niedriger Sicherheit. Wird verwendet, um Bereiche des Schiffes vor Eindringlingen zu schützen. Ein langsames aber sicheres Gerät. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_615", "! Beschreibung : 615 - Sentinel Droid ! Klassenname: Sentinel! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Sensor 2 : Infrarot ! Fühler 3 : - ! Anmerkungen: Ausgeklügelter Sentinel-Droide. Nur 2000 gebaut von der Nicholson Corporation. Diese sind mittlerweile sehr selten. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_629", "! Beschreibung : 629 - Sentinel Droid ! Klassenname: Sentinel! Antrieb: Gleise! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Sensor 2: Unterschall! Fühler 3 : - ! Anmerkungen: Langsamer Sentinel-Droide. In den Turm sind Laser eingebaut. Diese sind auf einem kleinen Tankkörper montiert. Kann bei der Gillen-Version mit einer automatischen Kanone ausgestattet werden. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_711", "! Beschreibung : 711 - Battle Droid ! Klassenname: Kampf! Antrieb : Bipdeal ! Gehirn: Neutronisch! Waffe: Unterbrecher! Sensor 1 : Ultraschall ! Sensor 2: Radar! Fühler 3 : - ! Anmerkungen: Schwerlast-Kampfdroide. Disruptor ist in den Kopf eingebaut. Einer der ersten im Militärdienst. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_742", "! Beschreibung : 742 - Battle Droid ! Klassenname: Kampf! Antrieb: Zweibeinig! Gehirn: Neutronisch! Waffe: Unterbrecher! Sensor 1: Spektral! Sensor 2: Radar! Fühler 3 : - ! Hinweise: Diese Version wird hauptsächlich vom Militär verwendet. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_751", "! Beschreibung : 751 - Battle Droid ! Klassenname: Kampf! Antrieb: Zweibeiner! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Fühler 2 : - ! Fühler 3 : - ! Anmerkungen: Sehr robuster Kampfdroide. Bisher sind nur wenige in Dienst gestellt worden. Dies sind die mächtigsten Kampfeinheiten, die jemals gebaut wurden. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_821", "! Beschreibung : 821 - Sicherheitsdroide ! Klassenname: Sicherheit! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Sensor 2: Radar! Sensor 3 : Infrarot ! Anmerkungen: Dieser Droide ist mit einer sehr zuverlässigen Anti-Grav-Einheit ausgestattet. Es wird das Schiff patrouillieren und Eindringlinge beseitigen, sobald sie von leistungsstarken Sensoren erkannt werden. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_834", "! Beschreibung : 834 - Sicherheitsdroide ! Klassenname: Sicherheit! Antrieb: Anti-Grav! Gehirn: Neutronisch! Waffe: Laser! Sensor 1: Spektral! Sensor 2: Radar! Fühler 3 : - ! Anmerkungen : Anti-Grav-Sicherheitsdroide des frühen Typs. Ausgestattet mit einer übersteuerten Anti-Grav-Einheit. Dieser Droide ist sehr schnell, aber nicht zuverlässig. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_883", "! Beschreibung : 883 - Sicherheitsdroide ! Klassenname: Sicherheit! Antriebsräder ! Gehirn: Neutronisch! Waffe: Kammerjäger! Sensor 1: Spektral! Sensor 2: Radar! Fühler 3 : - ! Anmerkungen: Dieser Droide wurde aus Archivdaten entworfen. Aus unbekannten Gründen flößt es menschlichen Gegnern große Angst ein. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");
    gui_addKeyAndText ("db_999", "! Beschreibung : 999 - Befehl Cyborg! Klassenname : Befehl ! Antrieb: Anti-Grav! Gehirn: Primode! Waffe: Laser! Sensor 1 : Infrarot ! Sensor 2: Radar! Sensor 3: Unterschall! Anmerkungen: Experimenteller Befehls-Cyborg. Ausgestattet mit einem neuen Gehirntyp. Der Einfachheit halber auf einer Anti-Grav-Einheit von Security Droid montiert. Warnung: Das Einflussgerät kann ein Primode-Gehirn nicht lange kontrollieren. ! ! ! ! ! ! Informationen wiederholen sich! ! ! ! !");

	gui_addKeyAndText ("scrTutorial.tutMoveLabel", "Lernprogramm - Bewegen");
	gui_addKeyAndText ("scrTutTransfer.tutTransferLabel", "Lernprogramm - Übertragungsmodus");
	gui_addKeyAndText ("scrTutTransGame.tutTransGameLabel", "Lernprogramm - Übertragungsspiel");
	gui_addKeyAndText ("scrTutLifts.tutLiftLabel", "Lernprogramm - Aufzüge");
	gui_addKeyAndText ("scrTutTerminals.tutTerminalLabel", " - Terminals");
	gui_addKeyAndText ("scrTutHealing.tutHealingLabel", "Lernprogramm - Heilung");
	gui_addKeyAndText ("scrTutTips.tutTipsLabel", "Tutorial - Gameplay-Tipps");

	gui_addKeyAndText ("scrTutorial.tutMoveText", " Benutze die Pfeiltasten zur Bewegung. ! ! Durch Drücken der Aktionstaste wird in Ihre aktuelle Bewegungsrichtung geschossen. ! ! Halten Sie die Aktionstaste gedrückt, wenn keine anderen Tasten gedrückt werden, um den Übertragungsmodus zu starten.");
	gui_addKeyAndText ("scrTutTransfer.tutTransText", " Um einen anderen Droiden zu übernehmen, wechseln Sie in den Transfermodus. ! ! Halten Sie die Aktionstaste ohne andere Tasten gedrückt, um diesen Modus aufzurufen. Die Kollision mit einem anderen Droiden löst die Transfer-Subroutine aus.");
	gui_addKeyAndText ("scrTutTransGame.tutTransGameText", "Um die Kontrolle über einen anderen Droiden zu erlangen, müssen Sie das Circuit-Transfer-Spiel gewinnen. Wählen Sie die Seite aus, die Sie verwenden möchten. Drücken Sie die UP / DOWN-Tasten, um Ihren Token zu einem geeigneten Kreis zu bewegen. Drücken Sie die Aktionstaste, um diesen Stromkreis auszuwählen. Sie müssen mehr von Ihrer Farbe haben, bevor die Zeit abläuft, um zu gewinnen.");
	gui_addKeyAndText ("scrTutLifts.tutLiftText", "Um zwischen den Decks zu wechseln, benutzen Sie einen Aufzug. Ein Aufzug wird aktiviert, indem man über einer Aufzugskachel anhält und die Aktionstaste drückt. Verwenden Sie die Richtungstasten, um ein neues Level auszuwählen, und drücken Sie die Aktionstaste, um es auszuwählen.");
	gui_addKeyAndText ("scrTutTerminals.tutTerminalText", "Halten Sie an einem Terminal an und drücken Sie die Aktionstaste. Dadurch erhalten Sie Zugriff auf die Computersysteme des Schiffes. Der Zugriff auf die Droidendatenbank wird durch die Zugriffsebene Ihres aktuellen Droiden gesteuert.");
	gui_addKeyAndText ("scrTutHealing.tutHealingText", "Alle Droiden haben eine Gesundheitsanzeige, die sich auf ihnen dreht. Je langsamer sich dieser dreht, desto niedriger ist die Gesundheit des Droiden. Um Schäden zu reparieren, halte über einem Reparaturplättchen an. Ihre Punktzahl wird beim Reparieren automatisch abgezogen.");
	gui_addKeyAndText ("scrTutTips.tutTipsText", "- Die Kollision mit einem anderen Droiden wird beide beschädigen. ! - Einige Droiden sind immun gegen die Disruptor-Waffen. ! - Ihre Kontrolle über einen Droiden schwächt sich mit der Zeit ab. ! - Die erste Zahl auf einem Droiden zeigt seinen Rang an.");

	gui_addKeyAndText("gameLeft", "Geh nach links");
	gui_addKeyAndText("gameRight", "Nach rechts bewegen");
	gui_addKeyAndText("gameDown", "Sich abwärts bewegen");
	gui_addKeyAndText("gameUp", "Bewegen Sie sich nach oben");
	gui_addKeyAndText("gameAction", "Führen Sie eine Aktion aus");
	gui_addKeyAndText("gamePause", "Unterbrechen Sie das Spiel");
	gui_addKeyAndText("gameEscape", "Flucht");
	gui_addKeyAndText("consoleAction", "Rufen Sie die Konsole auf");
	gui_addKeyAndText("gameScreenShot", "Mach ein Bildschirmfoto");

	gui_addKeyAndText ("hudTransfer", "Transfer");
	gui_addKeyAndText ("hudMoving", "Umzug");
	gui_addKeyAndText ("hudGameOn", "Spiel weiter");
	gui_addKeyAndText ("hudRecharging", "Aufladen");
	gui_addKeyAndText ("hudLift", "Aufzug");
	gui_addKeyAndText ("hudBriefing", "Einweisung");
	gui_addKeyAndText ("hudTutorial", "Lernprogramm");
	gui_addKeyAndText ("hudPauseMode", "Angehalten");
	gui_addKeyAndText ("hudOptions", "Optionen");
	gui_addKeyAndText ("hudPressKey", "drücken Sie die Taste");

    gui_addKeyAndText ("backButton", "Zurück");
    gui_addKeyAndText ("nextButton", "Nächste");
    gui_addKeyAndText ("cancelButton", "Abbrechen");

	gui_addKeyAndText ("optionsMenu.videoButton", "Video");
	gui_addKeyAndText ("optionsMenu.audioButton", "Audio");
	gui_addKeyAndText ("optionsMenu.controlsButton", "Kontrollen");
	gui_addKeyAndText ("optionsMenu.gameButton", "Spiel");
	gui_addKeyAndText ("audioOptions.volume", "Volumen");
	gui_addKeyAndText ("audioOptions.enableSound", "Sound einschalten");
	gui_addKeyAndText ("audioOptions.speakers", "Sprecher");
	gui_addKeyAndText ("audioOptions.heading", "AUDIOOPTIONEN");

	gui_addKeyAndText ("videoOptions.heading", "VIDEOOPTIONEN");
	gui_addKeyAndText ("videoOptions.renderers", "Renderer");
	gui_addKeyAndText ("videoOptions.display", "Anzeige");
	gui_addKeyAndText ("videoOptions.windowed", "Fenster");
	gui_addKeyAndText ("videoOptions.fullscreen", "Vollbild");
	gui_addKeyAndText ("videoOptions.desktop", "Vollständiger Desktop");

	gui_addKeyAndText ("videoOptions.nearest", "Nächste");
	gui_addKeyAndText ("videoOptions.linear", "Linear");
	gui_addKeyAndText ("videoOptions.best", "Am besten");
	gui_addKeyAndText ("videoOptions.scalequality", "Qualität skalieren");
	gui_addKeyAndText ("videoOptions.usevsync", "Verwenden Sie vsync");
	gui_addKeyAndText ("videoOptions.borderlesswindow", "Kein Fensterrahmen");
	gui_addKeyAndText ("videoOptions.highdpi", "Hohe DPI zulassen");
	gui_addKeyAndText ("videoOptions.screeneffect", "CRT-Effekt");
	gui_addKeyAndText ("videoOptions.applyButton", "Sich bewerben");

	gui_addKeyAndText ("gameOptions.heading", "SPIELEINSTELLUNGEN");

	gui_addKeyAndText ("controlsOptions.heading", "STEUERUNG");

	gui_addKeyAndText ("mainMenu.startGameButton", "Spiel starten");
	gui_addKeyAndText ("mainMenu.optionsButton", "Optionen");
	gui_addKeyAndText ("mainMenu.tutorialButton", "Tutorial");
	gui_addKeyAndText ("mainMenu.briefingButton", "Anweisung");
	gui_addKeyAndText ("mainMenu.quitButton", "Spiel verlassen");

    gui_addKeyAndText ("terminalMenu.terminalText", "Terminalzugriff");
    gui_addKeyAndText ("terminalMenu.logoffButton", "Abmelden");
    gui_addKeyAndText ("terminalMenu.databaseButton", "Datenbank");
    gui_addKeyAndText ("terminalMenu.deckviewButton", "Deckansicht");
    gui_addKeyAndText ("terminalMenu.shipviewButton", "Schiffsansicht");

	gui_addKeyAndText ("gameMenu.tileStyle", "Fliesenstil");
	gui_addKeyAndText ("gameMenu.tileColor", "Fliesenfarbe");
	gui_addKeyAndText ("gameMenu.language", "Sprache");

	gui_addKeyAndText ("databaseScreen.previousButton", "Vorherige");
	gui_addKeyAndText ("databaseScreen.nextButton", "Nächste");

    gui_addKeyAndText ("guiTransferOne.heading", "Übertragung - Aktuelle Einheit");
    gui_addKeyAndText ("guiTransferOne.textbox", "Dies ist die Einheit, die Sie derzeit steuern.");

    gui_addKeyAndText ("guiTransferTwo.heading", "Übertragung - Zieleinheit");
    gui_addKeyAndText ("guiTransferTwo.textbox", "Dies ist die Einheit, die Sie steuern möchten. Bereiten Sie sich darauf vor, eine Transferübernahme zu versuchen.");

	gui_addKeyAndText ("selectSide", "Seite auswählen:");
	gui_addKeyAndText ("transferFinish", "Beenden :");
	gui_addKeyAndText ("burntout", "Ausgebrannt");
	gui_addKeyAndText ("transferFailed", "Übertragung fehlgeschlagen");
	gui_addKeyAndText ("transferred", "Übertragen");
	gui_addKeyAndText ("deadlock", "Festgefahren");

	gui_addKeyAndText ("lostTransmission", "ÜBERTRAGUNG");
	gui_addKeyAndText ("lostTerminated", "BEENDET");
	gui_addKeyAndText ("lostGameOver", "Spiel ist aus");
	gui_addKeyAndText ("wonGameOver", "Spiel gewonnen");
	gui_addKeyAndText ("highScore", "Highscores");

	gui_addKeyAndText ("quitGameDialogbox", "Übertragung abbrechen.");
	gui_addKeyAndText ("quitGameDialogbox.text", "Möchten Sie diese Übertragung abbrechen und zum Menü zurückkehren?");
	gui_addKeyAndText ("quitGameDialogbox.confirmButton", "Bestätigen Sie");
	gui_addKeyAndText ("quitGameDialogbox.denyButton", "Aberkennen");

	gui_addKeyAndText ("hudCaptured", "Gefangen");
	gui_addKeyAndText ("hudMainMenu", "Hauptmenü");

	gui_addKeyAndText ("highScoreDisplay.heading", "Highscores");
	gui_addKeyAndText ("hudHighscore", "Highscore");
	gui_addKeyAndText ("highScoreEntry.achieved", "Sie haben eine hohe Punktzahl erreicht.");
	gui_addKeyAndText ("highScoreEntry.initials", "Geben Sie Ihre Initialen ein.");
	gui_addKeyAndText ("emptyString", "...");
}