;
;          ����������������������������������������������������ͻ
;          �  ������������������������������������������������  �
;          �  � ���������   �������    ��������   ��      �� �  �
;          �  � ���������  ���������  ����������  ����    �� �  �
;          �  �    ���     ���������  ��      ��  �� ���  �� �  �
;          �  �    ���     ��  ����   ��      ��  ��   ����� �  �
;          �  �    ���     ��    ���  ����������  ��     ��� �  �
;          �  ������������������������������������������������  �
;          �                                                    �
;          ����������������������������������������������������ͼ
;                   �  FUCHS Steve & SCHNEIDER J�rome  �
;                   ����������������������������������͹
;                   �            Version 2             �
;                   ����������������������������������ͼ
;


; ***** Segment principal de donn�es, qui contiendra toutes les constantes et toutes les variables

SEGMENT data
    ;Touches de jeu du premier joueur
    Player1Up      equ  72   ;Fl�che Haut
    Player1Left    equ  75   ;Fl�che Gauche
    Player1Right   equ  77   ;Fl�che Droite
    Player1Down    equ  80   ;Fl�che Bas

    ;Touches de jeu du second joueur
    Player2Up      equ  17   ;Touche <Z>
    Player2Left    equ  30   ;Touche <Q>
    Player2Right   equ  32   ;Touche <D>
    Player2Down    equ  31   ;Touche <S>

    ;Param�tres de dessin du cadre de l'aire de jeu
    LigHaut        equ  0    ;Num�ro de la ligne de pixels sur laquelle on aura la barre sup�rieure du cadre
    LigBas         equ  189  ;Num�ro de la ligne de pixels sur laquelle on aura la barre inf�rieure du cadre

    ;Param�tres concernant l'affichage des labels des joueurs
    LblJ1Col       equ  20
    LblJ2Col       equ  0
    LblJoueur1     db   '  Joueur 1 : '
    LblJoueur2     db   '  Joueur 2 : '

    ;Variables utilis�es pour l'affichage des scores... ainsi que les autres trucs en rapport avec les scores
    LblPointsJ1    db   '00' ;Une cha�ne de caract�res transformable...
    LblPointsJ2    db   '00' ;Idem
    PointsJoueur1  db   0    ;Le nombre de points du premier joueur
    PointsJoueur2  db   0    ;Le nombre de points du second joueur
    Partie         equ  2    ;Le nombre de points � avoir pour gagner le jeu

    ;Variables utilis�es pour l'affichage d'un petit compte � rebourd au d�but de chaque combat...
    CAR            equ  3    ;Chiffre � partir duquel l'on veut commencer le compte � rebourd
    CARch          resb 1    ;Un simple octet qui repr�sentera le chiffre courant du compte � rebourd sous forme ASCII

    ;Param�tres de d�part des deux joueurs
    DepartJ1       equ  30340     ;Position de d�part du premier joueur dans le segment de la m�moire graphique
    DepartJ2       equ  30140     ;Position de d�part du second joueur dans le segment de la m�moire graphique

    ;Tableau permettant la red�finition de la palette de couleurs
    Couleurs:      db   0,0,0     ; (0) Noir - Couleur du fond
                   db   63,0,0    ; (1) Rouge - Couleur du joueur 1
                   db   0,0,63    ; (2) Bleu - Couleur du joueur 2
                   db   63,63,0   ; (3) Jaune - Couleur du cadre de l'espace de jeu
                   db   63,63,63  ; (4) Blanc - Couleur des bonus � l'�cran

    ;D�finition des constantes et des variables n�c�ssaires � la gestion de la trace constante des deux joueurs
    TailleMax      equ  199  ;Taille maximale qu'aucun joueur ne pourra jamais d�passer
    TailleMaxJ1    resw 1    ;Taille maximale courante du premier joueur
    TailleMaxJ2    resw 1    ;Taille maximale courante du second joueur
    CooJ1          resw 200  ;Tableau contenant les coordonn�es des TailleMaxJ1 premiers points du premier joueur
    CooJ2          resw 200  ;Tableau contenant les coordonn�es des TailleMaxJ1 premiers points du second joueur
    TailleJ1       dw   0    ;Taille courante du premier joueur
    TailleJ2       dw   0    ;Taille courante du second joueur

    ;D�finitions des variables repr�sentant le texte qui sera affich� tout au long de l'�x�cution du programme
    Intro:         db   '������   �������    �������   ��      ��'
                   db   '������  ���������  ���������  ����    ��'
                   db   '  ��    ���������  ��     ��  �� ���  ��'
                   db   '  ��    ��  ����   ��     ��  ��   �����'
                   db   '  ��    ��    ���  ���������  ��     ���'
                   db   '                                        '
                   db   '              Version 2                 '
                   db   '                                        '
                   db   '                                        '
                   db   '      by                                '
                   db   '                                        '
                   db   '                FUCHS Steve             '
                   db   '                                        '
                   db   '                     and                '
                   db   '                                        '
                   db   '                    SCHNEIDER J�rome    '
    TailleIntro    equ  40*16
    Quitter        db   'Quitter R�ellement le jeu? (O) / (N)'
    TailleQuitter  equ  36
    Joueur1Win     db   '      Le joueur 1 est vainqueur...      '
    TailleJ1Win    equ  40
    Joueur2Win     db   '      Le joueur 2 est vainqueur...      '
    TailleJ2Win    equ  40
    NewPartieQ     db   '       Nouvelle partie? (O) / (N)       '
    TailleNPQ      equ  40
    MatchNulWin    db   '   Match nul entre les deux joueurs !   '
    TailleMNW      equ  40

; ***** Segment de pile

SEGMENT stack stack
    resw 100h
StackTop:


; ***** Segment de code, qui contiendra tout le code du programme...

SEGMENT code

..start  ;Le programme d�marre ici

; ***** Initialisation du jeu

;Initialisation de la pile
mov ax,stack
mov ss,ax
mov sp,StackTop

;Passage en mode graphique 320*200 avec 256 couleurs
mov word ax,0013h
int 10h

;Initialisation du DATA Segment, ainsi que de l'EXTRA Segment
mov word ax,data
mov word es,ax          ;On a ainsi : DS = ES = SEGMENT data
mov word ds,ax

;Red�finition de la nouvelle palette de couleurs
mov cx,5*3         ;Pr�paration des param�tres � passer � la proc�dure loadPalette
mov si,Couleurs
call loadPalette   ;Appel de la proc�dure loadPalette de red�finition des palettes

; ***** Introduction

mov word ax,1300h       ;On veut �x�cuter la fonction 13h de l'interruption 10h
mov word bx,3           ;Attribut des caract�res de la cha�ne � afficher �gal � 1
mov word cx,TailleIntro ;Nombre de caract�res de la cha�ne � afficher
mov byte dh,4           ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,0           ;Colonne contenu dans une constante
mov word bp,Intro       ;Adresse dans ES de la cha�ne � afficher
int 10h                 ;On fait appel � l'interruption 10h

xor ax,ax
int 16h

; ***** D�but d'une nouvelle partie

@NEW_GAME:

;Remise � 0 des scores des deux joueurs
mov byte [ds:PointsJoueur1],0
mov byte [ds:PointsJoueur2],0


; ***** D�but d'un combat

@CONTINUE_GAME:

;D�finition d'une taille minimale des deux joueurs...
mov word [ds:TailleMaxJ1],39
mov word [ds:TailleMaxJ2],39

;Remise � z�ro de la pile (en cas d'�ventuel ret qui n'ont pas pu avoir satisfaction de leur t�che)
mov sp,StackTop

;On va effacer l'�cran
call EffaceScreen

;Affichage des noms des joueurs
mov ax,data
mov es,ax          ;On a ainsi DS = ES = SEGMENT data

push bp            ;Sauvegarde de la valeur de BP

mov word ax,1300h       ;On veut �x�cuter la fonction 13h de l'interruption 10h
mov word bx,1           ;Attribut des caract�res de la cha�ne � afficher �gal � 1
mov word cx,13          ;Nombre de caract�res de la cha�ne � afficher
mov byte dh,24          ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,LblJ1Col    ;Colonne contenu dans une constante
mov word bp,LblJoueur1  ;Adresse dans ES de la cha�ne � afficher
int 10h                 ;On fait appel � l'interruption 10h

mov byte bl,2           ;Attribut des caract�res de la cha�ne � afficher �gal � 2
mov word cx,13          ;Nombre de caract�res de la cha�ne � afficher
mov byte dh,24          ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,LblJ2Col    ;Colonne contenu dans une constante
mov word bp,LblJoueur2  ;Adresse dans ES de la cha�ne � afficher
int 10h                 ;On fait appele

pop bp             ;Restitution de la valeur de BP

;Affichage des points courants des deux joueurs
call AffichePoints      ;Appel de la proc�dure d'affichage des points des deux joueurs

;Dessin de l'aire de jeu
push word 0A000h
pop word es

;Dessin de la ligne sup�rieure de l'aireb de jeu
mov word si,320*LigHaut
DessinCadreHaut:
    mov byte [es:si],3
    inc si
    cmp word si,320+320*LigHaut
    jl DessinCadreHaut

;Dessin des deux lignes verticales de l'aire de jeu
mov word si,320+320*LigHaut
DessinCadreLigVer:
    mov byte [es:si],3
    add si,319
    mov byte [es:si],3
    inc si
    cmp si,319+320*LigBas
    jbe DessinCadreLigVer

;Dessin de la ligne inf�rieure de l'aire de jeu
mov si,320*LigBas
DessinCadreBas:
    mov byte [es:si],3
    inc si
    cmp si,320+320*LigBas
    jl DessinCadreBas

;Affichage des positions de d�part des deux joueurs en fonction des constantes en rapport
mov word si,DepartJ1
mov byte [es:si],1      ;Affichage du point de d�part du premier joueur
mov word di,DepartJ2
mov byte [es:di],2      ;Affichage du point de d�part du second joueur

;Affichage du compte � rebourd
mov ax,data
mov es,ax          ;ES = SEGMENT data

mov word cx,CAR+1            ;On affecte � CX la constante CAR, incr�ment�e de 1
CARBcle:
    mov byte dl,cl           ;On r�cup�re l'�tat du compteur du compte � rebourd dans DL
    dec byte dl              ;On d�cr�mente DL
    add dl,48                ;On le convertit sous forme de caract�re ASCII
    mov byte [ds:CARch],dl   ;On affecte ce nouveau caract�re � la variable CARch

    pushad                   ;On sauvegarde tout les registres
    mov word ax,1300h
    mov word bx,3
    mov word cx,2
    mov word dx,0B14h        ;On va �crire CARch au milieu de l'aire de jeu
    mov word bp,CARch
    int 10h

    mov cx,50
    CARBcleAttendre:
         call Attente             ;Petite boucle d'attente
         mov word ax,0100h
         int 16h                  ;On �x�cute la fonction 1 de l'interruption 16h
         jz CARBcleAttenteFin     ;Si une touche � �t� appuy�e, le flag ZF est mis � 0,
                                  ;  ainsi, si une touche est appuy�e, on fait le saut

         xor ax,ax                ;On �x�cute la fonction 0 de l'interruption 16h
         int 16h                  ;On r�cup�re le scancode de la touche appuy�e dans AH

         cmp byte ah,1            ;On v�rifie que ce n'est pas la touche <Echap>
         jne CARBcleAttenteFin
         jmp GameFin              ;Si c'est la touche <Echap>, on quitte le jeu

         CARBcleAttenteFin:
         loop CARBcleAttendre
    popad

    dec cx              ;On passe au chiffre suivant du compteur du compte � rebourd
    cmp word cx,0
    jne CARBcle

pushad
mov byte [ds:CARch],0
mov word ax,1300h
mov word bx,3
mov word cx,2
mov word dx,0B14h       ;On va effacer les r�sidus du texte du compte � rebourd affich� � l'�cran
mov word bp,CARch
int 10h
popad

;On va red�finir l'EXTRA Segment
push word 0A000h
pop word es

;On va d�finir les touches de d�part des deux joueurs
mov byte bh,Player1Left
mov byte bl,Player2Right

;On va remplir les param�tres de gestion de la file de chacun des deux joueurs afin d'obtenir la longueur de trace constante des deux joueurs
mov word ax,DepartJ1
mov word [ds:CooJ1],ax       ;On met le point de d�part de chacun des deux joueurs dans leurs files respectives
mov word ax,DepartJ2
mov word [ds:CooJ2],ax
mov word [ds:TailleJ1],1     ;On initialise la taille des deux files � 1
mov word [ds:TailleJ2],1

;D�but de la boucle principale de jeu
GameBcle:
    call Attente        ;Appel d'une petite proc�dure d'attente

    mov word ax,0100h
    int 16h             ;On �x�cute la fonction 1 de l'interruption 16h
    jnz ToucheApp       ;Si une touche � �t� appuy�e, le flag ZF est mis � 0,
                        ;  ainsi, si une touche est appuy�e, on fait le saut

    ;Si aucune touche n'est press�, on consid�re qu'une touche bidon a �t� enfonc� (la touche de scancode 0)
    xor ax,ax
    call Joueur1Joue
    xor ax,ax
    call Joueur2Joue
    jmp GameBcle

    ToucheApp:     ;On se trouve ici si une touche a �t� appuy�e
    xor ax,ax      ;On �x�cute la fonction 0 de l'interruption 16h
    int 16h        ;On r�cup�re le scancode de la touche appuy�e dans AH

    cmp byte ah,1       ;On v�rifie que ce n'est pas la touche <Echap>
    jne GameBcleBond1
    jmp GameFin         ;Si c'est la touche <Echap>, on quitte le jeu

    GameBcleBond1:
    push ax             ;On sauvegarde AX car ce registre risque d'�tre modifi�
    call Joueur1Joue
    pop ax              ;On restitue AX
    call Joueur2Joue
    call Etoile         ;On affiche une �toile de bonus al�atoirement sur l'�cran
    jmp GameBcle        ;On retourne au d�but de la grande boucle

;S�lection au cas par cas des actions du joueur 1, et on v�rifiera le cas d'un retour en arri�re de la moto...
Joueur1Joue:  ;Toutes les comparaisons se font en fonction de AH qui, � ce moment, peut contenir le scancode de la touche appuy� par le joueur
    cmp byte ah,Player1Up    ;On compare avec la touche HAUT du joueur
    jne Joueur1JoueBond1
    cmp byte bh,Player1Down  ;On v�rifie que la derni�re touche appuy�e n'est pas la touche bas (je ne veux pas pouvoir retourner en arri�re)
    je Joueur1JoueBond4
    call Player1AllerHaut    ;Appel de la proc�dure qui fait monter le joueur en haut
    jmp Joueur1JoueFin       ;On se dirige vers la fin de la proc�dure

    Joueur1JoueBond1:
    cmp byte ah,Player1Left  ;On compare avec la touche GAUCHE du joueur
    jne Joueur1JoueBond2
    cmp byte bh,Player1Right ;On fait en sorte que le joueur ne puisse pas revenir sur lui-m�me
    je Joueur1JoueBond4
    call Player1AllerGauche  ;On appel la proc�dure qui fait aller le joueur � gauche
    jmp Joueur1JoueFin

    Joueur1JoueBond2:
    cmp byte ah,Player1Right
    jne Joueur1JoueBond3
    cmp byte bh,Player1Left  ;M�me principe sauf pour aller � droite
    je Joueur1JoueBond4
    call Player1AllerDroite
    jmp Joueur1JoueFin

    Joueur1JoueBond3:
    cmp byte ah,Player1Down
    jne Joueur1JoueBond4
    cmp byte bh,Player1Up    ;M�me principe sauf pour aller en bas
    je Joueur1JoueBond4
    call Player1AllerBas
    jmp Joueur1JoueFin

    Joueur1JoueBond4:
    mov byte ah,bh           ;Si aucune touche appuy�e (m�me s'il n'y en a aucune) ne correspondent � celle du
    jmp Joueur1Joue          ;   joueur 1, on reprend la derni�re direction de ce joueur (contenu dans BH)

    Joueur1JoueFin:
    mov byte bh,ah           ;On sauvegarde la direction courante du joueur dans BH
    call PermutFileJoueur1   ;On fait appel � la proc�dure qui permet de g�rer la trace constante
    ret                      ;On retourne au niveau de l'instruction appelante

;Voici venu le tour du joueur 2...
Joueur2Joue:  ;Toutes les comparaisons se font en fonction de AH qui, � ce moment, peut contenir le scancode de la touche appuy� par le joueur
    cmp byte ah,Player2Up    ;On compare avec la touche HAUT du joueur
    jne Joueur2JoueBond1
    cmp byte bl,Player2Down  ;On v�rifie que la derni�re touche appuy�e n'est pas la touche bas (je ne veux pas pouvoir retourner en arri�re)
    je Joueur2JoueBond4
    call Player2AllerHaut    ;Appel de la proc�dure qui fait monter le joueur en haut
    jmp Joueur2JoueFin       ;On se dirige vers la fin de la proc�dure

    Joueur2JoueBond1:
    cmp byte ah,Player2Left  ;On compare avec la touche GAUCHE du joueur
    jne Joueur2JoueBond2
    cmp byte bl,Player2Right ;On fait en sorte que le joueur ne puisse pas revenir sur lui-m�me
    je Joueur2JoueBond4
    call Player2AllerGauche  ;On appel la proc�dure qui fait aller le joueur � gauche
    jmp Joueur2JoueFin

    Joueur2JoueBond2:
    cmp byte ah,Player2Right
    jne Joueur2JoueBond3
    cmp byte bl,Player2Left  ;M�me principe sauf pour aller � droite
    je Joueur2JoueBond4
    call Player2AllerDroite
    jmp Joueur2JoueFin

    Joueur2JoueBond3:
    cmp byte ah,Player2Down
    jne Joueur2JoueBond4
    cmp byte bl,Player2Up    ;M�me principe sauf pour aller en bas
    je Joueur2JoueBond4
    call Player2AllerBas
    jmp Joueur2JoueFin

    Joueur2JoueBond4:
    mov byte ah,bl           ;Si aucune touche appuy�e (m�me s'il n'y en a aucune) ne correspondent � celle du
    jmp Joueur2Joue          ;   joueur 2, on reprend la derni�re direction de ce joueur (contenu dans BL)

    Joueur2JoueFin:
    mov byte bl,ah           ;On sauvegarde la direction courante du joueur dans BL
    call PermutFileJoueur2   ;On fait appel � la proc�dure qui permet de g�rer la trace constante
    ret                      ;On retourne au niveau de l'instruction appelante


;Liste des proc�dures en rapport avec les d�placements du joueur 1
Player1AllerHaut:
    sub si,320
    call Player1MoveCheck
    ret
Player1AllerGauche:
    sub si,1
    call Player1MoveCheck
    ret
Player1AllerDroite:
    add si,1
    call Player1MoveCheck
    ret
Player1AllerBas:
    add si,320
    call Player1MoveCheck
    ret

;Proc�dure en rapport avec la recherche de collision et de rencontre avec un bonus dans le jeu
Player1MoveCheck:
    cmp byte [es:si],4       ;On compare la future position de la t�te de la moto du joueur 1
    jne Player1MoveCheckFin  ;   avec la couleur des points bonus
    cmp word [ds:TailleMaxJ1],TailleMax     ;On fait attention � ce que la moto du joueur ne puisse pas d�passer une certaine longueur commune au deux joueurs
    je Player1MoveCheckFin2
    add word [ds:TailleMaxJ1],10       ;On augmente la taille maximale du premier joueur de 10 pixels
    Player1MoveCheckFin2:
    mov byte [es:si],0                 ;On efface le point bonus en lui affectant la couleur 0 (celle du fond)
    Player1MoveCheckFin:
    cmp byte [es:si],0       ;On va faire maintenant la recherche de collision
    jne Player1Collision     ;   en comparant la couleur de la future position de la moto du joueur
    mov byte [es:si],1       ;Si il n'y a aucune collision, on affiche la moto sur la nouvelle position...
    ret

;Proc�dure repr�sentant les actions a �ffectuer lors d'une collision du joueur 1
Player1Collision:
    add sp,4                      ;On remet la pile au bon niveau
    inc byte [ds:PointsJoueur2]   ;On incr�mente les points du joueur adverse
    cmp word di,si                ;On compare la position des deux motos
    jne Player1CollisionPart2     ;   afin de rechercher l'�ventuel match nul
    inc byte [ds:PointsJoueur1]   ;Si match nul il y a, on incr�mente aussi les points du joueur courant
    Player1CollisionPart2:
    jmp GameContinue              ;On red�marre un nouveau combat...

;Voici maintenant une longue liste des actions du joueur 2
Player2AllerHaut:
    sub di,320
    call Player2MoveCheck
    ret
Player2AllerGauche:
    sub di,1
    call Player2MoveCheck
    ret
Player2AllerDroite:
    add di,1
    call Player2MoveCheck
    ret
Player2AllerBas:
    add di,320
    call Player2MoveCheck
    ret

;Proc�dure en rapport avec la recherche de collision et de rencontre avec un bonus dans le jeu
Player2MoveCheck:
    cmp byte [es:di],4
    jne Player2MoveCheckFin
    cmp word [ds:TailleMaxJ2],TailleMax
    je Player2MoveCheckFin2
    add word [ds:TailleMaxJ2],10  ;Le principe est identique � la proc�dure du joueur 1, sauf avec modifications propres pour le joueur 2
    Player2MoveCheckFin2:
    mov byte [es:di],0
    Player2MoveCheckFin:
    cmp byte [es:di],0
    jne Player2Collision
    mov byte [es:di],2
    ret

;Proc�dure repr�sentant les actions a �ffectuer lors d'une collision du joueur 1
Player2Collision:
    add sp,2
    inc byte [ds:PointsJoueur1]
    cmp di,si
    jne Player2CollisionPart2     ;Principe identique � la proc�dure du premier joueur
    inc byte [ds:PointsJoueur2]
    Player2CollisionPart2
    jmp GameContinue

; ***** Fin d'un combat
GameContinue:

xor ax,ax     ;On attend que l'un des deux joueurs appuie une quelconque touche pour continuer de joueur
int 16h       ;   On utilise pour cela, la fonction 0 de l'interruption 16h

;On v�rifie si un joueur � gagn� ou non
cmp byte [ds:PointsJoueur1],Partie+1
je Joueur1Gagne
cmp byte [ds:PointsJoueur2],Partie+1
je Joueur2Gagne
jmp @CONTINUE_GAME      ;On fait un saut jusqu'au label @CONTINUE_GAME qui repr�sente le point de d�part d'un nouveau jeu

;On va afficher un petit message de victoire
MatchNul:
mov word cx,TailleMNW
mov word bp,MatchNulWin
mov word bx,4
jmp AfficherGagnant

Joueur1Gagne:
pushad
cmp byte [ds:PointsJoueur2],Partie+1
je MatchNul
mov word cx,TailleJ1Win
mov word bp,Joueur1Win
mov word bx,1
jmp AfficherGagnant

Joueur2Gagne:
pushad
mov cx,TailleJ2Win
mov bp,Joueur2Win
mov word bx,2

AfficherGagnant:
call EffaceScreen
push word data
pop word es

mov word ax,1300h       ;On veut �x�cuter la fonction 13h de l'interruption 10h
mov byte dh,11          ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,0           ;Colonne contenu dans une constante
int 10h                 ;On fait appel � l'interruption 10h

xor ax,ax
int 16h

mov word ax,1300h       ;On veut �x�cuter la fonction 13h de l'interruption 10h
mov word bx,3           ;Attribut des caract�res de la cha�ne � afficher �gal � 1
mov word cx,TailleNPQ   ;Nombre de caract�res de la cha�ne � afficher
mov byte dh,11          ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,0           ;Colonne contenu dans une constante
mov word bp,NewPartieQ  ;Adresse dans ES de la cha�ne � afficher
int 10h                 ;On fait appel � l'interruption 10h
popad

DemandeNewPartie:
xor ax,ax
int 16h

cmp ah,24               ;24 est le scancode de la touche <O>
jne ContDemandeNP
jmp @NEW_GAME

ContDemandeNP:
cmp ah,49               ;49 est le scancode de la touche <N>
jne DemandeNewPartie

jmp VERITABLE_FIN_JEU   ;Si l'utilisateur a choisit de ne pas continuer, on quitte le jeu...

jmp @CONTINUE_GAME

; ***** Fin du jeu
GameFin:

call EffaceScreen            ;On va effacer l'�cran

push word es
push word data               ;ES = data
pop es

mov word ax,1300h            ;On veut �x�cuter la fonction 13h de l'interruption 10h
mov word bx,3                ;Attribut des caract�res de la cha�ne � afficher �gal � 1
mov word cx,TailleQuitter    ;Nombre de caract�res de la cha�ne � afficher
mov byte dh,10               ;Ligne o� l'on veut afficher la cha�ne
mov byte dl,2                ;Colonne contenu dans une constante
mov word bp,Quitter          ;Adresse dans ES de la cha�ne � afficher
int 10h                      ;On fait appel � l'interruption 10h

pop es        ;Restitution de ES

AttenteReponseQuitter:
xor ax,ax     ;On attend l'appui d'une touche
int 16h

cmp ah,24               ;24 est le scancode de la touche <O>
je VERITABLE_FIN_JEU

cmp ah,49               ;49 est le scancode de la touche <N>
jne AttenteReponseQuitter

jmp @CONTINUE_GAME      ;Si l'utilisateur a choisit de ne pas quitter, on revient au jeu, mais juste en recommencant le combat en cours

VERITABLE_FIN_JEU:
mov ax,0003h  ;On repasse en mode texte normal de type 80*25
int 10h

mov ah,4CH  ;On rend la main au dos et on stoppe le programme
int 21h


; ********** Proc�dures diverses

; Proc�dure de chargement d'une palette :
;-----------------------------------------
;   Param�tres :   ds:si     Tableau source
;                  cx        Nombre de couleurs * 3
loadPalette:
    pusha          ;Sauvegarde de l'�tat
    xor ax,ax      ;Met 0000 dans AX
    mov dx,3C8h    ;Met 03C8 dans DX
    out dx,al      ;Met 0 sur le port 3C8h
    mov dx,3c9h    ;Met 03C9 dans DX
    cld            ;Met le flag DF � 0
    rep outsb      ;Met les valeurs du tableau contenu dans DS:SI sur le port 3C9h
    popa           ;Restitution de l'�tat
    ret            ;Retour au niveau de l'appelant

; Proc�dure EffaceScreen :
;--------------------------
EffaceScreen:
    pushad
    push word 0A000h   ;ES = A000h
    pop word es
    xor ax,ax          ;Met AL � 0
    xor di,di          ;On commence par le premier pixel (celui de l'adresse ES:0000)
    mov word cx,64000  ;L'�cran compte 64000 pixels en tout... et on veut tous les colorier
    cld                ;Met le flag DF � 0 (Ainsi, stosb incr�mentera di � chaque fois)
    rep stosb          ;On va r�p�ter 64000 fois stosb...
    popad
    ret

; Proc�dure AffichePoints :
;---------------------------
AffichePoints:
    pusha          ;Sauvegarde de l'�tat

    xor ax,ax
    mov byte al,[ds:PointsJoueur1]
    mov byte dl,0Ah
    div byte dl                   ;Conversion des points du joueur 1 en ASCII
    add word ax,3030h
    mov word [ds:LblPointsJ1],ax

    xor ax,ax
    mov byte al,[ds:PointsJoueur2]
    mov byte dl,0Ah
    div byte dl                   ;Conversion des points du joueur 2 en ASCII
    add word ax,3030h
    mov word [ds:LblPointsJ2],ax

    mov word ax,1300h
    mov word bx,1
    mov word cx,2
    mov byte dh,24                ;Affichage des points du joueur 1
    mov byte dl,LblJ1Col+13
    mov word bp,LblPointsJ1
    int 10h

    mov byte bl,2
    mov byte dl,LblJ2Col+13
    mov word bp,LblPointsJ2       ;Affichage des points du joueur 2
    int 10h

    popa      ;Restitution de l'�tat
    ret

; Proc�dure d'attente :           (Proc�dure d'attente de retour du faisceau de l'�cran)
;-----------------------
Attente:
    pusha               ;Sauvegarde de l'�tat
    mov word dx,3DAh    ;Acc�de au port Input Status Register 1
    @inactif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jnz @inactif        ;Si AL<>0, alors le faisceau est �teint, on attend qu'il
                             ;rafra�chisse l'�cran pour �tre s�r que lorsqu'il sera inactif,
                             ;il sera �teint pendant environ 1/70e de seconde
    @actif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jz @actif           ;Si AL=0, alors le faisceau rafra�chit l'�cran, on attend qu'il a fini
    popa                ;Restitution de l'�tat initial
    ret                 ;Retour � l'instruction appelant la proc�dure

; Proc�dure qui fait appara�tre les bonus points en plein jeu :
;---------------------------------------------------------------
Etoile:
    pusha                    ;Sauvegarde de l'�tat des registres g�n�raux
    xor ax,ax
    in al,40h
    xchg al,ah
    in al,40h                ;On aura � ce moment-l�, AX al�atoire
    mov word di,ax           ;On va mettre le contenu de AX dans un registre d'index
    cmp byte [es:di],0       ;Je veux que les �toiles ne peuvent appara�tre que dans des zones vides de tout �l�ment
    jne EtoileFin
    cmp word di,LigBas*320   ;Je ne veux rien inscrire dans la zone d'affichage des scores
    ja EtoileFin
    mov byte [es:di],4       ;Affichage d'un bonus � un endroit al�atoire
    EtoileFin:
    popa                     ;Restitution de l'�tat initial
    ret                      ;Retour au niveau de l'appellant

;Proc�dures de gestion de la taille constante de chacun des deux joueurs
PermutFileJoueur1:
    pushad                   ;Sauvegarde de l'�tat initial
    mov word cx,TailleJ1
    mov word bx,CooJ1        ;Pr�paration des param�tres pour l'appel de la proc�dure PermutFile
    mov word di,TailleMaxJ1
    call PermutFile          ;Appel de la proc�dure PermutFile
    popad                    ;Restitution de l'�tat initial
    ret                      ;Retour au niveau de l'appelant

PermutFileJoueur2:
    pushad                   ;Sauvegarde de l'�tat initial
    mov word cx,TailleJ2
    mov word bx,CooJ2        ;Pr�paration des param�tres pour l'appel de la proc�dure PermutFile
    mov word si,di
    mov word di,TailleMaxJ2
    call PermutFile          ;Appel de la proc�dure PermutFile
    popad                    ;Restitution de l'�tat initial
    ret                      ;Retour au niveau de l'appelant

PermutFile:   ;Proc�dure principale de gestion de file circulaire
    push si                  ;Sauvegarde de si
    mov word si,[ds:di]      ;R�cup�ration de la taille maximale du joueur
    dec si                   ;On d�cr�mente cette taille d'une unit�
    add si,si                ;On la multiplie par 2
    mov word dx,[ds:bx+si]   ;On r�cup�re le derni�re �l�ment de la file du joueur
    BclePermutFileJoueur1:             ;Proc�dure dite de d�calement
         cmp word si,0                 ;On compare SI � 0
         je FIN_BclePermutFileJoueur1  ;Si c'est �gal, on quitte la proc�dure de d�calement
         sub word si,2                 ;On retranche 2 � SI
         mov word ax,[ds:bx+si]        ;On copie le contenu de la case courante de la file dans AX...
         mov word [ds:bx+si+2],ax      ;...et on la recopie un emplacement plus loin
         jmp BclePermutFileJoueur1     ;On recommence la boucle de d�calement
    FIN_BclePermutFileJoueur1:
    pop si                   ;On r�cup�re SI
    mov word [ds:bx],si      ;On place SI en t�te de la file du joueur courant
    mov word ax,[ds:di]      ;On r�cup�re dans AX la taille maximale de la file du joueur
    mov word di,cx           ;On r�cup�re dans DI l'adresse de la taille actuelle de la file du joueur
    cmp word [ds:di],ax      ;On compare la taille actuelle du joueur � la taille maximale courante de ce joueur
    je EffDerPointJ1         ;   si elles sont �gales, on efface le dernier point de la train�e du joueur
    inc word [ds:di]         ;Sinon, on incr�mente la taille actuelle du joueur d'une case sans effacer un quelconque point
    jmp FIN_PermutFileJoueur1

    EffDerPointJ1:
    mov word si,dx           ;On r�cup�re les coordonn�es du point terminal de la file dans SI
    mov byte [es:si],0       ;On y met la couleur du fond...

    FIN_PermutFileJoueur1:
    ret                      ;Retour au niveau de l'appelant

end      ;Fin total du programme