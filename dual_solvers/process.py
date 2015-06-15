#!/usr/bin/python

import sys
import subprocess

MAIN_FOLDER = "processed_files"

def process_pd_mink(filename):
    '''
        Given a output file from an execution of batch_input while executing both knapsack problem solver
        (Standard Dynamic Programming and Original Minknap algorithm from Pisinger) extracts the correspondent
        results of each lagragean subproblem of each instance

        It outputs all the files in the processed_files folder
        shell=True is DANGEROUS!!!!
    '''
    SUB_FOLDER = MAIN_FOLDER + "/process_pd_mink"
    FORMATTED_FOLDER = SUB_FOLDER + "/only_values"

    #Get the names of instances: pb_100rnd0100.dat
    output = subprocess.check_output( "grep %s %s | cut -d' ' -f2 " % ("\"\\*\\*\\*\\*.*$\"", filename), shell=True )
    instances_names = output.split("\n")


    #Get the byte where the data instance begins
    output = subprocess.check_output( "grep %s %s -b" % ("\"\\*\\*\\*\\*.*$\"",filename), shell=True )
    data_begins = map( lambda x: int( x.split(":")[0] ) if x!="" else None, output.split("\n") )

    #Get the byte where the data instances finishes
    output = subprocess.check_output( "grep %s %s -b" % ("SIMPLE:",filename), shell=True )
    data_ends = map( lambda x: int( x.split(":")[0] ) if x!="" else None, output.split("\n") )


    #Get data from a instance and print it in a separated file
    for i in xrange(0,len(data_begins)):
        if data_begins[i]==None:
            continue
        count = data_ends[i]-data_begins[i]+1
        subprocess.call( "dd skip=%d count=%d bs=1 if=%s of=%s/%s.txt" % (data_begins[i]+1,count,filename,SUB_FOLDER,instances_names[i]), shell=True )


    #Extract the values of PD_STANDARD( KNAP ) e MINKNAP and put it on different files.
    for inst_nm in instances_names:
        if inst_nm==None:
            continue
        subprocess.call( "grep %s %s/%s | cut -d' ' -f3 > %s/%s_KNAP.txt" % ("\"VALUE KNAP:.*$\" ",SUB_FOLDER,inst_nm+".txt",FORMATTED_FOLDER, inst_nm), shell=True )
        subprocess.call( "grep %s %s/%s | cut -d' ' -f3 > %s/%s_MINKKNAP.txt" % ("\"VALUE MINKNAP:.*$\" ",SUB_FOLDER,inst_nm+".txt",FORMATTED_FOLDER, inst_nm), shell=True )



def process_noknap_withknap(filename):
    SUB_FOLDER = MAIN_FOLDER + "/process_noknap_withknap"
    FORMATTED_FOLDER = SUB_FOLDER + "/only_values"

    #Get the names of instances: pb_100rnd0100.dat
    output = subprocess.check_output( "grep %s %s | cut -d' ' -f2 " % ("\"\\*\\*\\*\\*.*$\"", filename), shell=True )
    instances_names = output.split("\n")

    #Get the byte where the data instance begins
    output = subprocess.check_output( "grep %s %s -b" % ("\"\\*\\*\\*\\*.*$\"",filename), shell=True )
    data_begins = map( lambda x: int( x.split(":")[0] ) if x!="" else None, output.split("\n") )

    data_ends = [ data_begins[i] -2 if data_begins[i]!="" and data_begins[i]!=None else None for i in xrange(1,len(data_begins)) ]


    output = subprocess.check_output( "grep %s %s | cut -d':' -f3 | cut -d' ' -f2" % ("SIMPLE.*$",filename) , shell=True)
    primal_simple = output.split("\n")

    output = subprocess.check_output( "grep %s %s | cut -d':' -f4 | cut -d' ' -f1" % ("SIMPLE.*$",filename) , shell=True)
    dual_simple = output.split("\n")    

    output = subprocess.check_output( "grep %s %s | cut -d':' -f5 | cut -d' ' -f2" % ("SIMPLE.*$",filename) , shell=True)
    best = output.split("\n")    




    output = subprocess.check_output( "grep %s %s | cut -d':' -f3 | cut -d' ' -f2" % ("MINKNAP.*$",filename) , shell=True)
    primal_mink = output.split("\n")

    output = subprocess.check_output( "grep %s %s | cut -d':' -f4 | cut -d' ' -f1" % ("MINKNAP.*$",filename) , shell=True)
    dual_mink = output.split("\n")    

    output = subprocess.check_output( "grep %s %s | cut -d'-' -f2-" % ("MINKNAP.*$",filename) , shell=True)
    result = output.split("\n")    


    output_str = "INSTANCE \t PRIMAL \t DUAL \t BEST VALUE\n"
    for i in xrange(0,len(instances_names)):
        output_str += "%s\t%s\t%s\t%s\n" % (instances_names[i], primal_simple[i], dual_simple[i], best[i] )
        output_str += "%s\t%s\t%s\t%s\n" % (instances_names[i], primal_mink[i], dual_mink[i], result[i] )

    with open("%s/table.txt" % (FORMATTED_FOLDER,), 'w') as f:
        f.write(output_str)


if __name__=="__main__":
    if( len(sys.argv)>=3 ):
        if sys.argv[2] == "pd_mink":
            process_pd_mink( sys.argv[1] )
        elif sys.argv[2] == "noknap_withknap":
            process_noknap_withknap( sys.argv[1] )
    else:
        print "You  must enter a filename to process"
        