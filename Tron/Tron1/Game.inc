;
;          ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
;          º  ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ  º
;          º  Û ÜÜÜÜÜÜÜÜÜ   ÜÜÜÜÜÜÜ    ÜÜÜÜÜÜÜÜ   ÜÜ      ÜÜ Û  º
;          º  Û ßßßÛÛÛßßß  ÛÛßßßßßÛÛ  ÛÛßßßßßßÛÛ  ÛÛÛÜ    ÛÛ Û  º
;          º  Û    ÛÛÛ     ÛÛÜÜÜÜÜÛÛ  ÛÛ      ÛÛ  ÛÛ ßÛÜ  ÛÛ Û  º
;          º  Û    ÛÛÛ     ÛÛ  ßÛÛÜ   ÛÛ      ÛÛ  ÛÛ   ßÛÜÛÛ Û  º
;          º  Û    ÛÛÛ     ÛÛ    ßÛÛ  ßÛÛÛÛÛÛÛÛß  ÛÛ     ßÛÛ Û  º
;          º  ÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛ  º
;          º                                                    º
;          ÈÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍ¼
;                   º  FUCHS Steve & SCHNEIDER J‚rome  º
;                   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
;

;
;   PARTIE 3 : Jeu
;  ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
;

;Remise … niveau de la pile
mov ax,stack
mov ss,ax
mov sp,StackTop

;Passage en mode graphique 320*200 avec 256 couleurs
mov word ax,0013h
int 10h            ;On utilise la fonction 0 de l'interruption 10h

;On va d‚finir l'adresse du DATA Segment DS pour le changement de palette
mov word ax,dataJeu
mov word ds,ax

;Red‚finition de la palette de couleurs pour le jeu
mov cx,5*3         ;Pr‚paration des paramŠtres pour l'appel de la proc‚dure loadPalette
mov si,Couleurs
call loadPalette   ;Appel de la proc‚dure loadPalette


;***** Point de d‚part d'une nouvelle partie
@NEW_GAME:

;Remise … 0 des scores des deux joueurs
mov byte [ds:PointsJoueur1],0
mov byte [ds:PointsJoueur2],0


;***** Point de d‚part d'un nouveau combat
@CONTINUE_GAME:

;Initialisation de l'EXTRA Segment
push word 0A000h
pop word es

;On va effacer l'‚cran...
call EffaceScreen

;Initialisation de l'EXTRA Segment pour l'affichage du nom des deux joueurs
push word dataJeu
pop word es

;Affichage des noms des deux joueurs
push bp       ;Sauvegarde de l'‚tat du registre pointeur BP

mov word ax,1300h       ;Fonction 13h de l'interruption 10h
mov word bx,1           ;CaractŠres de couleur 1 (Rouge)
mov word cx,13          ;Nombre de caractŠres de la chaŒne … afficher
mov byte dh,24          ;Ligne o— l'on veut afficher la chaŒne
mov byte dl,LblJ1Col    ;Colonne de l'affichage de cette chaŒne
mov word bp,LblJoueur1  ;Adresse dans ES de la chaŒne … afficher
int 10h                 ;On fait appel … l'interruption

mov word bx,2           ;CaractŠres de couleur 2 (Bleu)
mov byte dl,LblJ2Col    ;Colonne o— l'on veut afficher la chaŒne
mov word bp,LblJoueur2  ;Adresse dans ES de la chaŒne … afficher
int 10h

pop bp        ;Restitution de l'‚tat initial de BP

;Affichage des points des deux joueurs
call AffichePoints      ;Appel de la proc‚dure AffichePoints

;Initialisation de l'EXTRA et du DATA Segment pour le corps du jeu
push word 0A000h
pop word es
push word dataJeu
pop word ds

;Dessin de la ligne sup‚rieure de l'aire de jeu
mov word si,320*LigHaut
DessinCadreHaut:
    mov byte [es:si],3
    inc word si
    cmp word si,320+320*LigHaut
    jl DessinCadreHaut

;Dessin des deux lignes verticales de l'aire de jeu
mov word si,320+320*LigHaut
DessinCadreLigVer:
    mov byte [es:si],3
    add word si,319
    mov byte [es:si],3
    inc word si
    cmp word si,319+320*LigBas
    jbe DessinCadreLigVer

;Dessin de la ligne inf‚rieure de l'aire de jeu
mov si,320*LigBas
DessinCadreBas:
    mov byte [es:si],3
    inc word si
    cmp word si,320+320*LigBas
    jl DessinCadreBas

;Placement des points de d‚part des deux joueurs
mov word si,DepartJ1
mov byte [es:si],1
mov word di,DepartJ2
mov byte [es:di],2

;Affichage d'un petit compte … rebourds
push word dataJeu
pop word es        ;ES = dataJeu momentan‚ment

mov word cx,CAR+1  ;Recherche de la constante contenant le d‚part du compte … rebourd

CARBcle:           ;Boucle principale du compte … rebourd
    mov byte dl,cl           ;On copie CL dans DL
    dec byte dl              ;On d‚cr‚ment DL
    add dl,48                ;Conversion du chiffre contenu dans DL en ASCII
    mov byte [ds:CARch],dl   ;On copie DL dans la zone temporaire CARch de la m‚moire

    push word cx             ;Sauvegarde des deux registres CX et BP
    push word bp

    mov word ax,1300h
    mov word bx,3
    mov word cx,2            ;Affichage du contenu de CARch grƒce … la fonction
    mov word dx,0B14h        ;   13h de l'interruption 10h
    mov word bp,CARch
    int 10h

    mov word cx,50           ;On a r‚p‚ter 50 fois la petite boucle d'attente
    CARBcle2:
         call Attente             ;Appel d'une petite boucle d'attente
         mov word ax,0100h
         int 16h                  ;On ‚x‚cute la fonction 1 de l'interruption 16h
         jz CARBcleAttenteFin     ;Si une touche … ‚t‚ appuy‚e, le flag ZF est mis … 0,
                                  ;  ainsi, si une touche est appuy‚e, on fait le saut
         xor ax,ax                ;On ‚x‚cute la fonction 0 de l'interruption 16h
         int 16h                  ;On r‚cupŠre le scancode de la touche appuy‚e dans AH

         cmp byte ah,1            ;On v‚rifie que ce n'est pas la touche <Echap>
         jne CARBcleAttenteFin

         jmp GameFin              ;Si c'est la touche <Echap>, on quitte le jeu

         CARBcleAttenteFin:
         loop CARBcle2            ;On retourne au niveau du label CARBcle2 en d‚cr‚mentant CX

    pop word bp              ;Restitution des deux registres CX et BP
    pop word cx

    dec word cx              ;On d‚cr‚mente CX
    cmp word cx,0            ;On compare CX … 0
    je GameBcleFin           ;Si c'est ‚gal, on quitte la boucle
    jmp CARBcle              ;   sinon, on retourne au d‚but de la boucle...

GameBcleFin:
push word bp
mov byte [ds:CARch],0
mov word ax,1300h
mov word bx,3
mov word cx,2                ;On va effacer le qui reste encore affich‚ … l'‚cran
mov word dx,0B14h
mov word bp,CARch
int 10h
pop word bp

;Initialisation de l'EXTRA Segment
push word 0A000h
pop word es

;Initialisation des d‚placements des deux joueurs
mov byte bh,Player1Left
mov byte bl,Player2Right

;D‚but de la boucle principale du jeu
GameBcle:
    call Attente        ;Appel d'une petite proc‚dure d'attente

    mov word ax,0100h
    int 16h             ;On ‚x‚cute la fonction 1 de l'interruption 16h
    jnz ToucheApp       ;Si une touche … ‚t‚ appuy‚e, le flag ZF est mis … 0,
                        ;ainsi, si une touche est appuy‚e, on fait le saut

    ;Si aucune touche n'est press‚e, on ‚x‚cutera les instructions ci-dessous
    xor ax,ax
    call Joueur1Joue
    xor ax,ax
    call Joueur2Joue
    jmp GameBcle        ;On retourne au d‚but de la boucle

    ;Si une touche a ‚t‚ press‚e, on ‚x‚cutera les instructions ci-dessous
    ToucheApp:
    xor ax,ax
    int 16h             ;On r‚cupŠre le scancode de la touche appuy‚e dans AH
                        ;   grƒce … la fonction 0 de l'intteruption 16h

    cmp byte ah,1       ;On v‚rifie que ce n'est pas la touche <Echap>
    jne GameBcleBond1   ;Si c'est la touche <Echap> qui a ‚t‚ press‚e,
    jmp GameFin         ;   on saute vers l'adresse de la proc‚dure "quitter le jeu"

    GameBcleBond1:
    push ax             ;On sauvegarde AX car ce registre risque d'ˆtre modifi‚
    call Joueur1Joue
    pop ax              ;On restitue AX
    call Joueur2Joue
    call Etoile         ;On affiche une ‚toile de mur
    jmp GameBcle        ;On retourne au d‚but de la boucle

;On va comparer un code de touche du clavier suppos‚e press‚ avec celui de chacune des touches de jeu du joueur
Joueur1Joue:
    cmp byte ah,Player1Up    ;On compare avec la touche Haut du joueur
    jne Joueur1JoueBond1
    cmp byte bh,Player1Down  ;On fait attention … ne pas faire revenir la moto du
    je Joueur1JoueBond4      ;   joueur sur elle-mˆme...
    call Player1AllerHaut    ;Appel de la proc‚dure AllerHaut du joueur
    jmp Joueur1JoueFin       ;On saute … la fin de la proc‚dure

    Joueur1JoueBond1:
    cmp byte ah,Player1Left  ;On compare avec la touche Gauche du joueur
    jne Joueur1JoueBond2
    cmp byte bh,Player1Right ;On fait attention … ne pas faire revenir la moto du
    je Joueur1JoueBond4      ;   joueur sur elle-mˆme...
    call Player1AllerGauche  ;Appel de la proc‚dure AllerGauche du joueur
    jmp Joueur1JoueFin       ;On saute … la fin de la proc‚dure

    Joueur1JoueBond2:
    cmp byte ah,Player1Right ;On compare avec la touche Droite du joueur
    jne Joueur1JoueBond3
    cmp byte bh,Player1Left  ;On fait attention … ne pas faire revenir la moto du
    je Joueur1JoueBond4      ;   joueur sur elle-mˆme...
    call Player1AllerDroite  ;Appel de la proc‚dure AllerDroite du joueur
    jmp Joueur1JoueFin       ;On saute … la fin de la proc‚dure

    Joueur1JoueBond3:
    cmp byte ah,Player1Down  ;On compare avec la touche Bas du joueur
    jne Joueur1JoueBond4
    cmp byte bh,Player1Up    ;On fait attention … ne pas faire revenir la moto du
    je Joueur1JoueBond4      ;   joueur sur elle-mˆme...
    call Player1AllerBas     ;Appel de la proc‚dure AllerBas du joueur
    jmp Joueur1JoueFin       ;On saute … la fin de la proc‚dure

    Joueur1JoueBond4:        ;On se trouve … ce niveau-l… si aucune touche press‚e ne correspondent aux touches de jeu du premier joueur
    mov byte ah,bh           ;On reprend la derniŠre direction du joueur 1
    jmp Joueur1Joue          ;On retourne en haut de la boucle

    Joueur1JoueFin:
    mov byte bh,ah           ;On sauvegarde la direction actuelle du joueur comme dernier direction...
    ret                      ;Retour au niveau de l'appellant

;On va comparer un code de touche du clavier suppos‚e press‚ avec celui de chacune des touches de jeu du joueur 2
Joueur2Joue:
    cmp byte ah,Player2Up    ;On compare avec la touche Haut du joueur
    jne Joueur2JoueBond1
    cmp byte bl,Player2Down  ;On fait attention … ne pas faire revenir la moto du
    je Joueur2JoueBond4      ;   joueur sur elle-mˆme...
    call Player2AllerHaut    ;Appel de la proc‚dure AllerHaut du joueur
    jmp Joueur2JoueFin       ;On saute … la fin de la proc‚dure

    Joueur2JoueBond1:
    cmp byte ah,Player2Left  ;On compare avec la touche Gauche du joueur
    jne Joueur2JoueBond2
    cmp byte bl,Player2Right ;On fait attention … ne pas faire revenir la moto du
    je Joueur2JoueBond4      ;   joueur sur elle-mˆme...
    call Player2AllerGauche  ;Appel de la proc‚dure AllerGauche du joueur
    jmp Joueur2JoueFin       ;On saute … la fin de la proc‚dure

    Joueur2JoueBond2:
    cmp byte ah,Player2Right ;On compare avec la touche Droite du joueur
    jne Joueur2JoueBond3
    cmp byte bl,Player2Left  ;On fait attention … ne pas faire revenir la moto du
    je Joueur2JoueBond4      ;   joueur sur elle-mˆme...
    call Player2AllerDroite  ;Appel de la proc‚dure AllerDroite du joueur
    jmp Joueur2JoueFin       ;On saute … la fin de la proc‚dure

    Joueur2JoueBond3:
    cmp byte ah,Player2Down  ;On compare avec la touche Bas du joueur
    jne Joueur2JoueBond4
    cmp byte bl,Player2Up    ;On fait attention … ne pas faire revenir la moto du
    je Joueur2JoueBond4      ;   joueur sur elle-mˆme...
    call Player2AllerBas     ;Appel de la proc‚dure AllerBas du joueur
    jmp Joueur2JoueFin       ;On saute … la fin de la proc‚dure

    Joueur2JoueBond4:        ;On se trouve … ce niveau-l… si aucune touche press‚e ne correspondent aux touches de jeu du second joueur
    mov byte ah,bl           ;On reprend la derniŠre direction du joueur 2
    jmp Joueur2Joue          ;On retourne en haut de la boucle

    Joueur2JoueFin:
    mov byte bl,ah           ;On sauvegarde la direction actuelle du joueur comme dernier direction...
    ret                      ;Retour au niveau de l'appellant


;Proc‚dure concernant les actions du premier joueur, ainsi que la proc‚dure ‚x‚cut‚e en cas de collision de ce joueur
;   SI correspond … la position du joueur 1
Player1AllerHaut:
    sub si,320
    call Player1CheckCol
    ret
Player1AllerGauche:
    sub si,1
    call Player1CheckCol
    ret
Player1AllerDroite:
    add si,1
    call Player1CheckCol
    ret
Player1AllerBas:
    add si,320
    call Player1CheckCol
    ret
Player1CheckCol:
    cmp byte [es:si],0       ;On v‚rifie une ‚ventuelle collision (couleur de la future position du joueur 1 <> 0)
    jne Player1Collision
    mov byte [es:si],1       ;Si aucune collision n'est d‚tect‚e, on affiche le nouveau point du joueur
    ret                      ;Retour au niveau de l'appelant
    Player1Collision:
         add sp,6                      ;On rectifie le sommet de la pile (ret non satisfait)
         inc byte [ds:PointsJoueur2]   ;On incr‚mente les points du premier joueur
         cmp word di,si                ;On v‚rifie le cas d'un ‚ventuel match nul entre les deux joueurs
         jne Player1CollisionPart2
         inc byte [ds:PointsJoueur1]   ;Si Match nul il y a, augmenter les points du joueur adverse tu devras...
         Player1CollisionPart2:
         jmp GameContinue

;Proc‚dure concernant les actions du second joueur, ainsi que la proc‚dure ‚x‚cut‚e en cas de collision de ce joueur
;   DI correspond … la position du joueur 2
Player2AllerHaut:
    sub di,320
    call Player2CheckCol
    ret
Player2AllerGauche:
    sub di,1
    call Player2CheckCol
    ret
Player2AllerDroite:
    add di,1
    call Player2CheckCol
    ret
Player2AllerBas:
    add di,320
    call Player2CheckCol
    ret
Player2CheckCol:
    cmp byte [es:di],0       ;On v‚rifie une ‚ventuelle collision (couleur de la future position du joueur 2 <> 0)
    jne Player2Collision
    mov byte [es:di],2       ;Si aucune collision n'est d‚tect‚e, on affiche le nouveau point du joueur
    ret                      ;Retour au niveau de l'appelant
    Player2Collision:
         add sp,6                      ;On rectifie le sommet de la pile (ret non satisfait)
         inc byte [ds:PointsJoueur1]   ;On incr‚mente les points du second joueur
         cmp word di,si                ;On v‚rifie le cas d'un ‚ventuel match nul entre les deux joueurs
         jne Player2CollisionPart2
         inc byte [ds:PointsJoueur2]   ;Si Match nul il y a, augmenter les points du joueur adverse tu devras...
         Player2CollisionPart2:
         jmp GameContinue

;***** Fin du combat
GameContinue:

xor ax,ax     ;On attend que l'un des deux joueurs appuie une quelconque touche pour continuer de joueur
int 16h       ;   On utilise pour cela, la fonction 0 de l'interruption 16h

pushad        ;Sauvegarde de l'‚tat de l'ensemble des registres

push word 0A000h
pop word es        ;ES = A000

call EffaceScreen  ;Appel de la proc‚dure EffaceScreen

push word dataJeu
pop word es        ;ES = dataJeu

;On v‚rifie si un joueur … gagn‚ ou non
cmp byte [ds:PointsJoueur1],Partie+1
je Joueur1Gagne
cmp byte [ds:PointsJoueur2],Partie+1
je Joueur2Gagne

;Si aucun joueur n'a gagn‚, faire ce qui suit
popad                   ;Restitution de l'‚tat initial de l'ensemble des registres
jmp @CONTINUE_GAME      ;On fait un saut jusqu'au label @CONTINUE_GAME qui repr‚sente le point de d‚part d'un nouveau jeu

;Dans le cas d'un match nul...
MatchNul:
mov word cx,TailleMNW
mov word bp,MatchNulWin
mov word bx,4                ;Affectation des registres sp‚cifique au cas du Match Nul
xor byte dl,dl
jmp AfficherGagnant          ;Saut vers AfficherGagnant

;Dans le cas du joueur 1 gagnant...
Joueur1Gagne:
cmp byte [ds:PointsJoueur2],Partie+1
je MatchNul                            ;On v‚rifie le cas d'un ‚ventuel match nul
mov word cx,TailleJWin
mov word bp,JoueurWin   ;On veut afficher la chaŒne "est vainqueur..." … l'‚cran
mov word bx,1
mov word ax,1300h       ;On veut ‚x‚cuter la fonction 13h de l'interruption 10h
mov byte dh,12          ;Ligne o— l'on veut afficher la chaŒne
mov byte dl,0           ;Colonne o— l'on veut afficher la chaŒne
int 10h                 ;Appel de l'interruption 10h

mov word bp,LblJoueur1  ;On veut ensuite afficher le nom du joueur 1 avant cette chaŒne...
mov byte dl,15          ;Colonne o— l'on veut afficher la chaŒne
mov word cx,10          ;Longueur de la chaŒne
jmp AfficherGagnant     ;Saut vers AfficherGagnant

;Dans le cas du joueur 2 gagnant
Joueur2Gagne:
mov word cx,TailleJWin
mov word bp,JoueurWin   ;On veut afficher la chaŒne "est vainqueur..." … l'‚cran
mov word bx,2
mov word ax,1300h       ;On veut ‚x‚cuter la fonction 13h de l'interruption 10h
mov byte dh,12          ;Ligne o— l'on veut afficher la chaŒne
mov byte dl,0           ;Colonne contenu dans une constante
int 10h                 ;Appel de l'interruption 10h

mov word bp,LblJoueur2  ;On veut ensuite afficher le nom du joueur 1 avant cette chaŒne...
mov byte dl,15          ;Colonne o— l'on veut afficher la chaŒne
mov word cx,10          ;Longueur de la chaŒne

AfficherGagnant:
mov word ax,1300h       ;On veut ‚x‚cuter la fonction 13h de l'interruption 10h
mov byte dh,11          ;Ligne o— l'on veut afficher la chaŒne
int 10h                 ;On fait appel … l'interruption 10h

popad         ;Restitution de l'‚tat initial de l'ensemble des registres

xor ax,ax
int 16h       ;Attente de l'appui d'une touche...

pushad        ;Sauvegarde de l'‚tat de l'ensemble des registres

push word 0A000h
pop word es        ;ES = A000

call EffaceScreen  ;Appel de la proc‚dure EffaceScreen

push word dataJeu
pop word es        ;ES = dataJeu

;Affichage de la demande "Nouvelle partie ? (O)ui / (N)on"...
mov word ax,1300h       ;On veut ‚x‚cuter la fonction 13h de l'interruption 10h
mov word bx,3           ;Attribut des caractŠres de la chaŒne … afficher ‚gal … 1
mov word cx,TailleNPQ   ;Nombre de caractŠres de la chaŒne … afficher
mov byte dh,11          ;Ligne o— l'on veut afficher la chaŒne
mov byte dl,0           ;Colonne contenu dans une constante
mov word bp,NewPartieQ  ;Adresse dans ES de la chaŒne … afficher
int 10h                 ;On fait appel … l'interruption 10h

popad    ;Restitution des paramŠtres

DemandeNewPartie:
xor ax,ax
int 16h       ;Attente de l'appui d'une touche

cmp ah,24               ;24 est le scancode de la touche <O>
jne ContDemandeNP
jmp @NEW_GAME

ContDemandeNP:
cmp ah,49               ;49 est le scancode de la touche <N>
jne DemandeNewPartie

jmp QuitterJeuTotal     ;Si l'utilisateur a choisit de ne pas continuer, on quitte le jeu...

;***** Fin du jeu
GameFin:
push word es
push word 0A000h
pop es
call EffaceScreen            ;On va effacer l'‚cran
pop es

push word bp                 ;Sauvegarde de BP et ES
push word es
push word dataJeu            ;ES = dataJeu
pop es

;Affichage de la demande "Quitter r‚ellement ce magnifique et subliminal jeu ?"...
mov word ax,1300h            ;On veut ‚x‚cuter la fonction 13h de l'interruption 10h
mov word bx,3                ;Attribut des caractŠres de la chaŒne … afficher ‚gal … 1
mov word cx,TailleQuitter    ;Nombre de caractŠres de la chaŒne … afficher
mov byte dh,10               ;Ligne o— l'on veut afficher la chaŒne
mov byte dl,2                ;Colonne contenu dans une constante
mov word bp,Quitter          ;Adresse dans ES de la chaŒne … afficher
int 10h                      ;On fait appel … l'interruption 10h

pop word es        ;Restitution de ES et BP
pop word bp

AttenteReponseQuitter:
xor ax,ax     ;On attend l'appui d'une touche
int 16h

cmp ah,24               ;24 est le scancode de la touche <O>
jne AttRepQuitterOORange
jmp QuitterJeuTotal

AttRepQuitterOORange:
cmp ah,49               ;49 est le scancode de la touche <N>
jne AttenteReponseQuitter

jmp @CONTINUE_GAME      ;Si l'utilisateur a choisit de ne pas quitter, on revient au jeu, mais juste en recommencant le combat en cours


;********** Proc‚dures diverses

; Proc‚dure de chargement d'une palette :
;-----------------------------------------
;   ParamŠtres :   ds:si     Tableau source
;                  cx        Nombre de couleurs * 3
loadPalette:
    pusha          ;Sauvegarde de l'‚tat avant la proc‚dure
    xor ax,ax      ;Met AX … 0000
    mov dx,3c8h    ;Met DX … 03C8
    out dx,al      ;Met 0 sur le port 3C8h
    mov dx,3c9h    ;Met DX … 03C9
    cld            ;Met le flad DF … 0
    rep outsb      ;Met les valeurs du tableau DS:SI sur le port 3C9h
    popa           ;Restitution de l'‚tat avant la proc‚dure
    ret            ;Retour au niveau de l'appelant

; Proc‚dure d'effacement de l'‚cran :
;-------------------------------------
EffaceScreen:
    pushad              ;Sauvegarde de l'‚tat avant la proc‚dure
    xor ax,ax           ;Met AX … 0000
    xor di,di           ;On commence par le premier pixel (celui de l'adresse ES:0000)
    mov word cx,64000   ;L'‚cran compte 64000 pixels en tout... et on veut tous les colorier
    cld                 ;Met le flag DF … 0 (Ainsi, stosb incr‚mentera di … chaque fois)
    rep stosb           ;On va r‚p‚ter 64000 fois stosb...
    popad               ;Restitution de l'‚tat avant la proc‚dure
    ret                 ;Retour au niveau de l'appelant

; Proc‚dure AffichePoints :
;---------------------------
AffichePoints:
    pusha                    ;Sauvegarde de l'‚tat

    xor byte ah,ah
    mov byte al,[ds:PointsJoueur1]     ;R‚cup‚ration dans AX des points du joueur 1
    xor dx,dx                ;Remise … 0 du registre DX
    mov byte dl,0Ah
    div byte dl              ;Division par 10 de AL
    add word ax,3030h        ;Conversion en ASCII des chiffres contenu dans AH et AL
    mov word [ds:LblPointsJ1],ax       ;Stocke en m‚moire ce nombre de point convertit

    xor ax,ax
    mov byte al,[ds:PointsJoueur2]
    xor dx,dx
    mov byte dl,0Ah          ;Mˆme principe, mais pour le joueur 2
    div byte dl
    add word ax,3030h
    mov word [ds:LblPointsJ2],ax

    ;Affichage des deux chaŒnes contenant les points des deux joueurs … l'‚cran au bon emplacement
    mov word ax,1300h
    mov word bx,1
    mov word cx,2
    mov byte dh,24
    mov byte dl,LblJ1Col+13
    mov word bp,LblPointsJ1
    int 10h

    mov byte bl,2
    mov byte dl,LblJ2Col+13
    mov word bp,LblPointsJ2
    int 10h

    popa           ;Restitution de l'‚tat
    ret

; Proc‚dure d'attente :           (Proc‚dure d'attente de retour du faisceau de l'‚cran)
;-----------------------
Attente:
    pusha               ;Sauvegarde de l'‚tat
    mov word dx,3DAh    ;AccŠde au port Input Status Register 1
    @inactif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jnz @inactif        ;Si AL<>0, alors le faisceau est ‚teint, on attend qu'il
                             ;rafraŒchisse l'‚cran pour ˆtre s–r que lorsqu'il sera inactif,
                             ;il sera ‚teint pendant environ 1/70e de seconde
    @actif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jz @actif           ;Si AL=0, alors le faisceau rafraŒchit l'‚cran, on attend qu'il a fini
    popa                ;Restitution de l'‚tat initial
    ret                 ;Retour … l'instruction appelant la proc‚dure

; Proc‚dure qui fait apparaŒtre les bonus points en plein jeu :
;---------------------------------------------------------------
Etoile:
    pusha                    ;Sauvegarde de l'‚tat des registres g‚n‚raux
    xor ax,ax
    in al,40h
    xchg al,ah
    in al,40h                ;On aura … ce moment-l…, AX al‚atoire
    mov word di,ax           ;On va mettre le contenu de AX dans un registre d'index
    cmp byte [es:di],0       ;Je veux que les ‚toiles ne peuvent apparaŒtre que dans des zones vides de tout ‚l‚ment
    jne EtoileFin
    cmp word di,LigBas*320   ;Je ne veux rien inscrire dans la zone d'affichage des scores
    ja EtoileFin
    mov byte [es:di],3       ;Affichage d'un bonus … un endroit al‚atoire
    EtoileFin:
    popa                     ;Restitution de l'‚tat initial
    ret                      ;Retour au niveau de l'appellant


;Quitter le jeu en beaut‚...
QuitterJeuTotal:

;Fin du module GAME.INC