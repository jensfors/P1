README-fil til det medfølgende c-program til automatisk highlight og chatfiltrering i af en Twitch Chat

// Ændringer i c-programmet //
I cprogrammet er der mulighed for at ændre følgende værdier:
	MIN_SIMIL: Bestemmer hvor striks chat filtreringen skal være (Double værdi fra 0 - 1)
	HIGHLIGHT_THRESHOLD: Er multiplikatoren for hvornår der vurderes der er highlight (Heltal)
	STARTHOUR: Start timen til en chatlog
	STARMIN: Start minuttet til en chatlog
	STARTSEC: Start sekundet til en chatlog


// Oversættelse af programmet //
Programmet oversættes med 'gcc chatbot.c' ved at bruge en GNU compiler 


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

	Under denne indstilling vil du først blive promptet for hvilken chatfil der skal anvendes
	(1) Nuværende chatfil (Bruger chatfilen fra den sidste livechat)
	(2) Vælg en anden fil (Vælg selv en chatfil)

	Efter chatfil valg promptes en ny undermenu
	(1) Emotemenu (Muligheden for at vælge emotes til emote streaks og highlights)
	(2) Søg chat (Søg efter brugere eller chatbeskeder fra chatten)
	(3) Vis spørgsmål (Viser alle spørgsmål stillet i den chatfil)
	(4) Vis highlights (Viser alle de steder, hvor der er fundet highlights)
	(5) Filtrer chat (Viser hele chatten filtreret)
	(6) Gå tilbage til hovedmenuen 

Sidste hovedmenu mulighed er 
3. Gå live

	Dette vil starte streamchatten i realtid, hvor emotestreaks, highlights og spørgsmåls også vises


Under både live indstillinger og offline indstillinger er der en emotemenu
Funktioner i emotemenuen:

	(1) Slet alle nuværende emotes og tilføj nye
	(2) Tilføj nye emotes uden at slette de forrige
	(3) Gør emotes til standard emotes (Kan findes i standard_emotes.txt)
	(4) Viser hvilke emotes er standard emotes
	(5) Viser de nuværende valgte emotes
	(6) Gå tilbage til undermenu

// Udarbejdet af gruppe A408b december 2017 //