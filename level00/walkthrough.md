> Password: level00

level00 프로그램의 어셈블리 코드를 보면

scanf("%d")를 이용해서 정수 입력을 받은 뒤, 그 값을 0x149c과 비교하여 같을 경우 쉘을 실행하는 코드임을 확인할 수 있다.

```
80484de:	e8 ed fe ff ff       	call   80483d0 <__isoc99_scanf@plt>
 80484e3:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 80484e7:	3d 9c 14 00 00       	cmp    $0x149c,%eax
```

따라서 프로그램의 표준 입력으로 5276(0x149c)을 넣으면 쉘을 획득할 수 있다.

```bash
cat <(echo 5276) - | ./level00
```
