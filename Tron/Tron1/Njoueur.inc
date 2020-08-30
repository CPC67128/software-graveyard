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
;   PARTIE 2 : Demande des noms des joueurs
;  ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
;
;        Demande des noms des joueurs (partie r‚alis‚e en mode texte)
;

;D‚finiton de la macro setCursPos
%macro setCursPos 2
    pushad              ;Sauvegarde de l'‚tat des registres
    mov ah,02h          ;Met AH … 02
    xor bx,bx           ;Met BX … 0
    mov dh,%1           ;Met le premier paramŠtre dans DH
    mov dl,%2           ;Met le second paramŠtre dans DL
    int 10h             ;Appel de l'interruption 10h
    popad               ;Restitution de l'‚tat des registres
%endmacro

;Initialisation du DATA Segment DS
mov word ax,dataNJ
mov ds,ax

;Initialisation du DATA Segment DS
mov word ax,dataNJ
mov ds,ax

;Initialisation de l'EXTRA Segment ES avec l'adresse m‚moire 0B800
mov ax,0B800h
mov es,ax

;Initialisation du STACK Segment SS
mov word ax,stack
mov ss,ax
mov sp,StackTop

;Initialisation du mode vid‚o-texte en 80*25
mov ax,3h
int 10h

;On va effacer l'‚cran avec un arriŠre-plan noir, des caractŠres nuls en jaunes
xor di,di
mov word ax,0E20h
mov cx,80*25
rep stosw

;On va pr‚parer les attributs des caractŠres qui composeront le titre
mov di,80*2*2
mov ax,0E00h

@NomJoueur_PreparerCar1:
    mov cx,80
    dec ah
    rep stosw
    cmp byte ah,07h
    jne @NomJoueur_PreparerCar1

;On va inscrire le titre (selon une m‚thode un peu archa‹que, j'avoue !)
xor si,si
mov di,80*2*2+34
@NomJoueur_EcrireTitre:
    mov byte al,[ds:NomJoueurTitre1+si]
    mov byte [es:di],al
    mov byte al,[ds:NomJoueurTitre2+si]
    mov byte [es:di+160],al
    mov byte al,[ds:NomJoueurTitre3+si]
    mov byte [es:di+160*2],al
    mov byte al,[ds:NomJoueurTitre4+si]
    mov byte [es:di+160*3],al
    mov byte al,[ds:NomJoueurTitre5+si]
    mov byte [es:di+160*4],al
    mov byte al,[ds:NomJoueurTitre6+si]
    mov byte [es:di+160*5],al
    mov byte al,[ds:NomJoueurTitre7+si]
    mov byte [es:di+160*6],al
    inc si
    add di,2
    cmp byte [ds:NomJoueurTitre1+si],'$'
    jne @NomJoueur_EcrireTitre

;On va inscrire la chaŒne 'Nom du joueur 1 : ' … sa place
mov si,80*2*11+48
mov di,NomJoueurChaineDemJ1
@NomJoueur_InsDemJ1:
    mov byte al,[ds:di]
    mov byte [es:si],al
    inc di
    add si,2
    cmp byte [ds:di],'$'
    jne @NomJoueur_InsDemJ1

;On va inscrire la chaŒne 'Nom du joueur 2 : ' … sa place
mov si,80*2*15+48
mov di,NomJoueurChaineDemJ2
@NomJoueur_InsDemJ2:
    mov byte al,[ds:di]
    mov byte [es:si],al
    inc di
    add si,2
    cmp byte [ds:di],'$'
    jne @NomJoueur_InsDemJ2

;On va maintenant entrer dans la boucle de demande d'insertion de nom
xor bx,bx
xor cx,cx
mov byte dh,[ds:NJILig]      ;Initialisation de quelques registres
mov byte dl,[ds:NJICol]
mov word si,0734h

@NomJoueur_BcleInsNom:

    setCursPos dh,dl    ;Positionnement du curseur

    xor ax,ax      ;On met ax … 0 afin d'utiliser la fonction 0 de l'int‚ruption 16h
    int 16h        ;J'attend que l'utilisateur appuie une touche

    cmp al,27           ;Je compare le num‚ro de cette touche avec celui de la touche <Echap>
    jne @NJI_QCont      ;Si c'est ‚gal, on sort de la proc‚dure d'insertion des noms
    jmp @NJI_BcleQuitter;Simple problŠme d'emplacement dans le fichier (NJI_Quitter est trop "‚loign‚e" d'ici)

    @NJI_QCont:

    cmp al,13           ;Je compare le num‚ro de cette touche avec celle de la touche <entr‚e>
    je @NJI_Suivant     ;Si c'est ‚gal, on passe … la zone d'insertion du nom du joueur adverse

    cmp al,8       ;On compare avec la touche BackSpace
    je @NJI_bs     ;Si c'est ‚gal, on ‚x‚cute le traitement appropri‚

    cmp al,36      ;Je veux pas de caractŠres '$'
    je @NomJoueur_BcleInsNom

    cmp al,32      ;On ne veut pas des touches de fonctions, Tabulations, ...
    jb @NomJoueur_BcleInsNom

    cmp al,165     ;On ne veut pas des codes claviers ‚tendus... (je ne veut pas que l'utlisateur
    ja @NomJoueur_BcleInsNom

    cmp byte [ds:NJITaille+bx],10      ;On regarde la taille du nom courant d‚j… entr‚ et on la compare … 10
    je @NomJoueur_BcleInsNom           ;Si c'est ‚gal, on ne fait rien

    mov byte [es:si],al      ;Inscrit … l'‚cran la touche appuy‚e
    add si,2                 ;On passe … l'aspace caractŠre suivant

    inc byte [ds:NJITaille+bx]    ;On incr‚mente la taille des noms des joueurs
    inc byte [ds:NJICol+bx]
    inc dl
    jmp @NomJoueur_BcleInsNom

    @NJI_bs:
         cmp byte [ds:NJITaille+bx],0
         je @NomJoueur_BcleInsNom
         sub si,2
         dec byte [ds:NJITaille+bx]
         dec byte [ds:NJICol+bx]
         dec dl
         mov byte [es:si],' '
         jmp @NomJoueur_BcleInsNom

    @NJI_Suivant:
         not bx
         and bx,0001h

         push ax   ;Sauvegarde de deux trois paramŠtres
         push dx
         push cx

         ;On va maintenant calculer l'offset de la position du caractŠre courant dans le segment 0B800 de la m‚moire
         xor ax,ax
         mov byte al,[ds:NJILig+bx]
         mov byte cl,05h
         mul cl
         shl ax,5
         mov dx,ax

         xor ax,ax
         mov byte al,[ds:NJICol+bx]
         mov byte cl,02h
         mul cl

         add ax,dx
         mov word si,ax

         pop cx
         pop dx
         pop ax

         mov byte dh,[ds:NJILig+bx]
         mov byte dl,[ds:NJICol+bx]

         jmp @NomJoueur_BcleInsNom

@NJI_BcleQuitter:

;On va maintenant r‚cup‚rer les noms des deux joueurs...
mov ax,dataNJ
mov ds,ax

mov word si,80*2*11+42*2
xor di,di
@NJI_RecJ1:
    mov byte ah,[es:si]
    mov byte [ds:NomJoueur1+di],ah
    add si,2
    inc di
    cmp word di,0Ah
    jne @NJI_RecJ1

cmp byte [ds:NJITaille],0
jne @NJI_RecJ1Fin
mov si,NomJoueur1
mov di,NJoueur1Def
xor bx,bx
@NJI_RecJ1Def:
    mov byte ah,[ds:di+bx]
    mov byte [ds:si+bx],ah
    inc bx
    cmp word bx,0Ah
    jne @NJI_RecJ1Def

@NJI_RecJ1Fin:
mov word si,80*2*15+42*2
xor di,di
@NJI_RecJ2:
    mov byte ah,[es:si]
    mov byte [ds:NomJoueur2+di],ah
    add si,2
    inc di
    cmp word di,0Ah
    jne @NJI_RecJ2

cmp byte [ds:NJITaille+1],0
jne @NJI_RecJ2Fin
mov si,NomJoueur2
mov di,NJoueur2Def
xor bx,bx
@NJI_RecJ2Def:
    mov byte ah,[ds:di+bx]
    mov byte [ds:si+bx],ah
    inc bx
    cmp word bx,0Ah
    jne @NJI_RecJ2Def

@NJI_RecJ2Fin:
mov ax,dataNJ
mov ds,ax

;... et ensuite aligner ces noms … droite dans leurs espaces m‚moires r‚spectifs
mov word bx,NomJoueur1
mov di,09h
xor si,si

@NJI_Alig1:
    cmp byte [ds:bx+di],20h
    je @NJI_Per1
    cmp byte [ds:bx+di],00h
    jne @NJI_Alig1Fin
    jmp @NJI_Alig1
    @NJI_Per1:
         pusha
         call NJIpermutationCh
         popa
    jmp @NJI_Alig1
@NJI_Alig1Fin:


mov word bx,NomJoueur2
mov di,09h
xor si,si

@NJI_Alig2:
    cmp byte [ds:bx+di],20h
    je @NJI_Per2
    cmp byte [ds:bx+di],00h
    jne @NJI_Alig2Fin
    jmp @NJI_Alig2
    @NJI_Per2:
         pusha
         call NJIpermutationCh
         popa
    jmp @NJI_Alig2
@NJI_Alig2Fin:

jmp @NJOUEUR_FIN

; Proc‚dure qui ‚ffectue une permutation vers la droite des ‚l‚ments qui compose une variable en m‚moire
NJIpermutationCh:
    push bp
    mov word bp,sp
    mov byte ah,[ds:bx+di]

    NJIpermuBcle:
    cmp word di,0
    je NJIpermutFin
    dec word di
    mov byte al,[ds:bx+di]
    mov byte [ds:bx+di+1],al
    jmp NJIpermuBcle

    NJIpermutFin:
    mov byte [ds:bx+di],ah

    pop bp
    ret

@NJOUEUR_FIN:

;On va maintenant exporter les noms des deux joueurs dans le segment de donn‚es rincipal du corps du jeu
mov ax,dataJeu
mov es,ax

xor bx,bx
@ExportNomJ1:
    mov byte ah,[ds:NomJoueur1+bx]
    mov byte [es:LblJoueur1+bx],ah
    inc word bx
    cmp word bx,0Ah
    jne @ExportNomJ1

xor bx,bx
@ExportNomJ2:
    mov byte ah,[ds:NomJoueur2+bx]
    mov byte [es:LblJoueur2+bx],ah
    inc word bx
    cmp word bx,0Ah
    jne @ExportNomJ2

;Fin du module NJOUEUR.INC