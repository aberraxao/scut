#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: 
## TODO
##
###############################################################################

fp='portagens.txt'
fr='relatorio_utilizacao.txt'
fc='condutores.txt'
nb='^[1-9][0-9]*$'


if [ $# -lt 1 ]; then
  # ERRO: Tem que ter pelo menos um argumento
  ./error 2

else
  case ${1} in
  
  'listar')
    if [[ $# -ne 1 ]]; then
      # ERRO: O Quando o primeiro argumento é 'listar', tem que ter 1 argumento
      ./error 2
    elif [ -f ${fp} ]; then
      # SUCESSO: Mostra o nome de todas as autoestradas presentes no ficheiro
      # 'portagens.txt' sem repetições
      cat ${fp} | cut -d':' -f 3 | sort | uniq | ./success 6
    else
      # ERRO: O ficheiro 'portagens.txt' não existe
      ./error 1 ${fp}
    fi
    ;;

  'registos')
    if [[ $# -ne 2 ]]; then
      # ERRO: O Quando o primeiro argumento é 'registos', tem que ter 2 argumentos
      ./error 2
    elif ! [[ ${2} =~ $nb ]]; then
      # ERRO: O número de registos não é um inteiro maior que zero
      ./error 3 ${2}
    elif [[ ${2} =~ $nb ]]; then
      if [ -f ${fr} ]; then
        # SUCESSO: Mostra todos os lanços com um número de utilizações maior ou igual a '${2}'
        var=$(cat ${fr} | cut -d':' -f2 | sort | uniq -c | awk -F' ' -v c=${2} '{if($1 >= c){print $2;}}')
        if [[ ${var} ]]; then
          echo ${var} | ./success 6
        else
          # ERROR: Não há lanços com pelo menos '${2}' utilizações
          ./error 2
        fi
      else
        # ERRO: O ficheiro 'relatorio_utilizacao.txt' não existe
        ./error 1 ${fr}
      fi
    fi
    ;;
    
  'condutores')
    if [[ $# -ne 1 ]]; then
      # ERRO: O Quando o primeiro argumento é 'condutores', tem que ter 1 argumento
      ./error 2
    elif [[ -f ${fr} && -f ${fc} ]]; then
      # SUCESSO: Mostra o nome de todos os condutores presentes no ficheiro
      # 'relatorio_utilizacao.txt' sem repetições
      condutores=$(cat ${fr} | cut -d':' -f3 | sort | uniq | awk -F' ' '{print $1;}')
      cat ${fc} | grep "$condutores" | cut -d';' -f1 | cut -d'-' -f2 | ./success 6
    elif ! [ -f ${fr} ]; then
      # ERRO: O ficheiro 'relatorio_utilizacao.txt' não existe
      ./error 1 ${fr}
    elif ! [ -f ${fc} ]; then
      # ERRO: O ficheiro 'condutores.txt' não existe
      ./error 1 ${fc}
    fi
    ;;

  *)
    # ERRO: O argumento 1 recebido não está coberto pelo programa
    ./error 3 ${1}
    ;;

  esac
  
fi
