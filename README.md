# **SWO3-Übungen - WS2023/24 - Übungszettel 5**

## **Beispiel 1: Tetris**

Vervollständigen Sie die in der Übung begonnene GLFW-Applikation "Tetris".
Beachten Sie dabei die folgenden Anforderungen und Hinweise:

1. Es müssen prinzipiell beliebig geformte Spielsteine (Tetriminos) unterstützt
   werden. Ein Spielstein ist dabei eine Matrix aus eingefärbten "Pixeln"
   (Blöcke). Natürlich gibt es in Ihrer Applikation einen vordefinierten Satz
   mit den sieben bekannten Tetriminos I, J, L, O, S, T und Z.

1. Tetriminos müssen die Bewegungen "links", "rechts", "drehen" und "fallen"
   durchführen können.

1. Führen Sie eine entsprechende Kollisionsbehandlung durch. Nur die
   Berücksichtigung eines "minimal umgebenden Rechtecks" ist nicht ausreichend.

1. Komplett gefüllte Reihen verschwinden vom Bildschirm.

1. Die Fallgeschwindigkeit der Tetriminos erhöht sich mit Fortgang eines Spiels.

1. Ein Spiel endet, sobald die nicht abgebauten Reihen den Bildschirm füllen.

1. Strukturieren Sie sauber, indem Sie Module und (Hilfs-)Funktionen schreiben.
   Auch die in der Übungsstunde vorgezeigten und implementierten Funktionen
   können noch wesentlich besser strukturiert werden.

1. Siehe auch die Quellen <https://en.wikipedia.org/wiki/Tetris> und
   <https://tetris.fandom.com/wiki/Tetris_Guideline>. Die Quelle
   <https://tetris.fandom.com/wiki/SRS> ist ebenfalls hilfreich.
