;
;          浜様様様様様様様様様様様様様様様様様様様様様様様様様融
;          �  樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛  �
;          �  � 樛樛樛樛�   樛樛樛�    樛樛樛樛   樛      樛 �  �
;          �  � 烝炳栩烝�  栩烝烝炳�  栩烝烝烝栩  栩桀    栩 �  �
;          �  �    栩�     栩樛樛樂�  栩      栩  栩 炳�  栩 �  �
;          �  �    栩�     栩  炳桀   栩      栩  栩   炳樂� �  �
;          �  �    栩�     栩    炳�  炳栩栩栩桎  栩     炳� �  �
;          �  桀樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樛樂  �
;          �                                                    �
;          藩様様様曜様様様様様様様様様様様様様様様様様僕様様様夕
;                   �  FUCHS Steve & SCHNEIDER J�rome  �
;                   藩様様様様様様様様様様様様様様様様夕
;

;
;   PARTIE 1 : Introduction
;  様様様様様様様様様様様様�
;
;        Introduction graphique ("Tron" affich� qui s'effacera doucement avec un effet Blur)
;

;Initialisation du STACK Segment
mov ax,stack
mov ss,ax
mov sp,StackTop

;Initialisation du DATA Segment
mov ax,dataIntro
mov ds,ax

;Initialisation de l'EXTRA Segment
mov ax,0x0A000
mov es,ax

;Passage en mode graphique 320*200 en 256 couleurs
mov ax,13h
int 10h

;Appel de la proc�dure de d�finition de la palette de couleurs
call Palette

;Appel de la proc�dure d'affichage du titre "TRON"
call Affichage

;On va op�rer un passage de l'effet BLUR sur l'�cran
mov cx,1      ;Met CX � 1 (on ne veut effectuer qu'un passage de BLUR)
call Blur     ;Appel de la procdure BLUR

;Appel de la proc�dured'attente
call Intro_Attente

;On va maintenant op�rer 50 passages de l'effet BLUR sur l'�cran
mov cx,50     ;Met CX � 50 (50 images)
call Blur     ;Appel de la proc�dure BLUR

;On quitte le module INTRO.INC
jmp @INTRO_END


; *********** Proc�dures diverses

; Proc�dure Palette qui permet de d�finir la palette courante utilis�e dans ce module
Palette:
    xor bx,bx      ;Initialisation des nuances / BX : n� de la couleur
    xor cx,cx      ;CH = Taux de vert / CL = Taux de bleu
    xor byte dh,dh ;DH = Taux de rouge

    BouclePalette:
         mov ax,1010h   ;AH = code de la fonction palette / AL = code '�criture' dans la palette
         int 10h        ;Appel de l'interruption avec les param�tres bx,ch,cl & dh
         inc bx         ;Couleur suivante
         inc cl         ;Niveau de bleu suivant
         cmp cl,63           ;Si niveau de bleu <= 63
         jbe BouclePalette   ;   alors traiter couleur suivante
    ret

; Proc�dure Affichage qui affiche le "TRON" � l'�cran
Affichage:
    mov si,22776   ;ES:22776, adresse du premier point � partir duquel l'image sera inscrite
    xor di,di      ;Met DI � 0
    mov bx,tron    ;Affecte � BX l'adresse du tableau dataIntro:tron qui contient l'image de titre de fa�on cod�e
    mov cx,208     ;Met CX � 208 (longueur d'une ligne du tableau tron)

    BoucleAffichage:
         mov al,[bx+di]           ;R�cup�re dans AL le contenu de la DI�me case du tableau tron
         cmp al,' '               ;Compare AL avec le caract�re <ESPACE>
         jne NonEspace            ;Si ce n'est pas �gal, on fait le saut
         mov al,0                 ;Met AL � 0 (couleur noire)
         NonEspace:
         mov [es:si],al           ;Place un point de couleur AL � l'adresse SI de l'EXTRA Segment

         inc di                   ;Incr�mentation de DI
         inc si                   ;Incr�mentation de SI
         loop BoucleAffichage     ;Incr�mentation de CX, comparaison de CX � 0, si ce n'est pas �gal, on fait le saut

    mov cx,208               ;Remise de CX � 208
    add si,320-208           ;On ajoute 112 � SI
    inc byte [compteur]      ;On incr�mente le compteur
    cmp byte [compteur],58   ;On compare ce compteur � 58
    jb BoucleAffichage       ;Si c'est inf�rieur, on effecte le saut
ret

; Proc�dure de l'effet BLUR
Blur:
    mov si,41600
    BoucleBlur:
        mov al,[es:si-320]   ;blur optimis�
        add al,[es:si-1]     ;seuls les pixels N-S-E-O sont additionn�s
        add al,[es:si+1]     ;registre 8 bits utilisable car
        add al,[es:si+320]   ;4 * 63 < 255 ( 0xFF )
        shr al,2             ;divise par 4 => moyenne
        mov [es:si],al       ;place la nouvelle couleur dans le pixel central
        dec si
        jnz BoucleBlur
        loop Blur
ret

; Proc�dure d'attente   (Proc�dure d'attente de retour du faisceau de l'�cran)
Intro_Attente:
    pusha               ;Sauvegarde de l'�tat
    mov word dx,3DAh    ;Acc�de au port Input Status Register 1
    @INTRO_inactif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jnz @INTRO_inactif  ;Si AL<>0, alors le faisceau est �teint, on attend qu'il
                             ;rafra�chisse l'�cran pour �tre s�r que lorsqu'il sera inactif,
                             ;il sera �teint pendant environ 1/70e de seconde
    @INTRO_actif:
         in al,dx            ;Lit 8 bits sur le port et le stocke dans AL
         and al,08h          ;ET LOGIQUE entre AL et 8 (teste le bit 3)
         jz @INTRO_actif     ;Si AL=0, alors le faisceau rafra�chit l'�cran, on attend qu'il a fini
    popa                ;Restitution de l'�tat initial
    ret                 ;Retour � l'instruction appelant la proc�dure

;Fin de la partie INTRODUCTION du programme Tron
@INTRO_END: