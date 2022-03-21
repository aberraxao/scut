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

fi='relatorio_utilizacao.txt'
fa='faturas.txt'
fp='pessoas.txt'

if [ -f ${fi} ]; then
  # Apaga o ficheiro 'faturas.txt' se existir
  rm -f ${fa}
  while read p; do
    echo $p | awk -F[:] '{print "Cliente: " $2}' >> ${fa}
    id=$(echo $p | cut -d':' -f 3)
    # TODO Match (~): https://www.tutorialspoint.com/awk/awk_regular_expression_operators.htm
    awk -F[:] -v i=${id} 'BEGIN{sum=0}{if($3 ~ i){sum=sum+$5; print;}}END{print "Total: " sum " créditos\n"}' ${fi} >> ${fa}
    # TODO: check if I can have a space at the end
  done < ${fp}
  
  ./success 5 ${fa}
  
else
  # O ficheiro 'relatorio_utilizacao.txt' não existe
  echo 'here'
  ./error 1 ${fi}
fi

