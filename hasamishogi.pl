%AUTORES: TIAGO BABO, FELIPE SCHMITT, HELDER MOREIRA
%UTILITARIOS
:-use_module(library(random)).
:-use_module(library(lists)).

conv(Let,Valor):- maiuscula(Let), Valor is Let-64.
conv(Let,Valor):- minuscula(Let), Valor is Let-96.
conv(Let,Valor):- numero(Let), Valor is Let-48.

maiuscula(Let):- Let>=65, Let=<74.
minuscula(Let):- Let>=97, Let=<106.
numero(Let):- Let>=49, Let=<58.

letra(a, 1). letra(b, 2). letra(c, 3). letra(d, 4). letra(e, 5). letra(f, 6). letra(g, 7). letra(h, 8). letra(i, 9).

troca(1,2).
troca(2,1).

if(Condition, TrueClause, FalseClause) :-
	Condition, !, TrueClause;
       !, FalseClause.

choose([], []).
choose(List, Elt) :-
        length(List, Length),
        random(0, Length, Index),
        nth0(Index, List, Elt).

cls :- put(27), put("["), put("2"), put("J").

%DESENHAR O TABULEIRO
linhaLimite:-printLinha([' ',*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,' ']).
linhaLetras:-printLinha([' ',' ',' ',' ',' ',' ','a',' ',' ',' ',' ',' ',' ',' ','b',' ',' ',' ',' ',' ',' ',' ','c',' ',' ',' ',' ',' ',' ',' ','d',' ',' ',' ',' ',' ',' ',' ','e',' ',' ',' ',' ',' ',' ',' ','f',' ',' ',' ',' ',' ',' ',' ','g',' ',' ',' ',' ',' ',' ',' ','h',' ',' ',' ',' ',' ',' ',' ','i',' ',' ',' ',' ',' ']).
linhaNumerosV(['1','2','3','4','5','6','7','8','9']).
linhaDivH:-printLinha([' ',*,' ',-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,' ',*]).

tabuleiro(
	 [[1,1,1,1,1,1,1,1,1],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [2,2,2,2,2,2,2,2,2]]).
/*tabuleiro(
	 [[1,1,0,0,0,0,0,0,0],
	  [0,2,0,0,0,0,0,0,0],
	  [2,0,2,0,2,0,0,0,0],
	  [0,1,0,0,2,0,0,0,0],
	  [2,0,2,2,2,0,0,0,0],
	  [2,0,2,2,0,0,0,0,0],
	  [2,0,0,0,2,0,0,0,0],
	  [0,2,2,2,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0]]).*/
/*tabuleiro(
	 [[0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,1,0,0,0,0,0,0],
	  [0,0,0,2,1,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0],
	  [0,0,0,0,0,0,0,0,0]]).*/

piece1(1):- write(' ----- |').
piece1(2):- write(' ----- |').
piece1(0):- write('       |').

piece2(1):- write('|  1  ||').
piece2(2):- write('|  2  ||').
piece2(0):- write(('       |')).

piece3(1):- write(' ----- |').
piece3(2):- write(' ----- |').
piece3(0):- write('       |').

printLinhaPeca([]).
printLinhaPeca([A|R]):-	piece1(A), printLinhaPeca(R).
printLinhaPeca3([]).
printLinhaPeca3([A|R]):-piece3(A), printLinhaPeca3(R).
printLinhaPeca2([]).
printLinhaPeca2([A|R]):-piece2(A), printLinhaPeca2(R).
printLinha([]).
printLinha([A|R]):-write(A), printLinha(R).
printPecas([],[]).
printPecas([A|R],[X|Y]):- write(' *|'), printLinhaPeca(A), write('*'), nl, write(X), write('*|'), printLinhaPeca2(A), write('*'), nl, write(' *|'), printLinhaPeca3(A), write('*'), nl, linhaDivH, nl, printPecas(R, Y).

desenha(Tabuleiro):- linhaNumerosV(X),linhaLetras, nl, linhaLimite, nl, linhaDivH, nl,printPecas(Tabuleiro,X),linhaLimite.

%FIM DO DESENHO DO TABULEIRO

%ESCOLHA DOS MODOS DE JOGO
iniciaJogo:-cls, menu.
menu:-
    nl,nl,write('         *** MENU *** '), nl,nl,
    write('1 - Jogador vs CPU'),nl,
    write('2 - Jogador vs Jogador'),nl,
    write('3 - CPU vs CPU'),nl,
    write('4 - Como Jogar?'),nl,
    write('5 - Sair'),nl,
    write('Escolha uma opcao (ex: 1) : '), get_code(Op),skip_line,Op>=49,conv(Op,Esc),
    verifica(Esc).

verifica(1):-jVsCpu, !.
verifica(2):-jVsJ, !.
verifica(3):-cpuVsCpu,!.
verifica(4):-descricaoJogo,menu,!.
verifica(5):-write('Até logo...'),nl,!.
verifica(_):-menu.


modoJ1(T,Jogador,Modo):-
	(Modo == -1;Modo == 1;Modo==2;Modo==3),
	modoJogador(T,Jogador,Modo), !.
modoJ1(T,Jogador,Modo):-
	(Modo == 4;Modo == 5;Modo==6),
	modoCPU(T,Jogador,Modo), !.
modoJ2(T,Jogador,Modo):-
	(Modo == 1;Modo==2;Modo==3;Modo==4,Modo==5;Modo==6),
	modoCPU(T,Jogador,Modo),!.
modoJ2(T,Jogador,Modo):-modoJogador(T,Jogador,Modo).

%MODO JOGADOR VS JOGADOR

jVsJ:-tabuleiro(T), modoJ1(T, 1, -1).

%MODO JOGADOR VS CPU

jVsCpu:-
	write('1 - Fácil'),nl,
	write('2 - Intermédio'), nl,
	write('3 - Difícil'),nl,
	write('4 - para sair'),nl,
	repeat,write('Opcao (Ex: 1) : '), get_code(Op),skip_line,Op>=49, Op=<53,conv(Op,Esc),
	dificuldade(Esc).
dificuldade(1):-tabuleiro(T), random(1,3,J), if(J == 1, modoJ1(T, J, 1), modoJ2(T,J,1)), !.
dificuldade(2):-tabuleiro(T), random(1,3,J), if(J == 1, modoJ1(T, J, 2), modoJ2(T,J,1)), !.
dificuldade(3):-tabuleiro(T), random(1,3,J), if(J == 1, modoJ1(T, J, 3), modoJ2(T,J,1)), !.
dificuldade(_):-write('Até logo...'),nl.

%MODO CPU VS CPU
cpuVsCpu:-
	write('1 - Fácil'),nl,
	write('2 - Intermédio'), nl,
	write('3 - Difícil'),nl,
	write('4 - para sair'),nl,
	repeat,write('Opcao (Ex: 1) : '), get_code(Op),skip_line,Op>=49, Op=<53,conv(Op,Esc),
	dificuldade2(Esc).

dificuldade2(1):-tabuleiro(T), modoJ1(T, 1, 4), !.
dificuldade2(2):-tabuleiro(T), modoJ1(T, 1, 5), !.
dificuldade2(3):-tabuleiro(T), modoJ1(T, 1, 6), !.
dificuldade2(_):-write('Até logo...'),nl.

%FIM DA ESCOLHA DOS MODOS DE JOGO

%AVALIACAO DA JOGADA

interaccaoJogador(T,Y,X,Yf,Xf,Jogador):-
	desenha(T), nl,
	write('Jogador actual: '),write(Jogador),nl,
	write('Peca a mover:'),nl,
	write('Linha (Ex: 1) : '),read(Y),
	write('Coluna (Ex: a) : '),read(Xt),letra(Xt, X),
	write('Posicao desejada:'),nl,
	write('Linha Final (Ex: 2) : '),read(Yf),
	write('Coluna Final (Ex: b) : '),read(Xt2),letra(Xt2, Xf).

modoJogador(T, Jogador, Modo):-
	if((interaccaoJogador(T,Y,X,Yf,Xf,Jogador),
	    verificaCaminho(Jogador,X,Y,Xf,Yf,T),
	    modificaT(Jogador,X-Y-Xf-Yf,T,TNovo2)),
	    (troca(Jogador, Jogador2),
	    conquistaPecas(TNovo2, TNovo3, Jogador),
	    conquistaPecas(TNovo3, TNovo4, Jogador2),
	    (((terminouJogo(TNovo4,Jogador);terminouJogo(TNovo4,Jogador2)),menu) ; modoJ2(TNovo4,Jogador2,Modo))),
	   modoJogador(T,Jogador,Modo)).

% CICLO DO BOT DEPENDENDO DA DIFICULDADE ESCOLHIDA
modoCPU(T, Jogador, Modo):-
	(Modo == 1; Modo == 4),!,
	desenha(T), nl,
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T), L),
	choose(L, M),
	modificaT(Jogador,M,T, TNovo2),
	conquistaPecas(TNovo2, TNovo3, Jogador),
	conquistaPecas(TNovo3, TNovo4, Jogador2),
	troca(Jogador, Jogador2),
	(((terminouJogo(TNovo4,Jogador);terminouJogo(TNovo4,Jogador2)),menu); modoJ1(TNovo4, Jogador2, Modo)).

% BOT DO MODO INTERMEDIO
modoCPU(T, Jogador, Modo):-
	(Modo == 2; Modo == 5),!,
	desenha(T), nl,
	write('Estou a Pensar! Aguarde um momento por favor...  '),nl,
	greedy(T,L,1,X, Jogador),
	((L == X,
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T),L1),
	choose(L1,M1),escolheNaoSuicida(T,Jogador,L1,M1,M));choose(L,M)),
	modificaT(Jogador,M,T, TNovo2),
	conquistaPecas(TNovo2, TNovo3, Jogador),
	conquistaPecas(TNovo3, TNovo4, Jogador2),
	troca(Jogador, Jogador2),
	(((terminouJogo(TNovo4,Jogador);terminouJogo(TNovo4,Jogador2)),menu); modoJ1(TNovo4, Jogador2, Modo)).
% BOT DO MODO DIFICIL
modoCPU(T, Jogador, Modo):-
	(Modo == 3; Modo == 6),!,
	desenha(T), nl,
	write('Estou a Pensar! Aguarde um momento por favor...  '),nl,
	countPieces(Jogador,T,NPecas),
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T),L),
	shuffle(L,[Head|L2]),
	if(NPecas < 5,
	evaluate_and_choose(Jogador,L2,T,2,1,(Head,-1000),(M,_)),
   	evaluate_and_choose(Jogador,L2,T,0,1,(Head,-1000),(M,_))),
    modificaT(Jogador,M,T, TNovo2),
	conquistaPecas(TNovo2, TNovo3, Jogador),
	conquistaPecas(TNovo3, TNovo4, Jogador2),
	troca(Jogador, Jogador2),
	(((terminouJogo(TNovo4,Jogador);terminouJogo(TNovo4,Jogador2)),menu); modoJ1(TNovo4, Jogador2, Modo)).

% UTILITARIOS DO GREEDY

greedy(T,L, P, _, J):-
	findall(X-Y-Xf-Yf,(verificaCaminho(J,X,Y,Xf,Yf,T),modificaT(J,X-Y-Xf-Yf,T,TNovo2),conquistas(J,_,_,_,_,TNovo2,1,N),N>P),L1),
	\+ L1 = [],
	P1 is P+1,
	greedy(T,L,P1,L1,J), !.
greedy(_,L, _, L,_).

escolheNaoSuicida(Tab,J,_,X-Y-Xf-Yf,X-Y-Xf-Yf):-
	troca(J,J2),
	modificaT(J,X-Y-Xf-Yf,Tab,TNovo2),
	conquistas(J2,_,_,_,_,TNovo2,1,N),
	N=<1,!.
escolheNaoSuicida(Tab,J,L,M2,M):-escolheNaoSuicida2(Tab,J,L,M2,M).
escolheNaoSuicida2(Tab,J,[X-Y-Xf-Yf|Tail],_,M):-
	troca(J,J2),
	modificaT(J,X-Y-Xf-Yf,Tab,TNovo2),
	conquistas(J2,_,_,_,_,TNovo2,1,N),
	N>1,
	escolheNaoSuicida2(Tab,J,Tail,X-Y-Xf-Yf,M),!.
escolheNaoSuicida2(_,_,_,M,M).

modificaT(J,X-Y-Xf-Yf,T,TNovo):-
	muda_tab(J,0,X,Y,T,NovoTab),
	muda_tab(0,J,Xf,Yf,NovoTab,TNovo).

% UTILITARIOS DO MINIMAX

shuffle([], []).
shuffle(List, [Element|Rest]) :-
        choose(List, Element),
        delete(List, Element, NewList),
        shuffle(NewList, Rest).

evaluate_and_choose(J,[Move|Moves] ,Position,D ,MaxMin,Record,Best):-
	modificaT(J,Move,Position, Position1),
	minimax(J,D,Position1,MaxMin,_,Value),
	update(J,Move,Value,Record,Recordl),
	evaluate_and_choose(J,Moves,Position,D,MaxMin,Recordl,Best).


evaluate_and_choose(_,[],_,_ ,_ ,Record ,Record).

minimax(J, 0,Position,MaxMin,_,Value):-
	troca(J,J2),
	value(J, Position, V1),
	value(J2, Position, V2),
	conquistaPecas(Position, Position2, J),
	conquistaPecas(Position2, Position3, J2),
       	value(J, Position3,V3),
	value(J2, Position3, V4),
	if(V2 > V4, V5 is 1000, V5 is 0),
	if(V1 < V3, V6 is -1000, V6 is 0),
	V is V5+V6+V4*100+V3*(-100),
	Value is V*MaxMin.

minimax(J,D,Position,MaxMin,Move,Value):-
	D > 0,
	findall(X-Y-Xf-Yf, verificaCaminho(J, X,Y,Xf,Yf, Position),Moves),
	D1 is D-1,
	MinMax is -MaxMin,
       	evaluate_and_choose(J,Moves,Position,D1,MinMax,(nil,-1000),(Move,Value)).

update(_,_,Value,(Move1,Value1),(Move1,Value1)):-
	Value =< Value1.

update(_,Move,Value,(_,Value1),(Move,Value)):-
	Value > Value1.


value(Jog, Pos, Val):-
	countPieces(Jog, Pos, Val).

countPieces(Jog, Pos, Val):-
	countPiecesAux(Pos, 1,1,0,Jog,Val).

countPiecesAux(_,1,10,NPecas,_,NPecas):-!.

countPiecesAux(T,X,Y,NPecas,Jogador,Val) :-
       	if(verificaPeca(T, X, Y, Jogador), NPecasNovo is NPecas+1, NPecasNovo is NPecas),
	if(X == 9, (X1 is 1, Y1 is Y+1), (X1 is X+1, Y1 is Y)),
	countPiecesAux(T,X1,Y1,NPecasNovo, Jogador,Val).

% FIM DOS BOTS

% VERIFICA SE A PECA E' DO UTLIZADOR

verificaPeca(T,X,Y,Jogador) :- verificaPecaAux(T,X,Y,Jogador,1).

verificaPecaAux([T|_],X,Y,Jogador,Y) :-
	verificaPecaLinha(T,X,Jogador, 1).
verificaPecaAux([_|R],X,Y,Jogador,Linha) :-
	Linha2 is Linha+1,
	verificaPecaAux(R,X,Y,Jogador,Linha2).

verificaPecaLinha([Jogador|_], X, Jogador, X).
verificaPecaLinha([_|R], X, Jogador, Coluna) :-
	N1 is Coluna+1,
	verificaPecaLinha(R, X, Jogador, N1).

% VERIFICA SE HA UM CAMINHO ENTRE PECAS

adjacente(X1,Y1,X2,Y2):-X1 == X2, Y1 =:= Y2+1,!.
adjacente(X1,Y1,X2,Y2):-X1 == X2, Y1 =:= Y2-1,!.
adjacente(X1,Y1,X2,Y2):-Y1 == Y2, X1 =:= X2+1,!.
adjacente(X1,Y1,X2,Y2):-Y1 == Y2, X1 =:= X2-1.
adjacente2(X1,Y1,X2,Y2):-X1 == X2, Y1 =:= Y2+1,!.
adjacente2(X1,Y1,X2,Y2):-Y1 == Y2, X1 =:= X2+1.
adjacente3(X1,Y1,X2,Y2):-X1 == X2, Y1 =:= Y2-1,!.
adjacente3(X1,Y1,X2,Y2):-Y1 == Y2, X1 =:= X2-1.
diagonal(X1,Y1,X2,Y2):-X1==X2,  Y1\==Y2,!.
diagonal(X1,Y1,X2,Y2):-Y1==Y2,  X1\==X2.

verificaCaminho(Jog, X,Y,Xf,Yf, Tab):-
       	verificaPeca(Tab,X,Y,Jog),
	verificaPeca(Tab,Xf,Yf,0),
	adjacente(X,Y,Xf,Yf).
verificaCaminho(Jog, X,Y,Xf,Yf, Tab):-
	verificaPeca(Tab, X,Y, Jog),
	verificaPeca(Tab, Xf,Yf, 0),
      	verificaPeca(Tab, Xa, Ya, 0),
      	diagonal(X,Y,Xf,Yf),
	adjacente(Xa,Ya,X,Y),
      	muda_tab(0,Jog,Xa,Ya,Tab,NovoTab),
      	verificaCaminho(Jog, Xa, Ya, Xf, Yf, NovoTab).

% VERIFICA SE HA PECAS CONQUISTADAS

% PROCESSA TODAS AS CONQUISTAS
conquistaPecas(Tab, NovoTab, Jog):-
	findall(X-Y-Xf-Yf, conquistas(Jog, X,Y,Xf,Yf,Tab,1,_), L),
	processaRemocoes(L, Tab, NovoTab, Jog).

% CHAMA A FUNCAO PARA REMOVER AS PECAS PARA CADA CONQUISTA
processaRemocoes([], Tab, Tab,_).
% CONQUISTA NA VERTICAL
processaRemocoes([X-Y-X-Yf|T], Tab, NovoTab,Jog):-
	Y1 is Y+1,
	Y2 is Yf-1,
	retiraPecas(X,Y1,X,Y2, Tab, NovoTab2, Jog),
	processaRemocoes(T, NovoTab2, NovoTab, Jog).
% CONQUISTA NA HORIZONTAL
processaRemocoes([X-Y-Xf-Y|T], Tab, NovoTab,Jog):-
	X1 is X+1,
	X2 is Xf-1,
	retiraPecas(X1,Y,X2,Y, Tab, NovoTab2, Jog),
	processaRemocoes(T, NovoTab2, NovoTab, Jog).

% RETIRA AS PECAS DO INTERVALO PASSADO COMO ARGUMENTO
retiraPecas(X,Y,X,Y, Tab, TRes, Jog):-
	troca(Jog, Jog2),
	muda_tab(Jog2, 0, X, Y,Tab, TRes).
retiraPecas(X,Y1,X,Y2, Tab, NovoTab2, Jog):-
	troca(Jog, Jog2),
	muda_tab(Jog2, 0, X, Y1,Tab, NovoTab3),
	Y3 is Y1+1,
	retiraPecas(X,Y3,X,Y2,NovoTab3, NovoTab2, Jog).
retiraPecas(X1,Y,X2,Y, Tab, NovoTab2, Jog):-
	troca(Jog, Jog2),
	muda_tab(Jog2, 0, X1, Y,Tab, NovoTab3),
	X3 is X1+1,
	retiraPecas(X3,Y,X2,Y,NovoTab3, NovoTab2, Jog).

% RETORNA UMA CONQUISTA

conquistas(_, X,Y,Xf,Yf, _, N, N):-
	N > 1,
	adjacente3(X,Y,Xf,Yf).

conquistas(Jog, X,Y,Xf,Yf, Tab, N, Cont):-
	verificaPeca(Tab, X,Y, Jog),
	verificaPeca(Tab, Xf,Yf, Jog),
	troca(Jog, Jog2),
      	verificaPeca(Tab, Xa, Ya, Jog2),
      	diagonal(X,Y,Xf,Yf),
	adjacente2(Xa,Ya,X,Y),
      	muda_tab(Jog2,Jog,Xa,Ya,Tab,NovoTab),
	N1 is N+1,
      	conquistas(Jog, Xa, Ya, Xf, Yf, NovoTab, N1,Cont).

% ALTERA A POSICAO DA PECA DO JOGADOR

muda_tab(Peca,Pnov,X,Y,Tab,NovoTab):-
	muda_tab2(1,Peca,Pnov,X,Y,Tab,NovoTab).

muda_tab2(_,_,_,_,_,[],[]).
muda_tab2(Y,Peca,Pnov,X,Y,[Lin|Resto],[NovLin|Resto2]):-
	muda_linha(1,Peca,Pnov,X,Lin,NovLin),
	N2 is Y+1,
	muda_tab2(N2,Peca,Pnov,X,Y,Resto,Resto2).
muda_tab2(N,Peca,Pnov,X,Y,[Lin|Resto],[Lin|Resto2]):-
	N\=Y, N2 is N+1,
	muda_tab2(N2,Peca,Pnov,X,Y,Resto,Resto2).

muda_linha(_,_,_,_,[],[]).
muda_linha(X,Peca,Pnov,X,[Peca|Resto],[Pnov|Resto2]):-
	N2 is X+1,
	muda_linha(N2,Peca,Pnov,X,Resto,Resto2).

muda_linha(N,Peca,Pnov,X,[El|Resto],[El|Resto2]):-
	N\=X, N2 is N+1,
	muda_linha(N2,Peca,Pnov,X,Resto,Resto2).

%VERIFICA SE O JOGO TERMINOU

terminouJogo(T,Jogador) :- terminouJogoaux(T,1,1,0,Jogador).
terminouJogoaux(T,1,10,NPecas,Jogador):-!, NPecas < 3,desenha(T),nl,nl,nl,
	write('Terminou o jogo. O jogador '),
	troca(Jogador, Jogador2), write(Jogador2), write(' venceu.'),nl,nl,nl.

terminouJogoaux(T,X,Y,NPecas,Jogador) :-
       	if(verificaPeca(T, X, Y, Jogador), NPecasNovo is NPecas+1, NPecasNovo is NPecas),
	if(X == 9, (X1 is 1, Y1 is Y+1), (X1 is X+1, Y1 is Y)),
	terminouJogoaux(T,X1,Y1,NPecasNovo, Jogador).

%DESCRICAO DO JOGO
descricaoJogo:-
	write('Como jogar:'),nl,
	write('O objectivo do Hashami Shogi é simples: conseguir retirar as peças do adversário prensando-as com as nossas. Para isso, podemos mover as peças para a frente, trás, esquerda, e direita, sem restrições de espaço. O processo de captura passa por rodear a peça inimiga com duas nossas na horizontal ou vertical (a diagonal não é considerada). Perde aquele que ficar primeiro com apenas duas peças no tabuleiro.').


	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).

port(60070).

% launch me in sockets mode
server:-
	port(Port),
	socket_server_open(Port, Socket),
	socket_server_accept(Socket, _Client, Stream, [type(text)]),
	write('Accepted connection'), nl,
	serverLoop(Stream),
	socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
	repeat,
	read(Stream, ClientMsg),
	write('Received: '), write(ClientMsg), nl,
	parse_input(ClientMsg, MyReply),
	format(Stream, '~q.~n', [MyReply]),
	write('Wrote: '), write(MyReply), nl,
	flush_output(Stream),
	(ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(comando(Arg1, Arg2), Answer) :-
	comando(Arg1, Arg2, Answer).
	
parse_input(verificaCaminho(Jogador,X,Y,Xf,Yf,T), 1) :-
	verificaCaminho(Jogador,X,Y,Xf,Yf,T).

parse_input(verificaCaminho(Jogador,X,Y,Xf,Yf,T), 0) :-
	\+ verificaCaminho(Jogador,X,Y,Xf,Yf,T).	

parse_input(jogadasPossiveis(Jog,X,Y,T), Res) :-
	findall(Xf-Yf, verificaCaminho(Jog, X,Y,Xf,Yf, T), Res).	

parse_input(conquistas(T), Res):-
	conquistaPecas(T, TNovo, 1),
	conquistaPecas(TNovo, Res, 2).

parse_input(terminouJogo(T), 1):-
	terminouJogoaux(T,1,1,0,1), !.
	
parse_input(terminouJogo(T), 2):-	
	terminouJogoaux(T,1,1,0,2), !.

parse_input(terminouJogo(_), 0).

parse_input(jogada(J,X,Y,Xf,Yf,T) , Res):-
	modificaT(J,X-Y-Xf-Yf,T,Res).

parse_input(modoIntermedio(T, Jogador), Res):-
	greedy(T,L,1,X, Jogador),
	((L == X,
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T),L1),
	choose(L1,M1),escolheNaoSuicida(T,Jogador,L1,M1,M));choose(L,M)),
	modificaT(Jogador,M,T, Res), !.

parse_input(modoFacil(T, Jogador), Res):-
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T), L),
	choose(L, M), modificaT(Jogador,M,T, Res), !.
	
parse_input(modoDificil(T, Jogador), Res):-
	countPieces(Jogador,T,NPecas),
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T),L),
	shuffle(L,[Head|L2]),
	if(NPecas < 5,
	evaluate_and_choose(Jogador,L2,T,2,1,(Head,-1000),(M,_)),
   	evaluate_and_choose(Jogador,L2,T,0,1,(Head,-1000),(M,_))),
    modificaT(Jogador,M,T, Res), !.
	
parse_input(quit, ok-bye) :- !.

modoIntermedio(T, Jogador, Res):-
	greedy(T,L,1,X, Jogador),
	((L == X,
	findall(X-Y-Xf-Yf, verificaCaminho(Jogador, X,Y,Xf,Yf, T),L1),
	choose(L1,M1),escolheNaoSuicida(T,Jogador,L1,M1,M));choose(L,M)),
	modificaT(Jogador,M,T, Res), write('ola'), nl.
		
comando(Arg1, Arg2, Answer) :-
	write(Arg1), nl, write(Arg2), nl,
	Answer = 5.	