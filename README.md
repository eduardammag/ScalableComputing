# Tarefa 04: Análise de Ocorrências de "Love" e "Hate" em Shakespeare

## Sobre o Exercício
Este projeto tem como objetivo analisar a ocorrência das palavras **"love"** e **"hate"** nas obras de William Shakespeare. Para isso, será desenvolvido um programa que contará o número de vezes que cada termo aparece, utilizando **paralelismo com threads** para otimizar o processamento.

## Objetivos
- Contar o número de ocorrências das palavras **"love"** e **"hate"** em um arquivo contendo uma coletânea de livros de Shakespeare.
- Implementar uma solução que utilize **threads** para paralelizar a busca pelos termos.
- Permitir a configuração do número de threads através de uma variável global.
- Medir e relatar diferentes tempos de execução e estatísticas do programa.

## Entradas
- Um arquivo de texto contendo a coletânea de livros de William Shakespeare.
- Um parâmetro global para definir o número de **threads** utilizadas na pesquisa.

## Saídas
Ao final da execução, o programa deve exibir:
- O **número de threads** utilizadas.
- O **tamanho do bloco** processado por cada thread.
- O **tempo de preparação**, que inclui o carregamento dos dados na memória e a separação em blocos.
- O **tempo de execução** da pesquisa.
- O **tempo total** gasto no processo.
- O **número de ocorrências** de cada palavra pesquisada.
- Qual palavra teve um **número maior de ocorrências**.

## Experimento
Para avaliar a eficiência da implementação, siga os seguintes passos:
1. Inicie a execução do programa com **uma** thread.
2. Execute o programa repetidamente, aumentando o número de threads **até 20**.
3. Gere um **gráfico** mostrando a relação entre **tempo de execução** e **número de threads**:
   - Um gráfico considerando o **tempo total** de execução.
   - Um gráfico considerando apenas o **tempo da pesquisa**.
4. Discuta os resultados obtidos com os demais integrantes do grupo.
