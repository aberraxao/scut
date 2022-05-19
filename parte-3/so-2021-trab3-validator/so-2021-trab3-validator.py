#!/usr/bin/env python3

##########################################################################
# ISCTE-IUL: 2.º - Trabalho prático de Sistemas Operativos 2021/2022
#
# Avaliação automática de 1 trabalho
#
# Versão 0.3
##########################################################################

import subprocess
import os
import sys
import argparse

##########################################################################
# Utility functions
#
###########################################################################

if ( sys.stdout.encoding == "UTF-8" or sys.stdout.encoding == "utf-8" ):
  ok   = u"\u001b[1m\u001b[32m[✔]\u001b[0m"
  fail = u"\u001b[1m\u001b[31m[✗]\u001b[0m"
  warn = u"\u001b[1m\u001b[33m[⚠️]\u001b[0m"
  info = u"\u001b[1m\u001b[34m[ℹ︎]\u001b[0m"
else:
  ok = "\u001b[1m\u001b[32m[ ok ]\u001b[0m"
  fail = "\u001b[1m\u001b[31m[fail]\u001b[0m"
  warn = "\u001b[1m\u001b[33m[warn]\u001b[0m"
  info = "\u001b[1m\u001b[34m[info]\u001b[0m"

def create_file( filename, text ):
    """
    Creates a test file from a list of strings

    Parameters
    ----------
    filename : str
        Name of file to write, will be overriten
    text : list of str
        List of strings to write
    """
    f = open( filename, 'w' )
    for line in text:
        print( line, file = f )
    f.close()

def cleanup( files ):
    """Deletes file in list

    Parameters
    ----------
    files : list of str | str
        File or List of files to be deleted
    """
    if ( isinstance( files, str ) ):
        if ( os.path.exists(files) ):
            os.remove( files )
    else:
        for f in files:
            if ( os.path.exists(f) ):
                os.remove( f )

##########################################################################
# Test program
#
##########################################################################
def eval( t, prog ) :
    """Testa o programa especificado

    Parameters
    ----------
    t : str
        Source directory
    prog : str
        Programa a testar (sem extensão)
    """
    print( u"\n\u001b[1m[{}]\u001b[0m".format(prog))

    if (not os.path.exists( t + "/{}.c".format(prog) )):
        print( "1. (*error*) {} not found {}".format(t + "/{}.c".format(prog),fail) )
        return
    
    # Compile the code
    make = ['make', '-f', 'Makefile.{}'.format(prog), 'SOURCE='+t]

    try:
        print("{} Compiling {}/{}.c ...".format(info, t,prog))
        proc = subprocess.run( make, stdout=subprocess.PIPE )
        if ( proc.returncode == 0 ):
            print("{} Code compiled ok\n".format(ok));
            print("{} Evaluating {}/{}.c ...\n".format(info, t, prog))
            proc = subprocess.run( ['./{}-eval'.format(prog)] )
        else:
            print( "{} Unable to compile {}/{}.c, skipping test".format(fail,t,prog) )
    except:
        print( "{} Unable to compile {}/{}.c, skipping test".format(fail,t,prog) )

    # Cleanup test files
    cleanup( ["{}-eval".format(prog) ] )

##########################################################################
# Get grades
#
##########################################################################
def grade( t, prog ) :
    """Avalia o programa especificado

    Parameters
    ----------
    t : str
        Source directory
    prog : str
        Programa a testar (sem extensão)
    """

    if (not os.path.exists( t + "/{}.c".format(prog) )):
        print( "(*error*) {} not found {}".format(t + "/{}.c".format(prog),fail) )
        return
    
    # Compile the code
    make = ['make', '-f', 'Makefile.{}'.format(prog), 'SOURCE='+t]

    grades = dict()

    try:
        proc = subprocess.run( make, stdout=subprocess.PIPE )
        if ( proc.returncode == 0 ):
            proc = subprocess.run( ['./{}-eval'.format(prog),'-x'], stdout=subprocess.PIPE )            
        else:
            print( "{} Unable to compile {}/{}.c, skipping test".format(fail,t,prog) )
            return dict()
    except:
        print( "{} Unable to compile {}/{}.c, skipping test".format(fail,t,prog) )
        return dict()

    # Cleanup test files
    cleanup( ["{}-eval".format(prog) ] )

    stdout = proc.stdout.decode("utf-8").split('\n')

    idx = stdout.index('{}:grade'.format(prog))
    for item in stdout[idx+1].split(','):
        values = item.split(':')
        grades[ values[0] ] = float(values[1])

    return grades

##########################################################################
# Main
#
##########################################################################

parser = argparse.ArgumentParser( description="Evaluate test problem",
    usage = '%(prog)s [-g] <source>' )

parser.add_argument( "-g","--grade", dest="grade", action='store_true', \
                    help = "Generate grades")

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

if ( args.grade ):
    print("Grading directory '{}'...".format(testdir))
    print("cliente : ", grade( testdir, "cliente" ))
    print("servidor: ", grade( testdir, "servidor" ))
else:
    print("Evaluating directory '{}'...".format(testdir))
    eval( testdir, "cliente" )
    eval( testdir, "servidor" )

print("\ndone.")
