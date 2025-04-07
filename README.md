# CompOrg2025_HW1

## Directory Structure (upload file)
```
E94121038_HW1.zip/
├── E94121038_HW1/
    ├── README.md
    ├── Makefile
    ├── array_search.c
    ├── array_sort.c
    ├── linked_list_sort.c
```

## Directory Structure (here)
```
E94121038_HW1/
    ├── README.md
    ├── CO_StudentID_HW1
    ├   ├── Makefile
    ├   ├── array_search.c
    ├   ├── array_sort.c
    ├   ├── linked_list_sort.c
    ├   ├── judge1.conj
    ├   ├── judge2.conj
    ├   ├── judge3.conj
    ├── testcases
        ├── expected
        ├   ├── 1_1.out
        ├   ├── 1_2.out
        ├   ├── 1_3.out
        ├   ├── 1_4.out
        ├   ├── 2_1.out
        ├   ├── 2_2.out
        ├   ├── 3_1.out
        ├   ├── 3_2.out
        ├── input
            ├── 1_1.txt
            ├── 1_2.txt
            ├── 1_3.txt
            ├── 1_4.txt
            ├── 2_1.txt
            ├── 2_2.txt
            ├── 3_1.txt
            ├── 3_2.txt 
```

## Exercise 1 (array_sort)
1. 我們要做的事是完成判斷兩個數的排列是否正確(前面的數小於後面的數)  
  
2. 首先將j存進暫存器t0裡，但因為我們j的目的是要讀取p_a的下j位，因此我們將j\*4bytes(整數array一格式32bits)也就是向左位移2bits，接下來就是將p_a + j\*4的值存進t0裡  
  
3. 下一步就是將t0指向的值與t0下一個位置指向的值分別存到t1, t2裡，若t1 <= t2就不執行後方程式(跳到1)，否則繼續執行  
  
4. 最後就是將t1, t2的值交換  
  
## Exercise 2 (array_search)
1. 我們要做的事是在array(*p_a)裡找到target值的位置，並且回傳此位置(result)  
  
2. 第一步先建一個 t2 = 0 來標記我們做到第幾個位置了  
  
3. 第一個block(1)，先確認是否跑出array的size，再將t2存進暫存器t0裡，但因為我們t0的目的是要讀取p_a的下t2位，因此我們將t2\*4bytes(整數array一格式32bits)也就是向左位移2bits，接下來就是將p_a + t2\*4的值存進t0裡  
  
4. 將t0指向的值存到t1裡，若 t1 != target 就跳往第三個block，否則將t2的值存到result裡  
  
5. 第二個block(2)什麼事都不用做，負責結束  
  
6. 第三個block(3)負責將t2加一再跳回1  
## Exercise 3 (linked_list_sort)
我們要做的是有三部分，第一是將一個linked list拆一半，我的方法是用快慢指標來實作  
第二是將兩個以排列好的linked list合併，第三是將cur = cur -> next  

### 第一部分(splitList)
1. 首先將head放進t0, t1裡，檢查這兩個是否為NULL，若是就跳到*3*。還有將t1 -> next放入t3，若t3 == NULL，跳到*3*  
2. *1*是處理每一次指標的移動，但若遇到t1 -> next == NULL or t1 -> next -> next == NULL就跳到*2*，否則就做t0 = t0 -> next (slow), t1 = t1 -> next -> next (fast)，最後再回到*1*重新來一遍
3. *2*是將t2 = t0 -> next，再將t0 -> next = NULL，原本的head為第一個list，t2為第二個list
4. *3*代表結束

### 第二部分(mergeSortedLists)
1. *1*先確認a, b == NULL時跳往*3*，否則進行比較，將a -> data, b -> data存入t0, t1，若t0 <= t1跳往*5*，否則用t2紀錄b，t3紀錄b -> next，將b = b -> next，最後跳往*6*
2. *2*表示結束
3. *3*是在處理若倆個list已經NULL了就跳往*2*，然後若res仍為NULL跳往*4*，否則將剩餘的部分全部接到tail後，再跳往*2*
4. *4*是將剩餘的等於res
5. *5*是用t2紀錄a，t3紀錄a -> next，將a = a -> next，最後跳往*6*(跟*1*最後很像，只是是在處理a)
6. *6*先將t2 -> next = NULL，如果res == NULL跳往*7*，否則將t2存入tail -> next後再跳往*8*
7. *7*是將res = t2
8. *8*是將tail = t2，再跳往*1*
#### 遇到問題
一開始不知道Node的在記憶體占多少Byte，所以使用0(t0)代表t0 -> data，4(t0)代表t0 -> next  
但最後發現next是存在offset8，所以8(t0)才是代表t0 -> next，然後在這個架構裡(RV64)lw、sw要改成ld、sd
