#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: altera_taxa_portagem.sh
## Descrição/Explicação do Módulo: Lê o ficheiro 'portagens.txt' e altera a 
## taxa do Lanço. Caso o Lanço não exista, é adicionada uma nova linha ao 
## ficheiro. Caso o ficheiro 'portagens.txt' não exista, o ficheiro é criado 
## e uma linha com a taxa do Lanço é adicionada.
##
## @param $1: <Lanço: string>
## @param $2: <Auto-Estrada: string>
## @param $3: <Novo_Valor_Taxa: número inteiro positivo>
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
  # O lanço da portagem existe?
  x=$(cat ${fp} | cut -d':' -f2 | grep -w ${1})

  if ! [ "$x" == "" ]; then
    # O lanço da portagem existe => atualiza a taxa de utilização
   v=$(awk -F[:] -v lanco=${1} -v tx=${3} '{if($2==lanco) {sub($4,tx)}}{print;}' ${fp})
   echo $v | sort -t ' ' -k3 -k2 | tr " " "\n" > ${fp}
  else
    # O lanço da portagem não existe => adiciona-o com id = max{id} + 1
   awk -F[:] -v lanco=${1} -v ae=${2} -v tx=${3} 'BEGIN{max=0}{if($1>max) max=$1} END {print max+1 ":" lanco ":" ae ":" tx}' ${fp} >> ${fp}
  fi
  
  # SUCESSO: Informa que a taxa de utilização do Lanço foi atualizada
  ./success 3 ${1}
  # SUCESSO: Ordena os resultados por nome da Autoestrada, seguido do nome do Lanço
  ./success 4 ${fp}
  
else
  # Cria um ficheiro novo com o lanço da portagem
  echo '1:'${1}':'${2}':'${3} > ${fp}
  # SUCESSO: Informa que a taxa de utilização do Lanço foi atualizada
  ./success 3 ${1}
  # SUCESSO: Apresenta os resultados
  ./success 4 ${fp}
fi
