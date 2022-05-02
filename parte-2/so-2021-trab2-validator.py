#!/usr/bin/env python3

##########################################################################
# ISCTE-IUL: 2.º - Trabalho prático de Sistemas Operativos 2021/2022
#
# Avaliação automática de 2 trabalho
#
# Versão 0.1
##########################################################################

import os
import argparse

##########################################################################
# Main
#
##########################################################################

parser = argparse.ArgumentParser( description="Evaluate test problem",
    usage = '%(prog)s <source>' )

parser.add_argument( 'Source', 
    metavar='source', nargs = '?', default = None,
    type =str, 
    help='Directory with source files to evaluate' )

# Parse command line arguments
args = parser.parse_args()

if ( args.Source is None ):
    print("Source directory not specified")
    parser.print_usage()
    exit(1)

# Get test directory
testdir = args.Source

# Check if directory exists
if (not os.path.exists( testdir )):
    print('(*error*) Directory "{}" does not exist'.format(testdir))
    exit(1)

# Check if it is current directory
if ( os.path.samefile(".", testdir)):
    print('(*error*) Source must not be local (.) directory')
    exit(2)

# Run tests
print("Evaluating directory '{}'...".format(testdir))

os.system("make")
os.system("./cliente-eval")
os.system("./servidor-eval")

print("\ndone.")
