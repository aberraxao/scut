#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: lista_condutores.sh
## Descrição/Explicação do Módulo: A partir do ficheiro 'pessoas.txt' cria o 
## ficheiro 'condutores.txt' com um formato diferente.
## - pessoas.txt: <ID carta condução>:<Nome>:<Nr Contribuinte>:<Contacto>
## - condutores.txt: <ID>-<Nome>;<ID carta condução>;<Contacto>;
##                   <Nr Contribuinte>;<Saldo (em créditos)>
##
###############################################################################

fp="pessoas.txt"
fc="condutores.txt"

if [ -f ${fp} ]; then
  # SUCESSO: A partir do ficheiro 'pessoas.txt' cria o ficheiro 'condutores.txt'
  awk -F[:] '{print "ID" $3 "-" $2 ";" $1 ";" $4 ";" $3 ";150"}' ${fp} > ${fc}
  ./success 2 ${fc}
else
  # ERRO: O ficheiro 'pessoas.txt' não existe
  ./error 1 ${fp}
fi
