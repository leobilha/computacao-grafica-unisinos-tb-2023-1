![](https://upload.wikimedia.org/wikipedia/pt/9/91/Lp_logo_unisinos.png)

# Trabalho do Grau B - Computação Gráfica - 2023/1

![](https://github.com/leobilha/computacao-grafica-unisinos-tb-2023-1/blob/main/GifGB.gif)

### Passo para clonar, compilar e executar o programa: 

> Execute o seguinte comando no cmd: 
> 
> git clone https://github.com/leobilha/computacao-grafica-unisinos-tb-2023-1
> 

>
>As dependências estão pré-compiladas para Windows 10 32-bits e compilador do Visual Studio 2019/2022
>Se o seu ambiente de desenvolvimento for diferente, você deve baixar:
>
> GLFW: https://www.glfw.org/
> GLAD: https://glad.dav1d.de/ - para mais instruções, consulte: https://learnopengl.com/Getting-started/Creating-a-window
>
> Ao abrir o projeto, pode ser que seja necessário mudar a arquitetura para x86 (caso queira usar as dependências que vieram com o projeto):
>
>![image](https://user-images.githubusercontent.com/2465857/128773364-4589fffe-57f9-4de2-acdf-7898abbaad8b.png)
>
>Se você quiser ou precisar alterar as dependências, você deve ir em Projeto -> Propriedades e alterar estes 3 lugares:
>
>1) Diretório com os arquivos de cabeçalho (onde ficam os .h da biblioteca): 
>![image](https://user-images.githubusercontent.com/2465857/128772404-13b3a1a4-fc71-4a93-9de1-1eb9cba05969.png)
>
>2) Diretório com os arquivos de biblioteca estática pré-compilados (.lib ou .a):
>![image](https://user-images.githubusercontent.com/2465857/128772735-d04bec8e-edcd-485f-a103-e66f1810cdad.png)
>
>3) Incluir os nomes das bibliotecas estáticas:
>![image](https://user-images.githubusercontent.com/2465857/128772961-04745bc6-ef1d-45b3-9aa7-a623d2af3065.png)


### Tecnologias utilizadas
* C++
* OpenGL
* GLFW
* GLEW

### Autores/Alunos 

* @[Carlos-Vanoni](https://github.com/Carlos-Vanoni) - Carlos Vanoni
* @[leobilha](https://github.com/leobilha) - Leonardo Terragno

### Comandos de uso

>tecla 1	Seleciona o primeiro objeto
>
>tecla 2	Seleciona o segundo objeto
>
>tecla W	translata o objeto +N unidades no eixo Y
>
>tecla S	translata o objeto -N unidades no eixo Y
>
>tecla A	translata o objeto -N unidades no eixo X
>
>tecla D	translata o objeto +N unidades no eixo X
>
>tecla E	rotaciona N graus para direita em relação ao eixo Y
>
>tecla Q	rotaciona N graus para a esquerda em relação ao eixo Y
>
>tecla Z	rotaciona N graus para direita em relação ao eixo X
>
>tecla C	rotaciona N graus para a esquerda em relação ao eixo X
>
>tecla Tab	rotaciona N graus para direita em relação ao eixo Z
>
>tecla Capslock rotaciona N graus para direita em relação ao eixo Z
>tecla →	move câmera para direita
>
>tecla ←	move câmera para esquerda
>
>tecla ↑	move câmera para frente
>
>tecla ↓	move câmera para trás
