#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: faturacao.sh
## Descrição/Explicação do Módulo: 
## TODO
##
###############################################################################

fr='relatorio_utilizacao.txt'
ff='faturas.txt'
fp='pessoas.txt'

if [ -f ${fr} ]; then
  # Apaga o ficheiro 'faturas.txt' se existir
  rm -f ${ff}
  
  # Para cada cliente do ficheiro 'pessoas.txt',
  # agrega suas faturas presentes no ficheiro 'faturas.txt'
  while read p; do
    echo $p | awk -F[:] '{print "Cliente: " $2}' >> ${ff}
    id=$(echo $p | cut -d':' -f 3)
    # Used match (~) to cover Ids with spaces
    awk -F[:] -v id=${id} 'BEGIN{sum=0}{if($3 ~ id){sum=sum+$5; print;}}END{print "Total: " sum " créditos\n"}' ${fr} >> ${ff}
  done < ${fp}
  
  ./success 5 ${ff}
  
else
  # O ficheiro 'relatorio_utilizacao.txt' não existe
  ./error 1 ${fr}
  
fi
