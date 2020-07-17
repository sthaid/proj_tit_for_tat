#!/bin/python

print("hello")

a, b = 0, 1
while a < 10:
    print(a)
    a, b = b, a+b


#x = int(input("Please enter an integer: "))
x = 1
if x < 0:
    x = 0
    print('Negative changed to zero')
elif x == 0:
    print('Zero')
elif x == 1:
    print('Single')
else:
    print('More')

# Measure some strings:
words = ['cat', 'window', 'defenestrate']
for w in words:
    print(w, len(w))

for i in range(5):
    print(i)

for i in range(5,10):
    print(i)

a = ['Mary', 'had', 'a', 'little', 'lamb']
for i in range(len(a)):
    print(i, a[i])

x = sum(range(4))  # 0 + 1 + 2 + 3
print(x)

for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(n, 'equals', x, '*', n//x)
            break
    else:
        # loop fell through without finding a factor
        print(n, 'is a prime number')

for num in range(2, 10):
    if num % 2 == 0:
        print("Found an even number", num)
        continue
    print("Found a number", num)

def fib(n):    # write Fibonacci series up to n
    """Print a Fibonacci series up to n."""
    a, b = 0, 1
    while a < n:
        print(a, end=' ')
        a, b = b, a+b
    print()

fib(100)
f = fib
f(100)
ret = f(5)
print(ret)


def fib2(n):  # return Fibonacci series up to n
    """Return a list containing the Fibonacci series up to n."""
    result = []
    a, b = 0, 1
    while a < n:
        result.append(a)    # see below
        a, b = b, a+b
    return result

f100 = fib2(100)    # call it
print(f100)                # write the result


def ask_ok(prompt, retries=4, reminder='Please try again!'):
    while True:
        ok = input(prompt)
        if ok in ('y', 'ye', 'yes'):
            return True
        if ok in ('n', 'no', 'nop', 'nope'):
            return False
        retries = retries - 1
        if retries < 0:
            raise ValueError('invalid user response')
        print(reminder)

#print(ask_ok("OK TO CONTINUE? "))

def make_incrementor(n):
    return lambda x: x + n
f = make_incrementor(42)
print(f(0))
print(f(1))


def make_incrementor2(n):
    def xxx(z):
        return n+z
    return xxx
f2 = make_incrementor2(42)
print(f2(0))
print(f2(1))


  
def func():
    cnt = 0
    while (cnt < 10):
        str = input("prompt ")
        print(str)
        cnt = cnt + 1

func()

