#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: altera_taxa_portagem.sh
## Descrição/Explicação do Módulo: 
## TODO
##
###############################################################################


fp='portagens.txt'
nb='^[1-9][0-9]*$'
str='^[a-Z0-9-]+$'

if [ $# -lt 3 ]; then
  # ERRO: Menos de 3 argumentos
  ./error 2
elif ! [[ ${1} =~ $str ]]; then
  # ERRO: O lanço não é uma string alfanumérica
  ./error 3 ${1}
elif ! [[ ${2} =~ $str ]]; then
  # ERRO: A auto-estrada não é uma string alfanumérica
  ./error 3 ${2}
elif ! [[ ${3} =~ $nb ]]; then
  # ERRO: A taxa de utilização não é um número inteiro positivo
  ./error 3 ${3}
elif [ -f ${fp} ]; then
  # Verifica se o lanço da portagem existe
  x=$(cat ${fp} | cut -d':' -f2 | grep -w ${1})

  if ! [ "$x" == "" ]; then
    # O lanço da portagem existe, atualiza a taxa de utilização
  #  awk -F[:] -v lanco=${1} -v tx=${3} '{if ($2 == lanco) $4=tx;print;}' ${fp}
    echo 'here1'
  ./success 3 ${1}
  else
    # O lanço da portagem não existe, adiciona-o com id = max{id} + 1
   awk -F[:] -v lanco=${1} -v ae=${2} -v tx=${3} 'BEGIN{max=0}{if($1>max) max=$1} END {print max+1 ":" lanco ":" ae ":" tx}' ${fp} >> ${fp}
  fi
  
  # Ordena os resultados por nome da Autoestrada, seguido do nome do Lanço
  sort -t ':' -k3 -k2 ${fp} | ./success 4
  
else
  # Cria um ficheiro novo com o lanço da portagem
  echo '1:'${1}':'${2}':'${3} > ${fp}
  ./success 4 ${fp}
fi

