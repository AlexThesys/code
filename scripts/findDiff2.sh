#!/bin/bash
path1=$1
path2=$2
path1_name=" "
echo $path1
echo $path2
#if [[ "${path1: -1}" != / ]] || [[ "${path2: -1}" != / ]] ; then
#    echo "Append '/' to the end of the path!"
#    exit
#fi
echo "//--------------------"
files_checked=0
folders_checked=0

for j in $path1/*; do
    dir=${j##*/}
    if [[ -d $path1/$dir ]] && [[ -e $path2/$dir ]]; then
        echo "=================================="
        echo "folder: " $dir
        let folders_checked++
        for i in $path1/$dir/*.*; do 
            file=${i##*/}
            if [[ -f $i ]]; then
                echo "//------------------------------"
                echo "file: " $file
                let files_checked++
                if [[ -e $path2/$dir/$file ]]; then
                    diff $path1/$dir/$file $path2/$dir/$file
                    if [[ $path1/$dir/$file -ef $path2/$dir/$file ]]; then
                        echo "Same files!"
                    fi
                else
                    echo "No matching file!"
                fi
            fi
        done
    fi
done
echo "//=============================="
echo "folders checked: " $folders_checked
echo "files checked: " $files_checked
