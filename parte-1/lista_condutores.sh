#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: lista_condutores.sh
## Descrição/Explicação do Módulo: 
## TODO
##
###############################################################################

fp="pessoas.txt"
fc="condutores.txt"
if [ -f ${fp} ]; then
  # pessoas.txt format: <ID carta condução>:<Nome>:<Nr Contribuinte>:<Contacto>
  # condutores.txt format: <ID>-<Nome>;<ID carta condução>;<Contacto>;<Nr Contribuinte>;<Saldo (em créditos)>
  awk -F[:] '{print "ID" $3 "-" $2 ";" $1 ";" $4 ";" $3 ";150"}' ${fp} > ${fc}
  ./success 2 ${fc}
else
  ./error 1 ${fp}
fi
