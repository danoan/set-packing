declare la=(0.001 0.002 0.005 0.01 0.02 0.05)
declare lb=(0.9 0.8 0.7 0.6 0.5)
declare lc=('C' 'L')
declare ld=(5 10 15 20)
declare le=('N' 'K')


bin_path="./build/bin/batch_run.out"


for e in 'N' 'K'
do          
    mkdir -p "output/experiment/$e"
    for a in 0.001 0.002 0.005 0.01 0.02 0.05
    do
        for b in 0.9 0.8 0.7 0.6 0.5
        do
            for c in 'C' 'L'
            do
                for d in 5 10 15 20
                do        
                    echo   "/$e/${d}IT_${c}HEUR_${b}PI_${a}GAP"
                    $bin_path -i S -g $a -p $b -h $c -n $d -f $e > "output/experiment/$e/${d}IT_${c}HEUR_${b}PI_${a}GAP"
                done

            done

        done

    done
done