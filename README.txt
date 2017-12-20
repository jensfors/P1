README-fil til program med automatisk highlight og chatfiltrering i en Twitch Chat

// Oversættelse af programmet //
Programmet oversættes med 'gcc chatanalyser.c' ved at bruge en GNU compiler 


// Her vil menu funktionerne beskrives //
Programmet kan derefter køres og man vil blive promptet til en hovedmenu med tre muligheder
Den første mulighed er:

1. Live chat indstillinger
	
	Under denne indstilling bliver du promptet for en undermenu med forskellige muligheder
	(1) Emotemenu (Muligheden for at vælge emotes til emote streaks og highlights)
	(2) Gem chatten til en fil (Redigere hvilken fil chatten gemmes til, standard = dato)
	(3) Kun spørgsmål (Programmet vil kun vise spørgsmål sendt i chatten)
	(4) Kun highlight (Programmet vil kun vise tidspunkter, hvor der er et highlight)
	(5) Vi spørgsmål og highlights (Programmet viser på highlights og spørgsmål) 
	(6) Gå tilbage til hovedmenuen

Den anden mulighed i hovedmenuen er:
2. Offline indstillinger

	Under denne indstilling vil du først blive promptet for hvilken chatfil skal bruges
	(1) Nuværende chatfil (Bruger chatfilen fra den sidste livechat)
	(2) Vælg en anden fil (Vælg selv en chatfil)

	Efter chatfil valg promptes en ny undermenu
	(1) Emotemenu (Muligheden for at vælge emotes til emote streaks og highlights)
	(2) Søg chat (Søg efter brugere eller chatbeskeder fra chatten)
	(3) Vis spørgsmål (Viser alle spørgsmål stillet i den chatfil)
	(4) Vis highlights (Viser alle de steder, hvor der er fundet highlights)
	(5) Filtrer chat (Viser chatten filtreret)
	(6) Gå tilbage til hovedmenuen 

Sidste hovedmenu mulighed er 
3. Gå live

	Dette vil starte streamchatten i realtid, hvor emotestreaks, highlights og spørgsmåls også vises



Under både live indstillinger og offline indstillinger er der en emotemenu
I emotemenuen promptes man for:

(1) Slet alle nuværende emotes og tilføj nye
(2) Tilføj nye emotes uden at slette de forrige
(3) Gør emotes til standard emotes fra standard_emotes.txt
(4) Viser hvilke emotes er standard emotes
(5) Viser de nuværende valgte emotes
(6) Tilbage til undermenu

// Udarbejdet af gruppe A408b //