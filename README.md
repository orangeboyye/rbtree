# rbtree
这是一种左斜红黑树的实现。


rbtree.c 是红黑树实现的关键代码。

queue.c 是打印红黑树和判断红黑树所用到的队列。

test.c 是测试代码。


运行 make 可生成 test 测试程序，

./test number

number为红黑树节点的个数，默认为10，

程序先随机生成number个各不相同且最大值为number的数值放入数组中，

然后依次把数组中的数插入到红黑树中并打印红黑树

并判断是否是红黑树，如果不是输出错误信息并退出。

然后再依次删除红黑树中的每个节点，

并判断是否是红黑树，如果不是输出错误信息并退出。

打印红黑树时，红色节点前标记一个 * 。

下面是一个运行实例

./test

----------------------------------

print the array

 8  3  1  2  7  6  5  9  4 10

insert 8 into the tree

 8
 
  the tree is red blcak tree
  

insert 3 into the tree

  8
  
*3

  the tree is red blcak tree
  

insert 1 into the tree

  3
  
 1 8
 
  the tree is red blcak tree
  

insert 2 into the tree

    3
    
  2   8
  
*1

  the tree is red blcak tree
  

insert 7 into the tree

    3
    
  2   8
  
*1  *7

  the tree is red blcak tree
  

insert 6 into the tree

        7
        
   *3       8
   
  2   6
  
*1

  the tree is red blcak tree

insert 5 into the tree

        7
        
   *3       8
   
  2   6
  
*1  *5

  the tree is red blcak tree
  

insert 9 into the tree

        7
        
   *3       9
   
  2   6  *8
  
*1  *5

  the tree is red blcak tree
  

insert 4 into the tree

        5
        
    3       7
    
  2   4   6   9
  
*1          *8

  the tree is red blcak tree
  

insert 10 into the tree

        5
        
    3       9
    
  2   4  *7   10
  
*1       6 8

  the tree is red blcak tree
  

delete 8 in the tree

        5
        
    3       9
    
  2   4   7   10
  
*1      *6

  the tree is red blcak tree

delete 3 in the tree

        5
        
    2       9
    
  1   4   7   10
  
        *6
        
  the tree is red blcak tree
  

delete 1 in the tree

        9
        
   *5       10
   
  4   7
  
*2  *6

  the tree is red blcak tree
  

delete 2 in the tree

        9
        
   *5       10
   
  4   7
  
    *6
    
  the tree is red blcak tree
  

delete 7 in the tree

    9
    
 *5   10
 
 4 6
 
  the tree is red blcak tree
  

delete 6 in the tree

    9
    
  5   10
  
*4

  the tree is red blcak tree
  

delete 5 in the tree

  9
  
 4 10
 
  the tree is red blcak tree
  

delete 9 in the tree

  10
  
*4

  the tree is red blcak tree
  

delete 4 in the tree

 10
 
  the tree is red blcak tree
  

delete 10 in the tree

[ empty tree ]

the tree is red blcak tree


