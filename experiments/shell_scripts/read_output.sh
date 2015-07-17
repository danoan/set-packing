output_folder="output/experiment/K"

#Put the parameters in every line of the output
# for n in `ls $output_folder`
# do
#     echo $n
#     sed "s/OK/`awk -F\"\\t\" '{if(\$1==\"PARAMETERS\") print $2, $3, $4, $5}' ${output_folder}/${n}`/" -i ${output_folder}/$n
# done

#'pb_100rnd0100.dat' 'pb_100rnd0200.dat' 'pb_100rnd0300.dat' 'pb_100rnd0400.dat' 'pb_100rnd0500.dat' 'pb_100rnd0600.dat' 'pb_100rnd0700.dat' 'pb_100rnd0800.dat' 'pb_100rnd0900.dat' 'pb_100rnd1000.dat' 'pb_100rnd1100.dat' 'pb_100rnd1200.dat' 'pb_200rnd0100.dat' 'pb_200rnd0200.dat' 'pb_200rnd0300.dat' 'pb_200rnd0400.dat' 'pb_200rnd0500.dat' 'pb_200rnd0600.dat' 'pb_200rnd0700.dat' 'pb_200rnd0800.dat' 'pb_200rnd0900.dat' 'pb_200rnd1000.dat' 'pb_200rnd1100.dat' 'pb_200rnd1200.dat' 'pb_200rnd1300.dat' 'pb_200rnd1400.dat' 'pb_200rnd1500.dat' 'pb_200rnd1600.dat' 'pb_200rnd1700.dat' 'pb_200rnd1800.dat' 'pb_500rnd0100.dat' 'pb_500rnd0200.dat' 'pb_500rnd0300.dat' 'pb_500rnd0400.dat' 'pb_500rnd0500.dat' 'pb_500rnd0600.dat' 'pb_500rnd0700.dat' 'pb_500rnd0800.dat' 'pb_500rnd0900.dat' 'pb_500rnd1000.dat' 'pb_500rnd1100.dat' 'pb_500rnd1200.dat' 'pb_500rnd1300.dat' 'pb_500rnd1400.dat' 'pb_500rnd1500.dat' 'pb_500rnd1600.dat' 'pb_500rnd1700.dat' 'pb_500rnd1800.dat' 'pb_1000rnd0100.dat' 'pb_1000rnd0200.dat' 'pb_1000rnd0300.dat' 'pb_1000rnd0400.dat' 'pb_1000rnd0500.dat' 'pb_1000rnd0600.dat' 'pb_1000rnd0700.dat' 'pb_1000rnd0800.dat' 'pb_2000rnd0100.dat' 'pb_2000rnd0200.dat' 'pb_2000rnd0300.dat' 'pb_2000rnd0400.dat' 'pb_2000rnd0500.dat' 'pb_2000rnd0600.dat' 'pb_2000rnd0700.dat' 'pb_2000rnd0800.dat'
for i in 'pb_100rnd0100.dat'
do
    echo "*****".$i."******"
    # awk -F"\t" "{if(\$1==\"$i\") print;}" $output_folder/*.* | awk -f awk_print_line -v line=`awk -F"\t" "{if(\\$1==\"$i\") print;}" $output_folder/*.* | awk -F"\t" -f 'awk_max_script'`
    # awk -F"\t" "{if(\$1==\"$i\") print;}" $output_folder/*.* | cat awk -f awk_print_line -v line=

    # teste=`awk -F"\t" '{if($1=="$i") print;}' $output_folder/*.* | awk -F\"\t\" -f 'awk_max_script'`
    # echo "${teste}"

    max_lines=`awk -F"\t" "{if(\\$1==\"$i\") print;}" $output_folder/*.* | awk -F"\t" -f 'awk_max_script' -v column=2`
    min_lines=`awk -F"\t" "{if(\\$1==\"$i\") print;}" $output_folder/*.* | awk -F"\t" -f 'awk_min_script' -v column=3`

    min_gap=`awk -F"\t" "{if(\\$1==\"$i\") print;}" $output_folder/*.* | awk -F"\t" -f 'awk_min_script' -v column=4`

    # echo $min_lines    

    awk -F"\t" "{if(\$1==\"$i\") print;}" $output_folder/*.* > teste.txt


    echo "BETTER PRIMAL"
    echo $max_lines | cat - teste.txt | awk -f awk_print_line

    echo "BETTER DUAL"
    echo $min_lines | cat - teste.txt | awk -f awk_print_line    

    echo "BETTER GAP"
    echo $min_gap | cat - teste.txt | awk -f awk_print_line        

    echo
    echo

done