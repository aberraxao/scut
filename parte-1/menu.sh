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

until [ '${m}' = '0' ]; do

  # Lista as opções disponíveis
  echo '1. Listar condutores'
  echo '2. Altera taxa de portagem'
  echo '3. Stats'
  echo '4. Faturação'
  echo '0. Sair'
  
  # Pede ao utilizador para escolher uma opção
  echo ''
  read -p 'Opção: ' m
  echo ''

  case ${m} in
  
    1)
      echo 'Listar condutores'
      echo ''
      ./lista_condutores.sh
      read ;;
      
    2)
      echo 'Altera taca de portagem'
      echo ''
      read -p 'Lanço           : ' l
      read -p 'Auto-estrada    : ' ae
      read -p 'Novo valor taxa : ' tx
      ./altera_taxa_portagem.sh ${l} ${ae} ${tx}
      read ;;
      
    3)
      echo 'Stats'
      echo ''
      
      until [ '${sm}' = '0' ]; do

        # Lista as opções disponíveis
        echo '1. Nome de todas as Autoestradas'
        echo '2. Registos de utilização'
        echo '3. Listagem condutores'
        echo '0. Voltar'
  
        # Pede ao utilizador para escolher uma opção
        echo ''
        read -p 'Opção: ' sm
        echo ''

        case ${sm} in
      
          1)
            # 1. Nome de todas as Autoestradas
            ./stats.sh 'listar'
            read ;;
       
          2)
            # 2. Registos de utilização
            read -p 'Mínimo de registos : ' r
            echo $r
            ./stats.sh 'registos' ${r} 
            read ;;
        
          3)
            # 3. Listagem condutores
            ./stats.sh 'condutores'
            read ;;
        
          0)
            # Sai do menu
            break
            ;;
       
          *)
            # ERRO: O argumento recebido é inválido
            ./error 3 ${sm}
            read ;;
        
        esac
      
      done
      
      ;;
    4)
      echo 'Faturação'
      echo ''
      ./faturacao.sh
      read ;;
      
    0)
      # Sai do menu
      break
      ;;
      
    *)
      # ERRO: O argumento recebido é inválido
      ./error 3 ${m}
      read ;;

  esac
  
done
