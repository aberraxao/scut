#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
## Nome do Módulo: menu.sh
## Descrição/Explicação do Módulo: 
## TODO
##
###############################################################################

# Apresenta o menu até ser selecionada a opção '0'
until [ "${m}" = "0" ]; do

  # Lista as opções disponíveis
  echo '1. Listar condutores'
  echo '2. Altera taxa de portagem'
  echo '3. Stats'
  echo '4. Faturação'
  echo '0. Sair'
  
  # Pede ao utilizador para escolher uma opção
  read -p $'\nOpção: ' m

  case ${m} in
  
    1) # Listar condutores
      echo $'\nListar condutores\n'
      ./lista_condutores.sh
      read ;;
      
    2) # Altera taxa de portagem
      echo $'\nAltera taxa de portagem\n'
      
      # Pede ao utilizador para preencher os inputs
      read -p 'Lanço           : ' l
      read -p 'Auto-estrada    : ' ae
      read -p 'Novo valor taxa : ' tx
      
      ./altera_taxa_portagem.sh ${l} ${ae} ${tx}
      read ;;
      
    3) # Stats
      echo $'\nStats\n'
   
      until [ "${sm}" = "0" ]; do

        # Lista as opções disponíveis
        echo '1. Nome de todas as Autoestradas'
        echo '2. Registos de utilização'
        echo '3. Listagem condutores'
        echo '0. Voltar'
  
        # Pede ao utilizador para escolher uma opção
        read -p $'\nOpção: ' sm

        case ${sm} in
      
          1) # Nome de todas as Autoestradas
            ./stats.sh 'listar' ;;
       
          2) # Registos de utilização
            read -p 'Mínimo de registos : ' r
            ./stats.sh 'registos' ${r} ;;
        
          3) # Listagem condutores
            ./stats.sh 'condutores' ;;
        
          0) # Voltar ao menu anterior
            break ;;
       
          *) # ERRO: O argumento recebido é inválido
            ./error 3 ${sm} ;;
        
        esac
        
        read # Espera até uma tecla ser pressionada
        break # Volta automaticamente ao menu anterior
        
      done ;;
      
    4) # Faturação
      echo $'\nFaturação\n'
      ./faturacao.sh
      read ;;
     
    0) # Voltar
      exit ;;
      
    *) # ERRO: O argumento recebido é inválido
      ./error 3 ${m}
      read ;;

  esac
  
done
