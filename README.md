# DEBUG:
第 $1$ 版： 

第 $2$ 版：  
$1.$ 用其它函数调用Queue时应当使用引用传递，因为Queue中的Node的指针与原来的相同，在函数中被删除后不会改变原queue的headNode，导致爆炸。  
$2.$ 用literalList遍历literal时不仅要判断该literal是否在CNFList中，还要判断clause在不在。因为在clause pullOut的时候没有把里面的literal->inCNFList一起更新()。  
$3.$ 由于literalList中的元素在被pull out时会重置nextPal和prePal，故需要用tempLiteral来承接pLiteral。  
$4.$ 在pull out literal中加了一行判断：`if(!clause->inCNFList)return ;` ，导致在回溯删除添加的单子句时直接return了！  
$5.$ 当然，`if(!liter->inCNFList)`也需要删除。  
$6.$ 当然，pull out clause的时候也是因为在调用deleteClause的时候使用了pull out clause，所以确实它会有可能不在CNFList中。
