;Initialisation de l'EXTRA Segment
mov ax,0A000h
mov es,ax

;Initialisation du STACK Segment, c'est � dire de la pile
mov ax,stack
mov ss,ax
mov sp,StackTop

;Initialisation du DATA Segment
mov ax,dataFinal
mov ds,ax

;Passage en mode graphique 320*200 en 256 couleurs
mov ax,13h
int 10h

;Appel de la proc�dure qui permet d'�mettre une petite musique de fin
call SonFIN

;Affichage de la premi�re bitmap
mov dx,fichier1
call Bitmap        ;Appel de la proc�dure Bitmap

;On va effacer l'�cran
call effaceEcran

;Affichage de la seconde bitmap
mov dx,fichier3
call Bitmap

mov ax,03h
int 10h


mov ah,2
xor bh,bh
mov dh,24
mov dl,0
int 10h

mov ax,credits
mov ds,ax

mov ah,01
mov cx,2000h
int 10h

mov dx,l1
call Affichage2
mov dx,l2
call Affichage2
mov dx,l3
call Affichage2
mov dx,l4
call Affichage2
mov dx,l5
call Affichage2
mov dx,l6
call Affichage2
mov dx,l7
call Affichage2
mov dx,l8
call Affichage2
mov dx,l15
call Affichage2
mov dx,l10
call Affichage2
mov dx,l15
call Affichage2
mov dx,l15
call Affichage2
mov dx,l12
call Affichage2
mov dx,l14
call Affichage2
mov dx,l15
call Affichage2
mov dx,l15
call Affichage2
mov dx,l16
call Affichage2
mov dx,l15
call Affichage2
mov dx,l18
call Affichage2
mov dx,l19
call Affichage2
mov dx,l15
call Affichage2
mov dx,l15
call Affichage2
mov dx,l21
call Affichage2
mov dx,l15
call Affichage2
mov dx,l22
call Affichage2
mov dx,l15
call Affichage2
mov dx,l15
call Affichage2
mov dx,l23
call Affichage2
mov dx,l15
call Affichage2
mov dx,l24
call Affichage2
mov dx,l25
call Affichage2
mov dx,l15
call Affichage2
mov dx,l15
call Affichage2
mov dx,l26
call Affichage2
mov dx,l15
call Affichage2
mov dx,l27
call Affichage2
mov dx,l28
call Affichage2
mov dx,l29
call Affichage2
mov dx,l30
call Affichage2
mov dx,l31
call Affichage2
mov dx,l15
call Affichage2
mov dx,l32
call Affichage2
mov dx,l15
call Affichage2
mov dx,l33
call Affichage2
mov dx,l15
call Affichage2
mov dx,l34
call Affichage2
mov dx,l35
call Affichage2
mov dx,l15
call Affichage2
mov dx,l36
call Affichage2
mov dx,l37
call Affichage2
mov dx,l15
times 25 call Affichage2

mov ah,2
xor bh,bh
mov dh,0
mov dl,0
int 10h

mov ax,4C00h
int 21h


; Proc�dure Bitmap qui permet l'affichage d'une image bitmap
Bitmap:
mov ah,0x3D   ; ouverture du fichier, sachant que dx contient de ce fichier
mov al,0x90
int 21h

mov bx,ax     ; sauvegarde du handle
mov [handle],ax

mov ah,0x3F   ; lecture des 2000 premiers octets pour d�coder la palette
mov cx,2000
mov dx,buffer      ; adresse de destination des octets
int 21h


FINALPalette:

xor bx,bx
xor si,si

FINALBouclePalette:
    mov cl,[buffer+54+si]    ; traitement de la palette
    inc si
    mov ch,[buffer+54+si]    ; les octets codants la palette se situent
    inc si
    mov dh,[buffer+54+si]    ; � partir de l'octet 54
    inc si
    inc si    ; 2 incr�mentations, car les couleurs sont cod�es sur 4 octets
              ; ( B V R L )
    shr cl,2  ; division de chaque niveau par 4 ( cod� de 0 � 255, 0 � 63 utile )
    shr ch,2
    shr dh,2

    mov ax,0x1010       ; code de la fonction

    int 10h

    inc bx    ; couleur de palette suivante

    cmp bx,0xFF    ; si derni�re couleur, fin du traitement
    jbe FINALBouclePalette


mov ah,42h    ; fonction de pointage dans un fichier
mov al,0
mov bx,[handle]
mov cx,0
mov dx,1078   ; on se d�place en fait apr�s la palette ( 256 * 4 + 54 )
int 21h


mov ah,0x3F
mov bx,[handle]
mov cx,64000       ; lecture des 64000 octets de l'image
mov dx,buffer
int 21h

    mov cx,16000 ; affichage a l'�cran ( 16000 double-mots, soit 64000 octets )
    mov si,buffer
    xor di,di
    rep movsd      ; copi�s de l'adresse ds:si vers es:di

mov ah,0h
int 16h       ; attente de l'appui d'une touche

ret

; Proc�dure effaceEcran

effaceEcran:
    mov ax,03h
    int 10h        ; astuce pour effacer l'�cran : changer de mode graphique
    mov ax,13h
    int 10h
    ret

SonFIN:
mov al,182
out 67,al     ;Ecriture sur le port 67h de 182d

mov ax,1099h
Descente:               ;On va jouer les notes du style 1X99h avec X allant de 0 � F
    call note
    inc byte ah
    cmp byte ah,1Fh
    jbe Descente

mov cx,3
NiveauBasSon:
    mov ax,0x1f00       ;On joue 3 fois la note 1F00h
    call note
    loop NiveauBasSon

call desactiveHP   ;D�sactivation du PC-Speaker

ret

; Proc�dure d'activation du PC-Speaker :
;----------------------------------------
activeHP:
pusha         ;Sauvegarde de l'�tat initial
in al,97      ;On r�cup�re dans AL l'�tat du port 97, ce port correspond � celui du PC-Speaker
or al,3       ;Les deux premiers bits (0 et 1) correspondent � l'�tat du PC-Speaker, qui est d�sactiv� par d�faut... on les inverse !
out 97,al     ;On envoie sur le port 97 le contenu de AL, c'est � dire le code d'activation du PC-Speaker
popa          ;restitution de l'�tat intial
ret           ;Retour au niveau de l'appelant

; Proc�dure de d�sactivation du PC-Speaker :
;--------------------------------------------
desactiveHP:
pusha         ;Sauvegarde de l'�tat initial
in al,97      ;On r�cup�re dans AL l'�tat du port 97, ce port correspond � celui du Haut-Parleur syst�me
and al,252    ;On met les deux bits de d�but de AL � 0 ce qui correspond au code de d�sactivation du PC-Speaker
out 97,al     ;On envoie ce code sur le port 97
popa          ;On restitue l'�tat des registres d'avant l'appel de la proc�dure
ret           ;Retour au niveau de l'appelant

; Proc�dure de temporisation de la note :
;-----------------------------------------
loadTimer:
out 66,al     ;On envoie l'octet de poids le plus faible sur le port 66
mov al,ah     ;On copie AH dans AL
out 66,al     ;On envoie l'octet de poids le plus fort sur le port 66
ret           ;Retour au niveau de l'appelant

; Proc�dure note :
;------------------
note:
call loadTimer     ;Appel de la proc�dure loadTimer
call activeHP      ;Activation du PC-Speaker
push word cx       ;Sauvegarde de CX
mov cx,8           ;Met CX � 8
LongueurSon:
    call FINALAttente        ;Appel de la proc�dure Attente
    loop LongueurSon
pop word cx        ;Restitution du registre CX
ret                ;Retour au niveau de l'appelant

; Proc�dure d'attente :
;-----------------------
FINALAttente
    pusha               ;Sauvegarde de l'�tat
    pushf
    mov word dx,3DAh    ;Acc�de au port Input Status Register 1
    @FINALinactif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jnz @FINALinactif        ;Si AL<>0, alors le faisceau est �teint, on attend qu'il
                             ;rafra�chisse l'�cran pour �tre s�r que lorsqu'il sera inactif,
                             ;il sera �teint pendant environ 1/70e de seconde
    @FINALactif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jz @FINALactif           ;Si AL=0, alors le faisceau rafra�chit l'�cran, on attend qu'il a fini
    popf
    popa                ;Restitution de l'�tat initial
    ret                 ;Retour � l'instruction appelant la proc�dure

Attente2:
    pusha
    mov word dx,3DAh
    @AttenteBcle1:
         in al,dx
         and al,08h
         jnz @AttenteBcle1
    @AttenteBcle2:
         in al,dx
         and al,08h
         jz @AttenteBcle2
    popa
    ret

Affichage2:
    mov ah,09h
    int 21h
    times 15 call Attente2
    ret

;Segment contenant les variables concernant le module FINAL.INC
segment credits
         l1  db '               ������������������������������������������������',10,13,'$'
         l2  db '               � ���������   �������    ��������   ��      �� �',10,13,'$'
         l3  db '               � ���������  ���������  ����������  ����    �� �',10,13,'$'
         l4  db '               �    ���     ���������  ��      ��  �� ���  �� �',10,13,'$'
         l5  db '               �    ���     ��  ����   ��      ��  ��   ����� �',10,13,'$'
         l6  db '               �    ���     ��    ���  ����������  ��     ��� �',10,13,'$'
         l7  db '               ������������������������������������������������',10,13,10,'$'
         l8  db '                           Producteurs executifs  :',10,13,'$'
         l10 db '            Steve "WONDER" Fuchs           J�r�me "BEACH" Schneider',10,13,'$'
         l12 db '                            Conseiller technique  :',10,13,'$'
         l14 db '                             J�r�my "JASON" Wagner',10,13,'$'
         l15 db 10,'$'
         l16 db '                                  Cascadeurs :',10,13,'$'
         l18 db '           Joseph "BOMBARDIER" Roller           Ludovic "ASM MASTER" Cadart',10,13,'$'
         l19 db '           Deruvyer    Aline                        Bernard "KILLER" Kittel',10,13,'$'
         l21 db '                                     Bouffe:',10,13,'$'
         l22 db '                                 Eric Prevotooo',10,13,10,'$'
         l23 db '                                    Dresseurs :',10,13,'$'
         l24 db '                                 Raoul  Rosenthal',10,13,'$'
         l25 db '                                 Marcel   Steffan',10,13,'$'
         l26 db '                                  Remerciements :',10,13,'$'
         l27 db '           Grand merci � Marcel, Raoul, Mouloud et toute la City',10,13,'$'
         l28 db '                             La mairie de Barrseille',10,13,'$'
         l29 db '                  UCLA,  The Masschussets Institute of Technology, NASA',10,13,'$'
         l30 db '                  Adisasss, Pizza HOT, Heineken, ASPRO 500 effervescent',10,13,'$'
         l31 db '                       3D REALMS pour le culturisme de Mr. Steve',10,13,'$'
         l32 db '          Aucun  animal n"a �t� bless� ni maltrait� durant le tournage',10,13,'$'
         l33 db '            Les faits et r�cits de cette production sont imaginaires',10,13,'$'
         l34 db '        Toute ressemblance avec des personnes existantes ou ayant exist�',10,13,'$'
         l35 db '                              serait purement fortuite',10,13,'$'
         l36 db '                     Soundtracks availables on Sony MCA records',10,13,'$'
         l37 db '                            (c) Copyright 2000 World Co.','$'

SEGMENT dataFinal
    fichier3  db   'jerome.bmp',0
    fichier1  db   'steve.bmp',0
    handle    dw   0
    buffer    resb 0

