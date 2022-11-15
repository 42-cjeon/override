> Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H

level03의 어셈블리 코드를 보면

scanf("%d", &v); 같은 방법으로 정수 하나를 입력 받은 뒤 그 값을 0x1337d00d과 함께 test 함수에 넣는다.

```
 80488b1:	b8 85 8a 04 08       	mov    $0x8048a85,%eax # "%d"
 80488b6:	8d 54 24 1c          	lea    0x1c(%esp),%edx
 80488ba:	89 54 24 04          	mov    %edx,0x4(%esp)
 80488be:	89 04 24             	mov    %eax,(%esp)
 80488c1:	e8 6a fc ff ff       	call   8048530 <__isoc99_scanf@plt>
 80488c6:	8b 44 24 1c          	mov    0x1c(%esp),%eax # result of scanf (%d)
 80488ca:	c7 44 24 04 0d d0 37 	movl   $0x1337d00d,0x4(%esp) # magic number
 80488d1:	13 
 80488d2:	89 04 24             	mov    %eax,(%esp)
 80488d5:	e8 6d fe ff ff       	call   8048747 <test> # test(scan_decimal, 0x1337d00d);
```

test함수는 꽤 복잡하지만, 천천히 흐름을 타 보면 함수의 인자의 차를 0x15와 비교하여 그보다 클 경우 함정에 빠지고, 그렇지 않은 경우에는 값에 4를 곱한 것을 인덱스로 해서 0x80489f0 주소의 배열에 접근해 값을 가져오고, 그 값을 주소로 취급하여 jmp하는 부분이 있다.

```
8048747:	55                   	push   %ebp
8048748:	89 e5                	mov    %esp,%ebp
804874a:	83 ec 28             	sub    $0x28,%esp
804874d:	8b 45 08             	mov    0x8(%ebp),%eax # Input
8048750:	8b 55 0c             	mov    0xc(%ebp),%edx # MagicNumber
8048753:	89 d1                	mov    %edx,%ecx
8048755:	29 c1                	sub    %eax,%ecx # Input - MagicNumber
8048757:	89 c8                	mov    %ecx,%eax
8048759:	89 45 f4             	mov    %eax,-0xc(%ebp)
804875c:	83 7d f4 15          	cmpl   $0x15,-0xc(%ebp) # [0..21]
8048760:	0f 87 e4 00 00 00    	ja     804884a <LABEL_TRAP>
 
8048766:	8b 45 f4             	mov    -0xc(%ebp),%eax # value = Input - MagicNumber
8048769:	c1 e0 02             	shl    $0x2,%eax # 4 * value
 											  0x804871c - 0x80489f0
											   0x80489f0 ~ 0x8048a44
804876c:	05 f0 89 04 08       	add    $0x80489f0,%eax
8048771:	8b 00                	mov    (%eax),%eax
8048773:	ff e0                	jmp    *%eax
```

0x80489f0 주소의 배열을 보면

```
0x0:  0x0804884a, 0x08048775, 0x08048785, 0x08048795,
0x4:  0x080487a5, 0x080487b5, 0x080487c5, 0x080487d5,
0x8:  0x080487e2, 0x080487ef, 0x0804884a, 0x0804884a,
0xc:  0x0804884a, 0x0804884a, 0x0804884a, 0x0804884a,
0x10: 0x080487fc, 0x08048809, 0x08048816, 0x08048823,
0x14: 0x08048830, 0x0804883d
```

다음과 같은 함수 포인터들이 존재한다.

모든 함수 위치를 직접 찾아보는 것보다 0x1337d00d과의 차가 0 ~ 0x15인 모든 값을 시도해보는 것이 더 빠를 것 같다.

322424824 ~ 322424845의 값을 전부 넣었을 때 322424827이 원하는 값임을 알 수 있다.

```bash
cat <(echo 322424827) - | ./level03
```

