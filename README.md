# DEBUG:
第一版： 
- 应当在遇到空子句时直接返回false

第二版： 
- 用其它函数调用Queue时应当使用引用传递，因为Queue中的Node的指针与原来的相同，在函数中被删除后不会改变原queue的headNode，导致爆炸。