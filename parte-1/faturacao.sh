#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: faturacao.sh
## Descrição/Explicação do Módulo: A partir dos ficheiros 'pessoas.txt' e 
## 'relatorio_utilizacao.txt', cria o ficheiro 'faturas.txt' com as faturas 
## agregadas por cliente usando o seguinte formato:
## Cliente: <Nome do condutor>
## <ID Portagem>:<Lanço>:<ID Condutor>:<Matrícula>:<Taxa_Portagem>:<Data>
## Total: <Soma Taxa_Portagem> créditos
##
###############################################################################

fr='relatorio_utilizacao.txt'
fp='pessoas.txt'
ff='faturas.txt'

if [ -f ${fr} ]; then
  # Apaga o ficheiro 'faturas.txt' se existir
  rm -f ${ff}
  
  # Para cada cliente do ficheiro 'pessoas.txt',
  # agrega suas faturas presentes no ficheiro 'faturas.txt'
  while read p; do
    echo $p | awk -F[:] '{print "Cliente: " $2}' >> ${ff}
    id=$(echo $p | cut -d':' -f 3)
    # Nota: Usa o match (~) porque há IDs com espaços
    awk -F[:] -v id=${id} 'BEGIN{sum=0}{if($3 ~ id){sum=sum+$5; print;}}END{print "Total: " sum " créditos\n"}' ${fr} >> ${ff}
  done < ${fp}
  
  # SUCESSO: Apresenta a listagem de todas as faturas
  ./success 5 ${ff}
  
else
  # ERRO: O ficheiro 'relatorio_utilizacao.txt' não existe
  ./error 1 ${fr}
  
fi
