# Visualizador de Objetos 3D e Pipeline Gráfico com Qt

Este projeto consiste em uma aplicação interativa desenvolvida em C++ com o framework Qt, projetada para simular um pipeline gráfico simplificado em Computação Gráfica. A aplicação permite carregar arquivos de modelos 3D no formato padrão `.obj`, visualizá-los em uma projeção perspectiva e realizar transformações geométricas em tempo real.

## Funcionalidades Principais

* **Leitor de Arquivos OBJ:** Parser customizado implementado via software para ler vértices (`v`) e faces (`f`) de arquivos geométricos 3D.
* **Transformações Geométricas 3D:** Suporte completo a operações de translação, rotação (com escolha de eixos X, Y ou Z) e escala com base em matrizes de coordenadas homogêneas ($4 \times 4$).
* **Manipulação de Câmera (Panning e Zoom):** Navegação interativa na cena por meio do mouse (arrastar para mover a câmera e scroll do mouse para zoom dinâmico).
* **Algoritmo de Clipping de Linhas:** Implementação do clássico algoritmo de clipping de **Cohen-Sutherland** para recortar linhas que extrapolam os limites da Viewport/Window de visualização.
* **Display File:** Gerenciamento eficiente e centralizado da lista de objetos carregados na cena para renderização em tempo de execução.

## Detalhes Técnicos

* **Estrutura Bidimensional e Tridimensional:** Classes customizadas (`Vetor3D`, `Ponto2D`) criadas do zero para manipulação matemática de vetores, produtos escalares e vetoriais sem dependência de bibliotecas externas pesadas.
* **Matrizes Homogêneas:** Implementação manual da multiplicação de matrizes para aplicar transformações compostas com pontos pivô específicos.
* **Clipping por Region Codes:** O algoritmo de clipping utiliza atribuição de códigos binários de região (TOPO, BASE, DIREITA, ESQUERDA) para determinar a aceitação ou descarte rápido de retas.

## Tecnologias Utilizadas

* **Linguagem:** C++
* **Framework Interface/Gráfico:** Qt 6.11 (utilizando `QPainter` para rasterização das linhas no `QFrame`)
* **IDE Recomendada:** Qt Creator
