# DEBUG:
第一版： 

第二版： 
- 用其它函数调用Queue时应当使用引用传递，因为Queue中的Node的指针与原来的相同，在函数中被删除后不会改变原queue的headNode，导致爆炸。
- 用literalList遍历literal时不仅要判断该literal是否在CNFList中，还要判断clause在不在。因为在clause pullOut的时候没有把里面的literal->inCNFList一起更新()。
- 由于literalList中的元素在被pull out时会重置nextPal和prePal，故需要用tempLiteral来承接pLiteral。
- 