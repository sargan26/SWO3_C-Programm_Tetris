# **SWO3-Übungen - WS2023/24 - Übungszettel 5 - Ausarbeitung**

## **Beispiel 1**

### **Lösungsidee**

Eine grundlegende Entscheidung ist wohl, wie man die Tetrimino speichert. Ich habe mich dafür entschieden nur eine 'Anker'-Position zu speichern und eine 4x4 Matrix mit true oder false Werten repräsentieren dann die 7 verschiedenen Typen.

typedef struct {
    int x, y;
} position;

typedef struct {
    position pos;
    bool matrix[4][4];
    color color;
} block;

Die grundsätzliche Spielesteuerung ist ja schon implementiert mit einem case-statement. Beim Ereignis key_up ist die Logik zum Rotieren eines Tetriminos noch zu implementieren. In meiner Recherche, fand ich heraus, dass dies durch das Transponieren und Spiegeln der Matrix möglich ist. 

Die Kollisionsbehandlung ist auch anzupassen. Beispielweise beim Rotieren müssen für alle Einträge der Matrix die Positionen berechnet werden und überprüft werden, ob es keine Kollisionen zu anderen Tetriminos gibt bzw. zur Spielwand.

Um die Spielgeschwindigkeit anzupassen, werde ich mitzählen wie viel Blöcke bereits erstellt wurden. Dann bei zB. jedem 10. Block eine Funktion aus timer.c aufrufen, die die Geschwindigkeit erhöht.

Auch die Darstellung (Rendering) muss angepasst werden. Dabei iteriere ich über die Matrix und berechne für jeden Eintrag die Position, ausgehend von der 'Anker'-Position. 

### **Testfälle**

Leider ist mir am Ende, als ich die Testfälle erstellen wollte, etwas mit dem Renderer passiert, was ich nicht mehr beheben konnte. 
Grundsätzlich implementiert und teilweise auch getestet, ist folgendes:

Erzeugt zufällig einen der 7 Tetriminos mit entsprechender Farbe

Überprüft bei try_move auf mögliche Kollisionen

Rotiert den Tetriminos mit key_up

Zähler der die Anzahl der Blöcke zählt und eine Funktion von timer.c aufruft um die Geschwindigkeit zu erhöhen

