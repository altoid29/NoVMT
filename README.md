# NoVMT
Detect Virtual Table address swapping.

## About
This is a simple project which detects any Virtual Method Table (VMT) pointer swapping. VMT pointer swapping is typically done to hook virtual functions (or intercept them) to have them execute the code you'd like.

## Purpose
The purpose of this is to show that it's very simple to detect VMT pointer swapping, and it should be avoided as a primary hooking method.
