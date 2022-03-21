#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: 
##
##
###############################################################################


fr='relatorio_utilizacao.txt'
nb='^[1-9]+$'

if [ $# -lt 1 ]; then
  # ERRO: Tem que ter pelo menos um argumentos
  ./error 2
elif [[ ${1} == 'registos' && $# -lt 2 ]]; then
  # ERRO: O Quando o primeiro argumento é 'registos', tem que ter pelo menos 2 argumentos
  ./error 2
elif [[ ${1} == 'registos' && ! ${2} =~ $nb ]]; then
  # ERRO: O número de registos não é um inteiro maior que zero
  ./error 3 ${2}
  
elif [[ ${1} == 'registos' && ${2} =~ $nb ]]; then
  if [ -f ${fr} ]; then  
    var=$(awk -F[:] -v id=${2} 'BEGIN{count=0}{if($1 == id){count=count+1}}END{print count;}' ${fr})
    
    if [ ${var} == 0 ]; then
      ./error 2
    else
      awk -F[:] -v id=${2} '{if($1 >= id){print $2;}}' ${fr}
    fi
  
  else
    ./error 1 ${fr}
  
  fi
elif [ -f ${fi} ]; then
  echo 'here'
  # TODO sort by ae e depois lanço
  # <ID_portagem>:<Lanço>:<Auto-estrada atribuída>:<Taxa de utilização (em créditos)>
  #  awk -F[:] -v lanco=${1} -v ae=${2} -v tx=${3} '{x=sub(lanco, tx, $4)} END {if(x!=1) BEGIN {max=0}{if($1>max) max=$1}} END {print max+1 ":" lanco ":" ae ":" tx}' ${fi} >> ${fi}
  #./success 4 ${fi}

fi

