# Simplexity Game (g2)

### Autor

 __Tiago Oliveira__ - a21404702

O jogo foi desenvolvido em repositório local, disponível agora [neste link](https://github.com/tcotiago/simplexity-game-g2)


## Descrição

### Arquitectura da Solução

O Tabuleiro de jogo é um Array 2D de caracteres, o tamanho deste Array é dependente dos valores definidos no header _Constants_, para coluna e linhas máximas. O jogo continua enquanto enquanto não for devolvido um booleano verdadeiro numa função 

O jogo é renderizado de duas maneiras: 

__Ascii__: Imprime as peças como 0 e X no terminal.

__G2__: Abre uma janela X11 de dimensão variável, de acordo com os Defines do tamanho das peças e espaçamento entre elas, imprime as peças como círculo e quadrado.

Ambos renders são actualizados em cada turno.

Para a __verificação de vitória__, executada em cada turno, em vez de percorrer o tabuleiro inteiro, analisa-se a vizinhança da última peça jogada.
Existem 4 verificações que são feitas, vertical, horizontal e diagonais (diagonal baixo para cima e vice-versa).

No caso de se encontrar uma vitória, é retornado o tipo (símbolo ou cor), é mostrado o jogador vencedor e acaba o jogo, a forma sobrepõe-se à cor pois é analisada primeiro a condição símbolo.

As variáveis modificáveis encontram-se no header _constants.h_ e são passíveis de ser editadas pelo jogador, a fim de alterar por exemplo quantas peças são necessárias (em linha) para ganhar.

O código-fonte está comentado e documentado na pasta __html__ (Doxygen).



### Fluxograma de Funcionamento Geral

<p align="center">
  <img src="https://i.imgur.com/qY4SgTB.png" alt="Fluxograma"/>
</p>

### Nota:
##### Escolha válida entende-se que:
 a) o jogador ainda tem peças do tipo indicado.
 
 b) a coluna do tabuleiro ainda tem espaço para mais peças (max 7).



## References

* <a name="ref1">[1]</a> Whitaker, R. B. (2012). [The C Header Guidelines](http://umich.edu/~eecs381/handouts/CHeaderFileGuidelines.pdf),
   EECS Department, University of Michigan.
* <a name="ref1">[2]</a> Charlesworth, O. et al. (2011). [Creating your own header file in C](http://umich.edu/~eecs381/handouts/CHeaderFileGuidelines.pdf),
   StackOverflow.com.
* <a name="ref1">[3]</a> Loison, F. [Bash tips: Colors and formatting](https://misc.flogisoft.com/bash/tip_colors_and_formatting), Flogisoft.com.
* <a name="ref1">[4]</a> Milanovic, L. et al. (2006). [g2 Documentation](http://www.ncbr.muni.cz/~martinp/g2/index.html),
   EECS Department, University of Michigan.
* <a name="ref1">[5]</a> Brain Bender Games (2009). [Simplexity Game Rules](https://john.cs.olemiss.edu/~dwilkins/CSCI531/fall12/Simplexity_rules.pdf),
   Discovery Bay Games.
* <a name="ref1">[6]</a> Cronin, K. et al. (2009). [Organization of C Files](https://stackoverflow.com/questions/47919/organization-of-c-files), StackOverflow.com.
