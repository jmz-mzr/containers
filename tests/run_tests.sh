#!/bin/bash

realpath() {
	case "$1" in
		/*) echo "$1" ;;
		*) echo "$PWD/${1#./}" ;;
	esac
}

if [ -n "$BASH_SOURCE" ]; then
	SELF_DIR="$(dirname "${BASH_SOURCE[0]}")"
else
	SELF_DIR="$(dirname "$0")"
fi

WORKDIR="$(dirname "$(realpath "$SELF_DIR")")"

OWN_TESTER="$WORKDIR/ft_containers_tests"
OWN_TESTER_STD="$WORKDIR/ft_containers_tests_std"

TESTER_42="$WORKDIR/42_tests"
TESTER_42_STD="$WORKDIR/42_tests_std"

RAND_NB=$(awk 'BEGIN { srand(); print int(rand() * 100) }')

HACK_DEMO="$WORKDIR/demo_hack_stack"

print_header() {
	echo -e "\033[0;33m"
    echo -e "         )     )                (       )     (    (                (              (     "
    echo -e "   (  ( /(  ( /(   *   )  (     )\ ) ( /(     )\ ) )\ )    *   )    )\ )  *   )    )\ )  "
    echo -e "   )\ )\()) )\())\` )  /(  )\   (()/( )\())(  (()/((()/(  \` )  /((  (()/(\` )  /((  (()/(  "
    echo -e " (((_|(_)\ ((_)\  ( )(_)(((_)(  /(_)|(_)\ )\  /(_))/(_))  ( )(_))\  /(_))( )(_))\  /(_)) "
    echo -e " )\___ ((_) _((_)(_(_()))\ _ )\(_))  _((_|(_)(_)) (_))   (_(_()|(_)(_)) (_(_()|(_)(_))   "
    echo -e "((/ __/ _ \| \| ||_   _|(_)_\(_)_ _|| \| | __| _ \/ __|  |_   _| __/ __||_   _| __| _ \  "
    echo -e " | (_| (_) | .\` |  | |   / _ \  | | | .\` | _||   /\__ \    | | | _|\__ \  | | | _||   /  "
    echo -e "  \___\___/|_|\_|  |_|  /_/ \_\|___||_|\_|___|_|_\|___/    |_| |___|___/  |_| |___|_|_\  "
	echo "                                                                      ";
	echo -e "\033[m"
}

compile_testers() {
    grey="\033[0;90m"
    reset="\033[m"

	if ! [ -x "$OWN_TESTER" ] || ! [ -x "$OWN_TESTER_STD" ]; then
		make -sC "$WORKDIR"
        printf "\n$grey%s$reset\n\n" "---------------------------------------------------"
	fi
	if ! [ -x "$TESTER_42" ] || ! [ -x "$TESTER_42_STD" ]; then
		make -sC "$WORKDIR" "tester_42"
        printf "\n$grey%s$reset\n\n" "---------------------------------------------------"
	fi
	if ! [ -x "$HACK_DEMO" ]; then
		make -sC "$WORKDIR" "demo"
        printf "\n$grey%s$reset\n\n" "---------------------------------------------------"
	fi
}

compare_output() {
    tmpfile1=$(mktemp __std_version_output__.tmp.XX)
    tmpfile2=$(mktemp __test_version_output__.tmp.${tmpfile1#*.tmp.})
    trap 'rm -f "$tmpfile1" "$tmpfile2"' EXIT

    printf "%-50s" "Comparing output"

    "$1" "$3" > "$tmpfile1" 2>&1
    "$2" "$3" > "$tmpfile2" 2>&1

    res=0

    if diff -q "$tmpfile1" "$tmpfile2" >/dev/null 2>&1; then
        printf "✅\n\n"
    else
        res=1
        printf "❌ \n\n"
        mkdir -p "$WORKDIR/tests/results"
        suffix="$(date +'%F_%T')_${tmpfile1#*.tmp.}"
        std_version="$WORKDIR/tests/results/std_version_output_$suffix"
        your_version="$WORKDIR/tests/results/your_version_output_$suffix"
        mv "$tmpfile1" "$std_version"
        mv "$tmpfile2" "$your_version"
        test_file="the relevant 'xxx__tests.cpp'"
        test_dir="'tests/srcs/'"
        if [ "$3" != "" ]; then
            test_file="the '42_main.cpp'"
            test_dir="'tests/'"
        fi
        grey="\033[0;90m"
        reset="\033[m"
        printf "$grey%s\n%s\n%s\n%s$reset\n\n" "Calm down! It doesn't mean it's wrong." \
            "Go see more about the output differences in 'tests/results', and look at" \
            "$test_file file in the $test_dir folder to understand what happened." \
            "Here is the diff:"
        diff -u5 --color=always "$std_version" "$your_version"
        echo
    fi

    rm -f "$tmpfile1" "$tmpfile2"
    return $res
}

compare_time() {
    ctn="all containers'"
    if [ "$3" = "-v" ]; then ctn="'vector'"
    elif [ "$3" = "-s" ]; then ctn="'stack'"
    elif [ "$3" = "-m" ]; then ctn="'map'"
    elif [ "$3" = "-S" ]; then ctn="'set'"
    fi
    width=$(( 39 - ${#ctn} ))
    printf "Comparing %s %-*s" "$ctn" $width "time performances"

    execution_time_std=$( { time -p "$1" "$3" 2>&1 1>/dev/null; } 2>&1 )
    real_time_std=$(echo "$execution_time_std" | grep '^real' | cut -d' ' -f2)
    user_time_std=$(echo "$execution_time_std" | grep '^user' | cut -d' ' -f2)
    sys_time_std=$(echo "$execution_time_std" | grep '^sys' | cut -d' ' -f2)

    execution_time=$( { time -p "$2" "$3" 2>&1 1>/dev/null; } 2>&1 )
    real_time=$(echo "$execution_time" | grep '^real' | cut -d' ' -f2)
    user_time=$(echo "$execution_time" | grep '^user' | cut -d' ' -f2)
    sys_time=$(echo "$execution_time" | grep '^sys' | cut -d' ' -f2)

    res=0

    if echo "$real_time_std * 10 < $real_time" | bc -l | grep -q '^1$' ||
            echo "$user_time_std * 10 < $user_time" | bc -l | grep -q '^1$' ||
            echo "$sys_time_std * 10 < $sys_time" | bc -l | grep -q '^1$'; then
        res=1
    fi
    if [ $res -eq 0 ]; then
        printf "✅\n\n"
    else
        printf "❌ \n\n"
    fi

    red="\033[0;31m"
    reset="\033[m"
    printf "  YOUR VERSION:   |  STD VERSION:\n"
    echo "-----------------------------------"
    if echo "$real_time_std * 10 < $real_time" | bc -l | grep -q '^1$'; then
        printf "$red"
    fi
    printf "   real: %-8s |   real: %s$reset\n" $real_time $real_time_std
    if echo "$user_time_std * 10 < $user_time" | bc -l | grep -q '^1$'; then
        printf "$red"
    fi
    printf "   user: %-8s |   user: %s$reset\n" $user_time $user_time_std
    if echo "$sys_time_std * 10 < $sys_time" | bc -l | grep -q '^1$'; then
        printf "$red"
    fi
    printf "    sys: %-8s |    sys: %s$reset\n" $sys_time $sys_time_std
    echo

    return $res
}

run_own_tests() {
    blue="\033[0;36m"
    grey="\033[0;90m"
    reset="\033[m"

    while true; do
        printf '\033[1;37mSelect a version to run\033[m\n'
        printf "1)$blue With all containers tested for time performances$reset\n"
        printf "2)$blue With 'vector' time performances$reset   3)$blue With 'stack' time performances$reset\n"
        printf "4)$blue With 'map' time performances$reset      5)$blue With 'set' time performances$reset\n"
        printf "6)$grey Without time performances$reset\n"
        printf '\033[1;37mYour choice:\033[m '
        read -r choice

        case $choice in
            1) opt="-a" ; break ;;
            2) opt="-v" ; break ;;
            3) opt="-s" ; break ;;
            4) opt="-m" ; break ;;
            5) opt="-S" ; break ;;
            6) opt="-q" ; break ;;
            *) echo "Invalid option" ;;
        esac
        echo
    done
    echo

    compare_output $OWN_TESTER_STD $OWN_TESTER
    output_res=$?
    if [ "$opt" != "-q" ]; then
        compare_time $OWN_TESTER_STD $OWN_TESTER "$opt"
        time_res=$?
    else
        time_res=0
    fi

    if [ $output_res -ne 0 ] || [ $time_res -ne 0 ]; then
        return 1
    fi
    return 0
}

run_42_tests() {
    compare_output $TESTER_42_STD $TESTER_42 $RAND_NB
    output_res=$?
    compare_time $TESTER_42_STD $TESTER_42 $RAND_NB
    time_res=$?

    if [ $output_res -ne 0 ] || [ $time_res -ne 0 ]; then
        return 1
    fi
    return 0
}

run_demo() {
    $HACK_DEMO
    sleep 2
}

options=($'\033[0;36mHome made tester\033[m' $'\033[0;36m42 tester   \033[m' $'\033[0;36mHack stack demo\033[m' $'\033[0;90mQuit\033[m')
PS3=$'\n\033[1;37mSelect a tester to run:\033[m '

print_header
compile_testers
while true; do
	select opt in "${options[@]}"
	do
        ret=0
        demo=0
		case $opt in
			$'\033[0;36mHome made tester\033[m') echo ; run_own_tests ; ret=$? ; break ;;
			$'\033[0;36m42 tester   \033[m') echo ; run_42_tests ; ret=$? ; break ;;
            $'\033[0;36mHack stack demo\033[m') echo ; run_demo ; demo=1; break ;;
			$'\033[0;90mQuit\033[m') break 2 ;;
			*) demo=1 ; echo "Invalid option $REPLY" ; break ;;
		esac
	done
	print_header
    if [ $ret -ne 0 ]; then
        echo -e "❌  \t\033[0;31mYou n00b, you failed!!!\033[m\n"
    elif [ "$demo" -ne 1 ]; then
        echo -e "✅  \t\033[0;32mYou're a roxXxor, GG!!!\033[m\n"
    fi
done

exit 0
