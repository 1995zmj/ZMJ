# KMP算法

[参考1](https://zhuanlan.zhihu.com/p/105629613)

[参考2](https://blog.csdn.net/dark_cy/article/details/88698736?utm_medium=distribute.pc_relevant.none-task-blog-baidulandingword-2&spm=1001.2101.3001.4242)

```c++
void Getnext(int next[],String t)
{
   int j=0,k=-1;
   next[0]=-1;
   while(j<t.length-1)
   {
      if(k == -1 || t[j] == t[k])
      {
         j++;k++;
         next[j] = k;
      }
      else k = next[k];//此语句是这段代码最反人类的地方，如果你一下子就能看懂，那么请允许我称呼你一声大神！
   }
}
```