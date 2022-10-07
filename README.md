# NoVMT
Detect Virtual Table address swapping.

### About
This is a simple project which detects any Virtual Method Table (VMT) pointer swapping. VMT pointer swapping is typically done to hook virtual functions (or intercept them) to have them execute the code you'd like.

### Purpose
The purpose of this is to show that it's very simple to detect VMT pointer swapping, and it should be avoided as a primary hooking method.


Side note: The anti-cheat known as Valve Anti-Cheat (VAC) has a very similar method emplaced. What they do is check if the original pointer of the Virtual Table has been changed.
