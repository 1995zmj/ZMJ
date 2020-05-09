# ToDo

## C\# 使用技巧

获取变量名

```text
string abc = "123";
Debug.Log(nameof(abc));
```

模块

```text
#region " EMIT EVENT "
#endregion
```

```text
 public override bool Equals(object obj) 
{
    // If this and obj do not refer to the same type, then they are not equal.
    if (obj.GetType() != this.GetType()) return false;

    // Return true if  x and y fields match.
    var other = (Point) obj;
    return (this.x == other.x) && (this.y == other.y);
}
```

```text
EnumTest e = new EnumTest();
    string[] values = System.Enum.GetNames(e.GetType());
```

sealed （C\#中的修饰符） 防止类被继承

## C\#个人规范

[参考](https://www.cnblogs.com/cjm123/p/8571264.html) 变量都定义成私有

* 比较难以理解的函数要添加文档注释

[https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/classes-and-structs/using-properties](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/classes-and-structs/using-properties)

同一类型要统一管理



ref是有进有出，而out是只出不进

