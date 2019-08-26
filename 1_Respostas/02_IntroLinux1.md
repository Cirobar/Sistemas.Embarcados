1. Por que o Linux recebeu esse nome?

> Foi criado por Linus Torvalds e representa a fusão de "Linus" com o nome do sistema operacional "UNIX", formando Linux.

2. O que são daemons?

> São programas que executam em segundo plano sem controle de um usuário interativo. Breve, são programas executados para garantir o funcionamento do sistema operacional.

3. O que é o shell?

> É uma interface que permite o acesso às ferramentas do sistema operacional. No Linux é conhecido como bash ou terminal.

4. Por que é importante evitar executar o terminal como super-usuário?

> O acesso como super-usuário garante acesso a ferramentas restritas e em caso de mau uso, o usuário pode causar alterações não desejadas no sistema operacional. Trata-se, finalmente, de uma boa prática de uso do sistema também.

5. Qual botão do teclado completa o que o usuário escreve no terminal, de acordo com o contexto?

> TAB.


6. Quais botões do teclado apresentam instruções escritas anteriormente?

> Tecla UP (seta para cima). "ctrl+r" ou "ctrl+s" permite pesquisar comandos anteriores.

7. Apresente os respectivos comandos no terminal para:
  (a) Obter mais informações sobre um comando.
    ```bash
    $ man command
    ```
    ou
    ```bash
    $ command --help
    ```
  (b) Apresentar uma lista com os arquivos dentro de uma pasta.
    ```bash
    $  ls -l
    ```
  (c) Apresentar o caminho completo da pasta.
    ```bash
    $ pwd
    ```
  (d) Trocar de pasta.
  ```bash
    $ cd [path]
    ```
  (e) Criar uma pasta.
  ```bash
    $ mkdir [folder name]
    ```
  (f) Apagar arquivos definitivamente.
  ```bash
    $ rm [file name]
    ```
  (g) Apagar pastas definitivamente.
  ```bash
    $ rmdir [folder name]
    ```
  (h) Copiar arquivos.
  ```bash
    $ cp [file name][new file name]
    ```
  (i) Copiar pastas.
  ```bash
    $ cp -r [folder name][new folder name]
    ```
  (j) Mover arquivos.
  ```bash
    $ mv [file name][path]
    ```
  (k) Mover pastas.
  ```bash
    $ mv [original path][new path]
    ```
  (l) Renomear pastas.
  ```bash
    $ mv [old name][new name]
    ```
  (m) Apresentar o conteúdo de um arquivo.
  ```bash
    $ cat [file name]
    ```
  (n) Apresentar o tipo de um arquivo.
  ```bash
    $  ls -l [filme]
    ```
  (o) Limpar a tela do terminal.
  ```bash
    $ clear
    ```
  (p) Encontrar ocorrências de palavras-chave em um arquivo-texto.
  ```bash
    $ grep -c [keyword][file name]
    ```
  (q) Ordenar informações em um arquivo-texto.
  ```bash
    $ sort [filename.txt]
    ```
  (r) Substituir ocorrências de palavras-chave em um arquivo-texto.
  ```bash
    $ sed -e [keyword][file name]
    ```
  (s) Conferir se dois arquivos são iguais.
  ```bash
    $ diff [file name 1][file name 2]
    ```
  (t) Escrever algo na tela.
  ```bash
    $ echo [text]
    ```


