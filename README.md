# rbtree
这是一种左斜红黑树的实现  
rbtree.c 是红黑树的实现代码  
queue.c 是打印红黑树和判断红黑树所用到的队列  
test.c 是测试代码  

运行 make 可生成 test 测试程序  
运行 make run 可以生成测试程序并运行之  
或者直接运行测试程序  
./test [number]  
number为红黑树节点的个数，默认为 10  

程序先随机生成number个各不相同且最大值为number的数值放入数组中  
然后依次把数组中的数插入到红黑树中并打印红黑树  
并判断是否是红黑树，如果不是输出错误信息并退出  
然后再依次删除红黑树中的每个节点  
并判断是否是红黑树，如果不是输出错误信息并退出  
打印红黑树时，红色节点前标记一个 *   

